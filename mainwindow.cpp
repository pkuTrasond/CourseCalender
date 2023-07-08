#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QMainWindow>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>
#include <QMap>
#include <QHeaderView>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QFile>
#include <QJsonArray>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _codec = QTextCodec::codecForName("GBK");

    setWindowTitle("CourseTable");

    ui->tabWidget->setTabText(0, "课程表");

    // 设置课程表
    ui->tabWidget->setCurrentIndex(0);

    initCourseTable();

    //来自addcourse创建完成课程，需要在mainwindow里添加课程表和入口
    connect(&addCourse,&AddCourse::courseButtonSignal,this,&MainWindow::addCourseButton);

    //处理修改课程按钮的信号
    connect(&course,&course::changeCourseButtonSignal,this,&MainWindow::initMain);

    //处理删除课程的信号
    connect(&course,&course::delCourseButtonSignal,this,&MainWindow::initMain);

    // 处理修改课程信号
    connect(&course,&course::changeCourseTableSignal,this,&MainWindow::changeCourseWrite);

    // 处理添加课程信号
    connect(&addCourse,&AddCourse::addCourseTableSignal,this,&MainWindow::addCourseWrite);

    // 处理删除课程信号
    connect(&course,&course::deleteCourseTableSignal,this,&MainWindow::deleteCourseWrite);

    // 处理返回信号
    connect(&course, &course::back2Main,
            [=]
            {
                course.hide();
                this->show();
            });

    connect(&link,&Link::back2Course,this,&MainWindow::resetjson);


    this->show();

}

void MainWindow::initMain()
{
    this->ui->tabWidget->setCurrentIndex(0);
    initCourseTable();
    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addCourseWrite(QString courseName,
                           int courseDay,
                           int courseTimeBegin,
                           int courseTimeEnd,
                           QString courseLocation,
                           QString courseTeacher,
                           QString courseExamInfo)
{
    int course_id = this->courseMap.size()+1;
    qDebug() << "添加课程" << course_id;
    this->courseMap.insert(course_id, courseName);
    writeCourseJson(course_id, courseName, courseDay, courseTimeBegin, courseTimeEnd, courseLocation, courseTeacher, courseExamInfo);
}

void MainWindow::changeCourseWrite(int course_id,
                                   QString courseName,
                                   int courseDay,
                                   int courseTimeBegin,
                                   int courseTimeEnd,
                                   QString courseLocation,
                                   QString courseTeacher,
                                   QString courseExamInfo)
{
    writeCourseJson(course_id, courseName, courseDay, courseTimeBegin, courseTimeEnd, courseLocation, courseTeacher, courseExamInfo);
}

void MainWindow::deleteCourseWrite(int course_id)
{
    // 读入文件
    QFile file(jsonloc);
    QByteArray jsonData;
    if (file.open(QIODevice::ReadOnly)) {
        qDebug() << "成功打开文件，从已有课表创建！";
        jsonData = file.readAll();
        file.close();
    } else {
        qDebug() << "打开文件错误，将创建新表格！";
        return;
    }

    qDebug() << "删除文件内容" << course_id;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // 已有文件，替换原有的课程id
    if (jsonDoc.isObject()) {

        file.open(QIODevice::ReadWrite | QIODevice::Truncate);

        QJsonObject obj_root = jsonDoc.object();
        QJsonValue arrayTemp = obj_root.value("Course");
        QJsonArray courseArray = arrayTemp.toArray();

        bool found = false;
        for (int i = 0; i < courseArray.count(); i++) {
            if (courseArray.at(i).toObject().value("id").toInt()  == course_id) {
                found = true;
                courseArray.removeAt(i);
            }
        }

        if (found == false) {
            qDebug() << "无法发现id，错误！";
            obj_root.insert("Course", QJsonValue(courseArray));

            QJsonDocument newJsonDoc(obj_root);
            QByteArray newJsonData = newJsonDoc.toJson();

            file.write(newJsonData);
            file.close();
            return;
        }

        obj_root.insert("Course", QJsonValue(courseArray));

        QJsonDocument newJsonDoc(obj_root);
        QByteArray newJsonData = newJsonDoc.toJson();

        file.write(newJsonData);
        file.close();

    } else {
        qDebug() << "格式错误！";
        return;
    }
}

void MainWindow::initCourseTable()
{
    // 初始化表格
    ui->courseTable = new QTableWidget(ui->tab);
    ui->courseTable->setGeometry(10, 0, 1281, 675);

    // 设置行数、列数
    qDebug() << "开始构建课程表";
    int row = 11, col = 7;
    ui->courseTable->setRowCount(row);
    ui->courseTable->setColumnCount(col);
    ui->courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->courseTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表头
    QStringList header;
    header << "一" << "二" << "三" << "四" << "五" << "六" << "日";
    ui->courseTable->setHorizontalHeaderLabels(header);

    qDebug() << jsonloc;
    if(jsonloc=="")
    {
        jsonloc=QApplication::applicationDirPath() + "/course.json";
    }
    if(!set_json_flag)
        set_json_flag=true;
    else
    {
        readCourseJson();
        addCourse.courseFile=jsonloc;
    }

}

void MainWindow::on_addCourseButton_clicked() // 添加新课程
{
    qDebug() << "进入添加课程窗口";
    // 清空编辑区
    addCourse.clearEdit();
    addCourse.course_id=id_count;

    id_count++;
    // 添加新课程时不能做别的事情
    addCourse.exec();
}

void MainWindow::on_fromlinkButton_clicked() //批量导入
{
    // 由于link相当于重写，所有不用传参数
    qDebug() << "进入批量导入窗口";
    link.clearEdit();
    link.exec();
}

void MainWindow::addCourseButton(QString courseName,
                                 int courseDay,
                                 int courseTimeBegin,
                                 int courseTimeEnd,
                                 QString courseLocation,
                                 QString courseTeacher,
                                 QString courseExamInfo)
{
    qDebug() << "开始添加课程";
    set_json_flag=true;
    if (courseTimeEnd > courseTimeBegin) {
        // 合并单元格
        ui->courseTable->setSpan(courseTimeBegin-1, courseDay, (courseTimeEnd-courseTimeBegin+1), 1);
    }

    // 添加按钮至表格
    courseButton=new QPushButton(this);
    courseButton->setText(QString("%1\n(%2)").arg(courseName).arg(courseLocation));
    ui->courseTable->setCellWidget(courseTimeBegin-1, courseDay, courseButton);

    int course_id = courseMap.size()+1;
    // 设置course_id为目前courseMap的大小
    for (auto iter = courseMap.begin(); iter !=  courseMap.end(); ++iter) {
        if (iter.value() == courseName) {
            course_id = iter.key();
            break;
        }
    }


    // 每一个添加一个触发
    connect(courseButton, &QPushButton::clicked,
            [=]()
    {
        this->hide();
        this->course.courseName=courseName;
        this->course.courseDay=courseDay;
        this->course.courseTimeBegin=courseTimeBegin;
        this->course.courseTimeEnd=courseTimeEnd;
        this->course.courseLocation=courseLocation;
        this->course.courseTeacher=courseTeacher;
        this->course.courseExamInfo=courseExamInfo;
        this->course.run(course_id);
        qDebug()<<"选择课程id:"<<course_id;
    });
}

void MainWindow::readCourseJson()
{

    // 读入文件
    qDebug("read");
    QFile file(jsonloc);
    QByteArray jsonData;
    if (file.open(QIODevice::ReadOnly)) {
        qDebug() << "成功打开文件，从已有课表创建！";
        jsonData = file.readAll();
        file.close();
    } else {
        qDebug() << "打开文件错误，将创建新表格！";
        return;
    }

    qDebug("开始解析文件");

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // 转化文件
    if (jsonDoc.isObject()) {
        qDebug() << "打开格式正确，读入课程";
        QJsonObject obj_root = jsonDoc.object();

        if (obj_root.contains("Course")) {
            QJsonValue arrayTemp = obj_root.value("Course");
            QJsonArray courseArray = arrayTemp.toArray();

            for (int i = 0; i < courseArray.count(); i++) {

                QJsonObject courseObj = courseArray.at(i).toObject();

                int course_id = courseObj.value("id").toInt();
                QString courseName = courseObj.value("name").toString();
                int courseDay = courseObj.value("course_day").toInt();
                int courseTimeBegin = courseObj.value("course_time").toInt();
                int courseTimeEnd = courseObj.value("course_end_time").toInt();
                QString courseLocation = courseObj.value("location").toString();
                QString courseTeacher = courseObj.value("teacher").toString();
                QString courseExamInfo = courseObj.value("exam_info").toString();

                this->courseMap.insert(course_id, courseName);

                addCourseButton(courseName,
                                courseDay,
                                courseTimeBegin,
                                courseTimeEnd,
                                courseLocation,
                                courseTeacher,
                                courseExamInfo);
            }
        } else {
            qDebug() << "json不含有Course内容";
        }
    } else {
        qDebug() << "不是正确json格式";
        return;
    }
}

void MainWindow::writeCourseJson(int course_id,
                                 QString courseName,
                                 int courseDay,
                                 int courseTimeBegin,
                                 int courseTimeEnd,
                                 QString courseLocation,
                                 QString courseTeacher,
                                 QString courseExamInfo)
{

    qDebug()<<course_id;
    // 读入文件
    QFile file(jsonloc);
    QByteArray jsonData;

    if (file.open(QIODevice::ReadWrite)) {
        qDebug() << "打开文件成功，开始写入文件";
        jsonData = file.readAll();
        file.close();
    } else {
        qDebug() << "错误: 无法打开文件";
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // 已有文件，替换原有的课程id
    if (jsonDoc.isObject()) {

        file.open(QIODevice::ReadWrite | QIODevice::Truncate);

        QJsonObject obj_root = jsonDoc.object();
        QJsonValue arrayTemp = obj_root.value("Course");
        QJsonArray courseArray = arrayTemp.toArray();

        QJsonObject course;
        course.insert("id", QJsonValue(course_id));
        course.insert("name", QJsonValue(courseName));
        course.insert("course_day", QJsonValue(courseDay));
        course.insert("course_time", QJsonValue(courseTimeBegin));
        course.insert("course_end_time", QJsonValue(courseTimeEnd));
        course.insert("location", QJsonValue(courseLocation));
        course.insert("teacher", QJsonValue(courseTeacher));
        course.insert("exam_info", QJsonValue(courseExamInfo));

        bool found = false;
        for (int i = 0; i < courseArray.count(); i++) {
            if (courseArray.at(i).toObject().value("id") == course_id) {
                found = true;
                courseArray.removeAt(i);
                courseArray.append(QJsonValue(course));
                break;
            }
        }

        if (found == false) {
            courseArray.append(QJsonValue(course));
        }

        obj_root.insert("Course", QJsonValue(courseArray));

        QJsonDocument newJsonDoc(obj_root);
        QByteArray newJsonData = newJsonDoc.toJson();

        file.write(newJsonData);
        file.close();

    } else { // 格式不正确

        file.open(QIODevice::ReadWrite | QIODevice::Truncate);

        QJsonObject obj_root;
        QJsonArray courseArray;
        QJsonObject course;
        course.insert("id", QJsonValue(course_id));
        course.insert("name", QJsonValue(courseName));
        course.insert("course_day", QJsonValue(courseDay));
        course.insert("course_time", QJsonValue(courseTimeBegin));
        course.insert("course_end_time", QJsonValue(courseTimeEnd));
        course.insert("location", QJsonValue(courseLocation));
        course.insert("teacher", QJsonValue(courseTeacher));
        course.insert("exam_info", QJsonValue(courseExamInfo));

        courseArray.append(QJsonValue(course));
        obj_root.insert("Course", courseArray);

        QJsonDocument newJsonDoc(obj_root);
        QByteArray newJsonData = newJsonDoc.toJson();

        file.write(newJsonData);
        file.close();

        courseMap.insert(course_id,courseName);

    }
}


void MainWindow::on_load_clicked()
{
    jsonloc=ui->jsoninput->text();
    if(jsonloc=="")
    {
        jsonloc=QApplication::applicationDirPath() + "/course.json";
    }
    set_json_flag=true;
    readCourseJson();
    addCourse.courseFile=jsonloc;
}

