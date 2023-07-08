#include "addcourse.h"
#include "ui_addcourse.h"

#include <QMessageBox>
#include <QPushButton>
#include <QVariantList>

AddCourse::AddCourse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCourse)
{
    ui->setupUi(this);
    _codec = QTextCodec::codecForName("GBK");

    // 窗口标题
    setWindowTitle(_codec->toUnicode("添加课程"));

    // 设置dialog button文字
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(_codec->toUnicode("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(_codec->toUnicode("取消"));

}

AddCourse::~AddCourse()
{
    delete ui;
}

void AddCourse::on_buttonBox_accepted()
{
    // 文件校验

    // 获取编辑区内容
    QString courseName=ui->courseNameEdit->text();

    int courseDay=ui->courseDayEdit->currentIndex()+1;
    int courseTimeBegin=ui->courseDayEdit->currentIndex()+1;
    int courseTimeEnd=ui->courseTimeEditBegin->currentIndex()+1;
    QString courseLocation=ui->courseLocationEdit->text();
    QString courseTeacher=ui->courseTeacherEdit->text();

    // QString courseExamLocation=ui->courseExamLocationEdit->text();
    // QString courseExamTime=ui->courseExamTime->text();

    //判断课程名是否非空
    if(courseName=="")
    {
        QMessageBox::warning(this,"error",_codec->toUnicode("添加失败，课程名不能为空"));
        //isValid=false;
        //continue;
        return;
    }

    //判断课程节数是否合法
    if(courseTimeBegin>courseTimeEnd)
    {
        QMessageBox::warning(this,"error",_codec->toUnicode("添加失败，输入的课程节数不合法"));
        return;
    }

    //判断课程时间冲突
    if(conflict(courseDay,courseTimeBegin,courseTimeEnd))
    {
        QMessageBox::warning(this,"error",_codec->toUnicode("添加失败，新添加的课程与已有课程时间冲突"));
        return;
    }

    // 写入文件逻辑
    // ...

    // emit courseButtonSignal(...)

}

void AddCourse::clearEdit()
{
    ui->courseNameEdit->clear();
    ui->courseDayEdit->setCurrentIndex(0);
    ui->courseTimeEditBegin->setCurrentIndex(0);
    ui->courseTimeEditEnd->setCurrentIndex(0);
    ui->courseLocationEdit->clear();
    ui->courseTeacherEdit->clear();
}

bool AddCourse::conflict(int courseDay, int courseTimeBegin, int courseTimeEnd)
{
    // 冲突逻辑
    // ...
}
