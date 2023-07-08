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

    //处理导入的信号


    //处理修改课程按钮的信号
    connect(&course,&course::changeCourseButtonSignal,this,&MainWindow::initMain);

    //处理删除课程的信号
    connect(&course,&course::delCourseButtonSignal,this,&MainWindow::initMain);

    connect(&link,&Link::back2Course,this,&MainWindow::resetjson);

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
    qDebug("开始构建课程表");
    int row = 11, col = 8;
    ui->courseTable->setRowCount(row);
    ui->courseTable->setColumnCount(col);
    ui->courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->courseTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表头
    QStringList header;
    header << "时间" << "一" << "二" << "三" << "四" << "五" << "六" << "日";
    ui->courseTable->setHorizontalHeaderLabels(header);

    readCourseJson();

}

void MainWindow::on_addCourseButton_clicked() // 添加新课程
{
    // 清空编辑区
    addCourse.clearEdit();

    // 添加新课程时不能做别的事情
    addCourse.exec();
}

void MainWindow::on_fromlinkButton_clicked() //批量导入
{
    link.clearEdit();
    link.exec();
}

void MainWindow::addCourseButton(QString courseName,
                                 int courseDay,
                                 int courseTimeBegin,
                                 int courseTimeEnd,
                                 QString courseLocation,
                                 QString courseTeacher,
                                 QString courseExamLocation,
                                 QString courseExamTime)
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
        this->course.courseExamLocation=courseExamLocation;
        this->course.courseExamTime=courseExamTime;
    });

}

void MainWindow::readCourseJson()
{

    // 读入文件
    QFile file("./course.json");
    QByteArray jsonData;
    if (file.open(QIODevice::ReadOnly)) {
        jsonData = file.readAll();
        file.close();
    }

    qDebug("开始解析文件");

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // 转化文件
    if (jsonDoc.isObject()) {
        QJsonObject obj_root = jsonDoc.object();
        QStringList keys = obj_root.keys();
        for (auto key : keys) {
            QJsonValue arrayTemp = obj_root.value(key);
            QJsonArray courseArray = arrayTemp.toArray();

            // 添加课程信息

            // 课程id
            QString courseName=courseArray.at(0).toString();

            int courseDay=courseArray.at(1).toInt();
            int courseTimeBegin=courseArray.at(2).toInt();
            int courseTimeEnd=courseArray.at(3).toInt();
            QString courseLocation=courseArray.at(4).toString();
            QString courseTeacher=courseArray.at(5).toString();

            QString courseExamLocation=courseArray.at(6).toString();
            QString courseExamTime=courseArray.at(7).toString();

            this->courseMap.insert(key.toInt(), courseName);

            addCourseButton(courseName,
                            courseDay,
                            courseTimeBegin,
                            courseTimeEnd,
                            courseLocation,
                            courseTeacher,
                            courseExamLocation,
                            courseExamTime);
        }
    }
}
