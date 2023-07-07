#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include "addcourse.h"
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

    QMap<int, QString> courseMap;

    // 添加新课程的界面
    AddCourse addCourse;

    Ui::MainWindow *ui;

private:
    QPushButton *courseButton;
    QTextCodec *_codec;
};
#endif // MAINWINDOW_H
