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

    this->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initCourseTable()
{
    // 初始化表格
    ui->courseTable = new QTableWidget(ui->tab);
    ui->courseTable->setGeometry(10, 0, 1281, 675);

    // 设置行数、列数
    qDebug() << "开始构建课程表" << endl;
    int row = 11, col = 7;
    ui->courseTable->setRowCount(row);
    ui->courseTable->setColumnCount(col);
    ui->courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->courseTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表头
    QStringList header;
    header << "一" << "二" << "三" << "四" << "五" << "六" << "日";
    ui->courseTable->setHorizontalHeaderLabels(header);

    readCourseJson("course");

}

void MainWindow::on_addCourseButton_clicked() // 添加新课程
{
    qDebug() << "进入添加课程窗口" << endl;
    // 清空编辑区
    addCourse.clearEdit();

    // 添加新课程时不能做别的事情
    addCourse.exec();
}

void MainWindow::addCourseButton(QString courseName,
                                 int courseDay,
                                 int courseTimeBegin,
                                 int courseTimeEnd,
                                 QString courseLocation,
                                 QString courseTeacher,
                                 QString courseExamInfo)
{
    if (courseTimeEnd > courseTimeBegin) {
        // 合并单元格
        ui->courseTable->setSpan(courseTimeBegin-1, courseDay, (courseTimeEnd-courseTimeBegin+1), 1);
    }

    // 添加按钮至表格
    courseButton=new QPushButton(this);
    courseButton->setText(QString("%1\n(%2)").arg(courseName).arg(courseLocation));
    ui->courseTable->setCellWidget(courseTimeBegin-1, courseDay, courseButton);

    // 获取courseButton对应的course_id
    // 文件逻辑


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
    });

}

void MainWindow::readCourseJson(QString courseFile)
{

    // 读入文件
    QFile file("./" + courseFile + ".json");
    QByteArray jsonData;
    if (file.open(QIODevice::ReadOnly)) {
        jsonData = file.readAll();
        file.close();
    } else {
        qDebug() << "打开文件错误，将创建新表格！" << endl;
        return;
    }

    qDebug("开始解析文件");

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // 转化文件
    if (jsonDoc.isObject()) {
        QJsonObject obj_root = jsonDoc.object();

        if (obj_root.contains("Course")) {
            QJsonValue arrayTemp = obj_root.value("Course");
            QJsonArray courseArray = arrayTemp.toArray();

            for (auto iter = courseArray.begin(); iter != courseArray.end(); iter++) {

                QJsonObject courseObj = (*iter).toObject();

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
        }
    }
}

void MainWindow::writeCourseJson(QString courseFile,
                                 QString courseName,
                                 int courseDay,
                                 int courseTimeBegin,
                                 int courseTimeEnd,
                                 QString courseLocation,
                                 QString courseTeacher,
                                 QString courseExamInfo)
{

    // 读入文件
    QFile file("./" + courseFile + ".json");
    QByteArray jsonData;

    if (file.open(QIODevice::ReadWrite)) {
        jsonData = file.readAll();
    } else {
        qDebug() << "file open error!" << endl;
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isObject()) {

        QJsonObject obj_root = jsonDoc.object();

        if (obj_root.contains("Course")) {
            QJsonValue arrayTemp = obj_root.value("Course");
            QJsonArray courseArray = arrayTemp.toArray();

            int course_id;
            for (auto &pair : this->courseMap.toStdMap()) {
                if (pair.second.toStdString() == courseName.toStdString()) {
                    course_id = pair.first;
                    break;
                }
            }

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

            obj_root.insert("Course", QJsonValue(courseArray));

            QJsonDocument newJsonDoc(obj_root);
            QByteArray newJsonData = newJsonDoc.toJson();

            file.write(newJsonData);
            file.close();
        }
    }
}

