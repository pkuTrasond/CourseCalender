#include "course.h"
#include "ui_course.h"

#include <QMessageBox>>

course::course(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::course)
{
    ui->setupUi(this);
    _codec = QTextCodec::codecForName("GBR");

    // 设置窗口标题
    setWindowTitle(_codec->toUnicode("课程信息"));

    //回到课程表页面
    connect(ui->returnButton, &QPushButton::clicked,
            [=]
    {
        sendSlot();
    });

    //处理保存修改
    connect(ui->saveButton,&QPushButton::clicked,this,&course::save);

    //处理删除课程
    connect(ui->delButton,&QPushButton::clicked,this,&course::del);
}

course::~course()
{
    delete ui;
}

// 回主页信号
void course::sendSlot()
{
    emit back2Main();
}

void course::initEdit()
{
    //初始化编辑区信息
    ui->courseNameEdit->setText(courseName);
    ui->courseDayEdit->setCurrentIndex(courseDay-1);
    ui->courseTimeBeginEdit->setCurrentIndex(courseTimeBegin-1);
    ui->courseTimeEndEdit->setCurrentIndex(courseTimeEnd-1);
    ui->courseLocationEdit->setText(courseLocation);
    ui->courseTeacherEdit->setText(courseTeacher);
}

void course::run(int courseId)
{
    //传course_id
    course_id=courseId;

    //初始化编辑区信息
    initEdit();

    //展示界面
    this->show();
}

void course::save()
{

    //获取编辑区内容
    courseName=ui->courseNameEdit->text();
    courseDay=ui->courseDayEdit->currentIndex()+1;
    courseTimeBegin=ui->courseTimeBeginEdit->currentIndex()+1;
    courseTimeEnd=ui->courseTimeEndEdit->currentIndex()+1;
    courseLocation=ui->courseLocationEdit->text();
    courseTeacher=ui->courseTeacherEdit->text();

    //判断课程名是否非空
    if(courseName=="")
    {
        return;
    }

    //判断课程节数是否合法
    if(courseTimeBegin>courseTimeEnd)
    {
        return;
    }

    //判断课程时间冲突
    if(conflict(courseDay,courseTimeBegin,courseTimeEnd))
    {
        return;
    }

    // 文件保存

    //保存成功
    QMessageBox::information(this,"success",_codec->toUnicode("保存成功"));

    //向MainWindow发信号，修改课程按钮
    emit changeCourseButtonSignal();

    //qDebug() << "here";
    this->close();
}

void course::del()
{
    int sure=QMessageBox::question(this,"sure",_codec->toUnicode("确定要删除该课程吗？\n(删除课程将同时删除该课程的ddl和链接)"));
    if(sure==QMessageBox::Yes)
    {
        // 文件删除

        //删除成功
        QMessageBox::information(this,"success",_codec->toUnicode("删除成功"));

        //向MainWindow发信号，修改课程按钮
        this->close();
        emit delCourseButtonSignal();
    }
}

bool course::conflict(int courseDay, int courseTimeBegin, int courseTimeEnd)
{
    // ...
}
