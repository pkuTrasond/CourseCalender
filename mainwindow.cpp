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

    QTextCodec *_codec = QTextCodec::codecForName("GBK");

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
