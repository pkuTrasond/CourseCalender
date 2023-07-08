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
    setWindowTitle(_codec->toUnicode("批量添加课程"));

    //设置dialog button文字
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(_codec->toUnicode("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(_codec->toUnicode("取消"));


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
        QMessageBox::warning(this,"error",_codec->toUnicode("添加失败，输入地址不能为空"));

        return;
    }
    if(jsonloc=="")
    {
        QMessageBox::warning(this,"error",_codec->toUnicode("添加失败，输出地址不能为空"));

        return;
    }
    //解析文件
    QFile file(fileloc);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        filecontent = in.readAll(); // 将文件内容读取到成员变量中
        file.close();
    }

    //读和写入
    readandwrite();

    //发送信号 让mainwindow重新读入并显示数据库里的数据
    emit back2Course(jsonloc);
}

void Link::clearEdit()
{
    ui->fileloc->clear();
}

void Link::readandwrite()
{
    int index=1;
    //匹配
    QRegularExpression regex("<tr class=\"datagrid-all\">.*?"
                             "<td class=\"datagrid\"><span>(.*?)</span></td>.*?"
                             "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
                             "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
                             "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
                             "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
                             "<td class=\"datagrid\" align=\"center\"><span>(.*?)</span></td>.*?"
                             "<td class=\"datagrid\"><span>(.*?)</span></td>.*?"
                             "<td class=\"datagrid\"><span>(.*?)</span></td>");

    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(filecontent);

    QJsonArray jsonArray;

    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();

        QString courseName = match.captured(1);
        QString courseType = match.captured(2);
        QString credit = match.captured(3);
        QString creditHours = match.captured(4);
        QString teacher = match.captured(5);
        //QString week = match.captured(6);
        QString department = match.captured(7);
        QString extraInfo = match.captured(8);

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
            // 如果 courseDay 不是上述汉字之一，可以根据实际需求进行处理
            // 例如抛出异常、设置默认值等
            // 这里将 dayNumber 设置为 -1 作为无效值
        }

        //

        QJsonArray courseData;
        courseData.append(courseName);
        courseData.append(dayNumber);
        courseData.append(timeBegin);
        courseData.append(timeEnd);
        courseData.append(location);
        courseData.append(teacher);
        courseData.append(examInfo);

        QString key=QString::number(index);
        index++;
        QJsonObject jsonObject;
        jsonObject[key] = courseData;
        jsonArray.append(jsonObject);
    }
    QJsonDocument jsonDoc(jsonArray);

    QFile jsonFile(jsonloc);
    if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        jsonFile.write(jsonDoc.toJson());
        jsonFile.close();
    }
}
