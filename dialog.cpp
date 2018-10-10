#include <QStyleFactory>
#include <QDebug>

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //更改窗口风格: 方法一: 使用qstyle 方法二: 使用qss
    //方法一
    //查询当前平台支持的 QStyle
    QStringList styles = QStyleFactory::keys();
    foreach (QString style, styles) {
        qDebug() << style ;
    }
    //设置当前窗口风格
    this->setStyle(QStyleFactory::create("Windows"));
    //如果希望所有的窗口更改默认风格可以在主函数中设置  QApplication::setStyle(QStyleFactory::create("Fusion"))
    //另外可以重写相应的虚函数或者重写一个自己的style类,可以将自定义的style定义成插件供其他人使用

    //方法二: 使用qss
    //1、建立文本文件，写入样式表内容。更改文件后缀名为qss；
    //2、在project中新建资源文件*.qrc，将qss文件增加资源文件qrc中。此处注意prefix最好为"/"。否则在调用qss文件时会找不到文件；
    //3、通过传入路径\文件名称的方式创建一个QFile对象，以readonly的方式打开，然后readAll，最后qApp->setStyleSheet就能够使qss生效。
//    QString qss;
//    QFile qssFile(":/qss/css.qss");
//    qssFile.open(QFile::ReadOnly);
//    if(qssFile.isOpen()){
//        qss = QLatin1String(qssFile.readAll());
//        qApp->setStyleSheet(qss);
//        qssFile.close();
//    }

}

Dialog::~Dialog()
{
    delete ui;
}
