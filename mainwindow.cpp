#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtXml>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}
