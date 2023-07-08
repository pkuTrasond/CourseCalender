#include "link.h"
#include "ui_link.h"
#include <QDebug>
#include <QPushButton>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextCodec>

Link::Link(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Link)
{
    ui->setupUi(this);
    _codec = QTextCodec::codecForName("GBK");
    setWindowTitle("批量添加课程");

    //设置dialog button文字
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");


}

Link::~Link()
{
    delete ui;
}

void Link::on_buttonBox_accepted()
{
    QString fileloc=ui->fileloc->text();
    jsonloc=ui->jsonloc->text();
    if(fileloc=="")
    {
        QMessageBox::warning(this,"error","添加失败，输入地址不能为空");

        return;
    }
    if(jsonloc=="")
    {
        QMessageBox::warning(this,"error","添加失败，输出地址不能为空");

        return;
    }
    //解析文件
    QFile file(fileloc);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 设置文本流的编码格式为 UTF-8
        filecontent = in.readAll(); // 将文件内容读取到成员变量中
        qDebug() << "文件打开了";
        file.close();
    }
    //qDebug()<<filecontent;
    //读和写入
    readandwrite();

    //发送信号 让mainwindow重新读入并显示数据库里的数据
    emit back2Course(jsonloc);
    close();
}

void Link::clearEdit()
{
    ui->fileloc->clear();
}

void Link::readandwrite()
{
    int index=1;
    //匹配
    QRegularExpression regex(
        "<td class=\"datagrid\"><span>(.*?)</span></td>.*?"
        "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
        "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
        "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
        "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
        "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
        "<td class=\"datagrid\"><span>(.*?)</span></td>.*?"
        "<td class=\"datagrid\"><span>(.*?)</span></td>"
        );
    regex.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
    /*QRegularExpressionMatch findmatch=regex.match(filecontent);
    if (findmatch.hasMatch())
    {
        qDebug("读取到课程");
        qDebug()<<findmatch.captured(1);
        qDebug()<<findmatch.captured(2);
        return;
    }
    else
    {
        qDebug("没读取到课程");
        return;
    }
    */
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(filecontent);

    QJsonObject courseObj;  // 大类 "Course"
    QJsonArray courseArray;

    while (matchIterator.hasNext()) {

        QRegularExpressionMatch match = matchIterator.next();

        QString courseName = match.captured(1);
        QString teacher = match.captured(5);
        QString extraInfo = match.captured(8);
        qDebug()<<"找到课程 "<<index<<' ' <<courseName<< endl;

        // 匹配周几信息
        QRegularExpression dayRegex("每周周(.*?)\\d+~\\d+节");
        QRegularExpressionMatch dayMatch = dayRegex.match(extraInfo);
        QString courseDay = dayMatch.hasMatch() ? dayMatch.captured(1) : "";

        // 得到课程节的数字
        QRegularExpression timeRegex("每周.*?(\\d+)~(\\d+)节");
        QRegularExpressionMatch timeMatch = timeRegex.match(extraInfo);
        int timeBegin = timeMatch.hasMatch() ? timeMatch.captured(1).toInt() : 0;
        int timeEnd = timeMatch.hasMatch() ? timeMatch.captured(2).toInt() : 0;

        // 地点
        QString location;
        QRegularExpression locationRegex("节 (.*?)<br>");
        QRegularExpressionMatch locationMatch = locationRegex.match(extraInfo);
        if (locationMatch.hasMatch()) {
            location = locationMatch.captured(1);
        }

        // 提取考试信息，即 "考试时间：20240112晚上；"
        QString examInfo;
        QRegularExpression examRegex("<br>(.*?)$");
        QRegularExpressionMatch examMatch = examRegex.match(extraInfo);
        if (examMatch.hasMatch()) {
            examInfo = examMatch.captured(1);
        }
        qDebug()<<"找到课程"<< endl;
        //数据转换为json的格式，也就是day需要转换为int
        int dayNumber = -1; // 初始化 dayNumber

        if (courseDay == "一") {
            dayNumber = 1;
        } else if (courseDay == "二") {
            dayNumber = 2;
        } else if (courseDay == "三") {
            dayNumber = 3;
        } else if (courseDay == "四") {
            dayNumber = 4;
        } else if (courseDay == "五") {
            dayNumber = 5;
        } else {
            continue;

        }
        dayNumber--;

        //

        QJsonObject course;
        course.insert("id", QJsonValue(index));
        course.insert("name", QJsonValue(courseName));
        course.insert("course_day", QJsonValue(dayNumber));
        course.insert("course_time", QJsonValue(timeBegin));
        course.insert("course_end_time", QJsonValue(timeEnd));
        course.insert("location", QJsonValue(location));
        course.insert("teacher", QJsonValue(teacher));
        course.insert("exam_info", QJsonValue(examInfo));

        courseArray.append(QJsonValue(course));
//        courseMap.insert(index,courseName);
        index++;

    }
    id_count=index;
    courseObj.insert("Course", QJsonValue(courseArray));
    QJsonDocument jsonDoc(courseObj);

    QFile jsonFile(jsonloc);
    if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        jsonFile.write(jsonDoc.toJson());
        jsonFile.close();
    }
}
