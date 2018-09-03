#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtXml>
#include <qprogressdialog.h>
#include <dialog.h>

#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /***************************************************** xml件操作 **************************************************/
    QDomDocument doc;  //新建QDomDocument类对象，它代表一个XML文档
    QFile file("./my.xml");  // 建立指向“my.xml”文件的QFile对象
    if (!file.open(QIODevice::ReadOnly)) return;   // 以只读方式打开
    if(!doc.setContent(&file)){
        file.close();
        qDebug() << "xml file open fail" << endl;
        return;
    }
    file.close();  //关闭文件
    //-----读xml
    QDomNode firstNode = doc.firstChild(); //获取xml的第一个节点,即xml说明
    qDebug() << firstNode.nodeName() << firstNode.nodeValue() << endl;
    QDomElement docElem = doc.documentElement();  //获取根元素
    qDebug() << "根元素是:" << docElem.tagName() << endl;
    QDomNode n = docElem.firstChild();
    QDomNodeList list = n.childNodes(); //获得根元素下第一个元素的所有子节点的列表
    for(int i = 0; i < list.count(); i++){
        QDomNode node = list.at(i);
        if(node.isElement())
            qDebug() << "根元素下第一个元素的子节点:" << node.toElement().tagName() << endl;
    }
    while(!n.isNull()){  //如果节点不为空   这个while循环打印所有根元素下的兄弟节点
        if(n.isElement()){   //如果节点是元素
            QDomElement e = n.toElement();  //将其转换为元素
            qDebug() << e.tagName() << endl;
            //qDebug() << e.text() << endl; //输出元素的值
            //qDebug() << e.attribute("<属性名称>"); //输出<属性名称>对应的值
        }
        n = n.nextSibling();
    }
    //------写xml
    QFile file1("new.xml");
    file1.open(QIODevice::ReadWrite);
    QDomDocument doc1;
    QDomProcessingInstruction instruction;
    instruction = doc1.createProcessingInstruction("xml","version=\"1.0\" encoding=\"GB2312\"");
    doc1.appendChild(instruction);
    QDomElement root = doc1.createElement("ipconfig");
    doc1.appendChild(root);
    QDomText text = doc1.createTextNode("textnode");
    root.appendChild(text);
    QTextStream out(&file1);
    doc1.save(out,4);
    file1.close();

    /***************************************************** 进度条 Bar 方式 **************************************************/
    ui->progressBar->setRange(0,5000);
    ui->progressBar->setValue(0);

    /************************************************** Plot QCustomPlot 方式 **********************************************/
    ui->widget->setBackground(QBrush(QColor(125,125,125)));
    /***************************************************** Plot Qwt 方式 ***************************************************/


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    /***************************************************** 进度条 dialog方式 **************************************************/
    QProgressDialog process(this);
    process.setWindowModality(Qt::WindowModal);  //设置模态窗口
    process.setMinimumDuration(0); //QProgressDialog有一个智能延迟，默认一切短于4秒的操作都不显示Dialog，所以这里把这个值设为0
    process.setAttribute(Qt::WA_DeleteOnClose, true);  //有时候整个进度条会卡住，啥也不显示(一般是只显示一块白色的面板)然后就什么也动不了了调用close（）函数也关不掉。这时候 就需要progressDlg->setAttribute(Qt::WA_DeleteOnClose, true);这样在调用Close的时候，窗口一定会被 关掉。
    process.setWindowTitle("处理进度");
    process.setLabelText("processing......");
    process.setCancelButtonText("取消");
    process.setRange(0,5000);
    for(int i = 0; i < 5000; i++){
        for(int j = 0; j < 2000; j++){
            process.setValue(i);  //注意此处的值要与 setRange 指令相吻合
            if(process.wasCanceled())
                break;
        }
    }
    process.close();  //去掉这一句好像也没啥影响
}

void MainWindow::on_pushButton_2_clicked()
{
    for(int i = 0; i < 5000; i++){
        for(int j = 0; j < 2000; j++){
            ui->progressBar->setValue(i+1);  //注意此处的值要与 setRange 指令相吻合
        }
    }

    ui->progressBar->setValue(0);


}

void MainWindow::on_pushButton_3_clicked()
{
    dialog = new Dialog(this);
    //dialog->setWindowModality(Qt::WindowModal); //设置对话框是模态还是非模态
    dialog->setModal(true);  //也是设置模态和非模态,跟上一句作用一样,但是效果有细微差别,推荐这种方法
    dialog->setWindowTitle("对话框");
    //dialog->exec();  //只能是模态对话框,跟属性值无关
    dialog->show();  //既可以是模态对话框也可以是非模态对话框,取决于modal属性
}
