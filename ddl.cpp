#include "ddl.h"
#include "mainwindow.h"
#include "ui_ddl.h"

#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <QMap>

Ddl::Ddl(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Ddl)
{
    ui->setupUi(this);

    //父窗口指针
    courseMap = ((MainWindow*)parent)->courseMap;

    _codec = QTextCodec::codecForName("GBK");

    // 去除焦点
    ui->ddlTable->setFocusPolicy(Qt::NoFocus);

    //发送返回课程页面信号
    connect(ui->returnButton, &QPushButton::clicked, this, &Ddl::goback);

    //处理添加ddl的信号
    connect(ui->addDdlButton,&QPushButton::clicked,this,&Ddl::addDdl);

    //处理删除ddl的信号
    connect(ui->delDdlButton,&QPushButton::clicked,this,&Ddl::delDdl);

    //处理保存ddl的信号
    connect(ui->saveDdlButton, &QPushButton::clicked,this,&Ddl::saveDdl);

}

Ddl::~Ddl()
{
    delete ui;
}

void Ddl::goback()
{
    emit this->back2Course();
}

void Ddl::run(int course_id)
{
    // 传递course_id成员
    this->course_id = course_id;

    // 加载信息
    initDdlTable();

}

void Ddl::initDdlTable()
{
    // 清空ddl列表
    ddlList.clear();

    // 获取对应的course_id的coursename
    courseName = courseMap.toStdMap().at(course_id);

    // 设置标题
    ui->titleLabel->setText(courseName);

    this->show();

    // 设置行数、列数
    int row = 0, col = 5;
    ui->ddlTable->setRowCount(row);
    ui->ddlTable->setColumnCount(col);

    // 设置表头
    QStringList header;
    // 表头写入数据
    header << "ddl(必填)" << "要求(必填)" << "截止时间(必填)" << "状态";
    ui->ddlTable->setHorizontalHeaderLabels(header);

    // 表格列宽
    ui->ddlTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->ddlTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->ddlTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->ddlTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // 获取课程号为course_id的所有ddl信息



}

void Ddl::addDdl()
{

}

void Ddl::delDdl()
{

}

void Ddl::saveDdl()
{

}
