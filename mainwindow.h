#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include "addcourse.h"
#include "course.h"
#include <QMainWindow>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 课程详细信息
    course course;

    QMap<int, QString> courseMap;

    // 添加新课程的界面
    AddCourse addCourse;

    void initMain();

    Ui::MainWindow *ui;

    // 初始化课程列表
    void initCourseTable();


private:
    QPushButton *courseButton;
    QTextCodec *_codec;
};
#endif // MAINWINDOW_H
