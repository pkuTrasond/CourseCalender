#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include "addcourse.h"
#include "course.h"
#include "link.h"
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

    Link link;

    QString jsonloc;

    void resetjson(QString jloc)
    {
        this->jsonloc=jloc;
        initCourseTable();
    }

    // 添加新课程的界面
    AddCourse addCourse;

    void initMain();

    Ui::MainWindow *ui;

    // 初始化课程列表
    void initCourseTable();

    // 读取Json
    void readCourseJson(QString courseFile);

    // 写入Json
    void writeCourseJson(QString courseFile,
                         QString courseName,
                         int courseDay,
                         int courseTimeBegin,
                         int courseTimeEnd,
                         QString courseLocation,
                         QString courseTeacher,
                         QString courseExamInfo);

    // 添加课程
    void addCourseButton(QString courseName,
                         int courseDay,
                         int courseTimeBegin,
                         int courseTimeEnd,
                         QString courseLocation,
                         QString courseTeacher,
                         QString courseExamInfo);

private:
    QPushButton *courseButton;
    QTextCodec *_codec;

private slots:
    // 添加新课程
    void on_addCourseButton_clicked();

    // 从文件导入
    void on_fromlinkButton_clicked();

};
#endif // MAINWINDOW_H
