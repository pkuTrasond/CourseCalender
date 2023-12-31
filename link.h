#ifndef LINK_H
#define LINK_H

#include<QDialog>
#include<QDebug>
#include<QMessageBox>
#include<QPushButton>
#include<QVariantList>
#include<QFile>
#include<QMap>
namespace Ui {
class Link;
}


class Link : public QDialog
{
    Q_OBJECT

public:
    explicit Link(QWidget *parent = nullptr);
    ~Link();

    int id_count;
    QString courseName;
    //数据库位置
    QString jsonloc;
    //输入文件
    QString filecontent;
    // QMap<int, QString> courseMap;

    //从html文件
    void readandwrite();

    //清空输入
    void clearEdit();
private slots:
    //读取课程
    void on_buttonBox_accepted();
    void on_buttonBox_rejected()
    {
        close();
    }

signals:
    void back2Course(QString jsonloc);


private:
    Ui::Link *ui;
    QTextCodec *_codec;
};

#endif // LINK_H
