#ifndef COURSE_H
#define COURSE_H

#include <QWidget>
#include <QTextCodec>

namespace Ui{
class course;
}

class course : public QWidget
{
    Q_OBJECT
public:
    explicit course(QWidget *parent = nullptr);
    ~course();

    // 基本信息
    int course_id;

    QString courseName;
    int courseDay;
    int courseTimeBegin;
    int courseTimeEnd;
    QString courseLocation;
    QString courseTeacher;

    QString courseExamLocation;
    QString courseExamTime;

    // 初始化编辑区
    void initExit();

    void sendSlot();

    void run(int courseId);

    // 初始化编辑区
    void initEdit();

    // 保存修改
    void save();

    // 删除课程
    void del();

    //判断课程时间冲突
    bool conflict(int courseDay,int courseTimeBegin,int courseTimeEnd);
signals:
    void back2Main();

    //修改课程按钮->MainWindow
    void changeCourseButtonSignal();

    //删除课程按钮->MainWindow
    void delCourseButtonSignal();

private:
    Ui::course *ui;
    QTextCodec *_codec;

};

#endif // COURSE_H
