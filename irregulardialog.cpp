#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QBitmap>

#include "irregulardialog.h"
#include "ui_irregulardialog.h"

IrregularDialog::IrregularDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IrregularDialog)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint); //仅仅隐藏最大最小化和关闭按钮
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);  //设置透明窗体

    pixw = new QPixmap();  //ui设计那里的背景是通过右击 changeStylesheet 添加的resource背景,只是为了用来做设计参考,而这里是必不可少的
    pixw->load(":/png/linuxlogo");
    this->resize(pixw->size());

    QBitmap bitmap;  //设置遮罩,最初以为这个的作用是让透明部分没有窗体,也就是点击透明部分窗体不响应,但我太年轻,不知道是没用对还是他不是这个作用
    bitmap.load(":/png/linuxlogo");
    this->setMask(bitmap.mask());
}

IrregularDialog::~IrregularDialog()
{
    delete ui;
}

void IrregularDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, *pixw);
}

void IrregularDialog::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
        relativePos = this->pos() - event->globalPos();
}

void IrregularDialog::mouseMoveEvent(QMouseEvent * event)
{
    this->move(event->globalPos() + relativePos);
}
