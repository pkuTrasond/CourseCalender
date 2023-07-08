#ifndef DDL_H
#define DDL_H

#include <QWidget>
#include <QList>
#include <QDateTime>
#include <QLineEdit>
#include <QTextCodec>

namespace Ui {
class Ddl;
}

struct ddl
{
    QString ddlContent;
    QString ddlRequirement;
    QString ddlDay;
    int ddlTime;
    int ddlStatus;
    int ddl_id;
    int course_id;
};

class Ddl : public QWidget
{
    Q_OBJECT

public:
    explicit Ddl(QWidget *parent = nullptr);
    ~Ddl();

    void goback();

    // ddl 信息
    int course_id;
    QString courseName;
    QList<ddl> ddlList;

    // 显示课程对应ddl
    void run(int course_id);

    // coursemap
    QMap<int, QString> courseMap;

    // 初始化ddlTable
    void initDdlTable();

    // 添加ddl
    void addDdl();

    // 删除ddl
    void delDdl();

    // 保存ddl
    void saveDdl();

    // 判断时间合法性
    bool isValidTime(const QString ddlDay, const int ddlTime);

    // 比较时间
    bool compareTime(QString, QString);

signals:
    void back2Course();

private:
    Ui::Ddl *ui;
    QTextCodec *_codec;

};

#endif // DDL_H
