#ifndef ADDCOURSE_H
#define ADDCOURSE_H

#include <QDialog>
#include <QTextCodec>
#include <QMap>
namespace Ui {
class AddCourse;
}

class AddCourse : public QDialog
{
    Q_OBJECT

public:
    explicit AddCourse(QWidget *parent = nullptr);
    ~AddCourse();

    //新加的课程的id
    int course_id=1;
    QString courseFile;
    QMap<int, QString> courseMap;
    // 清空编辑区-*-
    void clearEdit();

    // 判断课程冲突
    bool conflict(int courseDay, int courseTimeBegin, int courseTimeEnd);

signals:
    // 课程按钮
    void courseButtonSignal(QString courseName,
                            int courseDay,
                            int courseTimeBegin,
                            int courseTimeEnd,
                            QString courseLocation,
                            QString courseTeacher,
                            QString courseExamInfo);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddCourse *ui;
    QTextCodec *_codec;

};

#endif // ADDCOURSE_H
