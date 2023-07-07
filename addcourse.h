#ifndef ADDCOURSE_H
#define ADDCOURSE_H

#include <QDialog>
#include <QTextCodec>

namespace Ui {
class AddCourse;
}

class AddCourse : public QDialog
{
    Q_OBJECT

public:
    explicit AddCourse(QWidget *parent = nullptr);
    ~AddCourse();

    // 清空编辑区
    void clearEdit();

    // 判断课程冲突
    bool conflict(...);

signals:
    // 课程按钮
    void courseButtonSignal(QString courseName);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddCourse *ui;
    QTextCodec *_codec;

};

#endif // ADDCOURSE_H
