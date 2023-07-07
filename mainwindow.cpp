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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _codec = QTextCodec::codecForName("GBK");

    setWindowTitle("CourseTable");

    ui->tabWidget->setTabText(0, _codec->toUnicode("课程表"));

    initMain();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void initMain()
{

}

void MainWindow::initCourseTable()
{
    // 初始化表格
    ui->courseTable = new QTableWidget(ui->tab);
    ui->courseTable->setGeometry(10, 0, 1281, 675);

    // 设置行数、列数
    int row = 11, col = 8;
    ui->courseTable->setRowCount(row);
    ui->courseTable->setColumnCount(col);
    ui->courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->courseTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表头
    QStringList header;
    header << _codec->toUnicode("时间")
           << _codec->toUnicode("一")
           << _codec->toUnicode("二")
           << _codec->toUnicode("三")
           << _codec->toUnicode("四")
           << _codec->toUnicode("五")
           << _codec->toUnicode("六")
           << _codec->toUnicode("日");
    ui->courseTable->setHorizontalHeaderLabels(header);

    // 读入文件，写入table
    /**
     *
     *
     **/

    // 创建课程按钮
    QString courseName;
    int courseDay;
    int courseTimeBegin;
    int courseTimeEnd;
    QString courseLocation;
    QString courseTeacher;
    QString courseExamLocation;
    QString courseExamTime;

    while() {

        // 获取信息

        // 记录
        this->courseMap.insert();

        // 添加课程按钮
        addCourseButton(coursename ....)
    }

}

void MainWindow::on_addCourseButton_clicked() // 添加新课程
{
    // 清空编辑区
    addCourse.clearEdit();

    // 添加新课程时不能做别的事情
    addCourse.exec();
}
