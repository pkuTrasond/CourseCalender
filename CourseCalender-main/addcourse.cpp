#include "addcourse.h"
#include "ui_addcourse.h"

#include <QMessageBox>
#include <QPushButton>
#include <QVariantList>
#include <QFile>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonArray>

AddCourse::AddCourse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCourse)
{
    ui->setupUi(this);
    _codec = QTextCodec::codecForName("GBK");

    // 窗口标题
    setWindowTitle("添加课程");

    // 设置dialog button文字
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");

}

AddCourse::~AddCourse()
{
    delete ui;
}

void AddCourse::on_buttonBox_accepted()
{

    // 获取编辑区内容
    QString courseName=ui->courseNameEdit->text();

    int courseDay=ui->courseDayEdit->currentIndex()+1;
    int courseTimeBegin=ui->courseDayEdit->currentIndex()+1;
    int courseTimeEnd=ui->courseTimeEditBegin->currentIndex()+1;
    QString courseLocation=ui->courseLocationEdit->text();
    QString courseTeacher=ui->courseTeacherEdit->text();

    // QString courseExamLocation=ui->courseExamLocationEdit->text();
    // QString courseExamTime=ui->courseExamTime->text();
    QString courseExamInfo=ui->courseExamInfoEdit->text();

    //判断课程名是否非空
    if(courseName=="")
    {
        QMessageBox::warning(this,"error", "添加失败，课程名不能为空");
        //isValid=false;
        //continue;
        return;
    }

    //判断课程节数是否合法
    if(courseTimeBegin>courseTimeEnd)
    {
        QMessageBox::warning(this,"error","添加失败，输入的课程节数不合法");
        return;
    }

    //判断课程时间冲突
    if(conflict(courseDay,courseTimeBegin,courseTimeEnd))
    {
        QMessageBox::warning(this,"error","添加失败，新添加的课程与已有课程时间冲突");
        return;
    }

    // 写入文件逻辑
    QFile file( courseFile );
    QByteArray jsonData;

    if (file.open(QIODevice::ReadWrite)) {
        jsonData = file.readAll();
    } else {
        qDebug() << "file open error!" << endl;
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isObject()) {

        QJsonObject obj_root = jsonDoc.object();

        if (obj_root.contains("Course")) {
            QJsonValue arrayTemp = obj_root.value("Course");
            QJsonArray courseArray = arrayTemp.toArray();

            QJsonObject course;
            course.insert("id", QJsonValue(course_id));
            course.insert("name", QJsonValue(courseName));
            course.insert("course_day", QJsonValue(courseDay));
            course.insert("course_time", QJsonValue(courseTimeBegin));
            course.insert("course_end_time", QJsonValue(courseTimeEnd));
            course.insert("location", QJsonValue(courseLocation));
            course.insert("teacher", QJsonValue(courseTeacher));
            course.insert("exam_info", QJsonValue(courseExamInfo));

            courseArray.append(QJsonValue(course));

            obj_root.insert("Course", QJsonValue(courseArray));

            QJsonDocument newJsonDoc(obj_root);
            QByteArray newJsonData = newJsonDoc.toJson();

            file.write(newJsonData);
            file.close();

            courseMap.insert(course_id,courseName);
        }
    }


    // ...

    emit courseButtonSignal(courseName, courseDay,courseTimeBegin, courseTimeEnd, courseLocation,courseTeacher,courseExamInfo);

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
    return false;
}
