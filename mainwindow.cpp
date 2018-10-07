#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtXml>
#include <qprogressdialog.h>
#include <dialog.h>
#include <qdatetime.h>

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

    /***************************************************** 多选下拉框 *******************************************************/
    //---------列表形式
    listwidget = new QListWidget(this);

    for (int i = 0; i < 5; ++i)
    {
        QListWidgetItem *pItem = new QListWidgetItem(listwidget);
        //pListWidget->addItem(pItem);
        //pItem->setData(Qt::UserRole, i);
        QCheckBox *pCheckBox = new QCheckBox(this);
        pCheckBox->setText(QStringLiteral("Qter%1").arg(i));
        //pListWidget->addItem(pItem);
        pCheckBox->setStyleSheet(QString("QCheckBox {background-color:white;}"
            "QCheckBox:checked{background-color:rgb(255,0,255,255);}"));
        listwidget->setItemWidget(pItem, pCheckBox);
        connect(pCheckBox, SIGNAL(stateChanged(int)), this, SLOT(combobox_list_solt(int)));
    }

    QLineEdit *lineedit;  //设置未点下拉框时的显示内容
    lineedit = new QLineEdit(this);
    lineedit->setReadOnly(true);

    ui->comboBox->setModel(listwidget->model());
    ui->comboBox->setView(listwidget);  //将listwidget添加到combobox中
    ui->comboBox->setLineEdit(lineedit);  //设置未点下拉框时显示的内容
    lineedit->setText("多选(列表)"); //注意这句要在ui->comboBox->setLineEdit(lineedit);之后加
    ui->comboBox->setEditable(true);
    ui->comboBox->setMaxVisibleItems(3);  //设置下拉框最大支持条数

    setStyleSheet("QComboBox{border:1px solid gray;}"
        "QComboBox QAbstractItemView::item{height:45px;}" //下拉选项高度
        //"QComboBox::down-arrow{image:url(:/combobox/Resources/icon1.jpg);}" //下拉箭头
        "QComboBox::drop-down{border:0px;}"); //下拉按钮
    //---------表格形式
    QTableWidget *tablewidget;
    tablewidget = new QTableWidget(3,2,this); //3行2列  注意此处的行列需要与下边两个for循环吻合

    for(int i = 0; i < 3; i++){  //设置行
        tablewidget->setColumnWidth(i, 49);
        tablewidget->setRowHeight(i, 50);
        for(int j = 0; j < 2; j++){
            QCheckBox *checbox = new QCheckBox(this);
            tablewidget->setCellWidget(i, j, checbox);
            checbox->setFixedSize(50, 50);
            checbox->setStyleSheet(QString("QCheckBox {background-color:lightgray;}"
                "QCheckBox:checked{background-color:white;}"));
            checbox->setText(QStringLiteral("好%1").arg(i));
            checbox->setCheckable(true);
            connect(checbox, &QCheckBox::stateChanged, this, [this,i,j]{   //此处的connect函数使用lambda表达式
                QObject *sender = this->sender();  //QObject::sender()返回发送信号的对象的指针，返回类型为QObject *
                if(QCheckBox *checbox_dy = dynamic_cast<QCheckBox *>(sender)){  //dynamic_cast是函数模板,返回指定类型的对象,该对象必须继承自 QObject
                    if(checbox_dy->isChecked()){  //如果被check到
                        qDebug() << "You clicked " << i << "行 " << j << "列" << endl;
                    }
                }
            });
        }
    }

    QLineEdit *lineedit_2;  //设置未点下拉框时的显示内容
    lineedit_2 = new QLineEdit(this);
    lineedit_2->setReadOnly(true);

    ui->comboBox_2->setModel(tablewidget->model());  //以下设置类似列表形式
    ui->comboBox_2->setView(tablewidget);
    ui->comboBox_2->setLineEdit(lineedit_2);
    lineedit_2->setText("多选(表格)");
    ui->comboBox_2->setEditable(true);
    ui->comboBox_2->setMaxVisibleItems(2);

    ui->pushButton_4->installEventFilter(this);  //1.函数的参数是一个有事件过滤器的对象,安装之后过滤器仅对调用install的对象有效.
                                                 //2.事件过滤器可以安装到任意 QObject 类型上面，并且可以安装多个。如果要实现全局的事件过滤器，则可以安装到 QApplication 或者 QCoreApplication 上面。
                                                 //3.事件过滤器最好声明成public,不然在类外安装的事件过滤器可能无效
                                                 //4.事件过滤器和被安装过滤器的组件必须在同一线程，否则，过滤器将不起作用。另外，如果在安装过滤器之后，这两个组件到了不同的线程，那么，只有等到二者重新回到同一线程的时候过滤器才会有效。

    /***************************************************** 时间戳 *******************************************************/
    QDateTime currenttime = QDateTime::fromString( "1970-01-01 08:01:01:000", "yyyy-MM-dd hh:mm:ss:zzz" );
    qDebug() << "current datetime:" << currenttime;
    qDebug() << "current datetime stamp:" << currenttime.toTime_t(); //将时间转化成时间戳,方法为从时间基点到设定时间的秒数
    qDebug() << "datetime stamp base on:" << currenttime.fromTime_t(0); //转化基点: 1970-01-01 08:01:01:000
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->pushButton_4){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() == Qt::LeftButton) qDebug() << "This is eventFilter  You Press LeftButton" << endl;
            if(mouseEvent->button() == Qt::RightButton) qDebug() << "This is eventFilter  You Press RightButton" << endl;
            return true; //return true表示接受事件,return false表示不接收事件,会继续传递处理
        }else{
            return false;
        }
    }else{
        return QMainWindow::eventFilter(watched,event);
    }
}

bool MainWindow::event(QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress){
        QMouseEvent *mouseevent = static_cast<QMouseEvent *>(event);
        if(mouseevent->button() == Qt::LeftButton){
            qDebug() << "This is event()  You Clicked mouset leftbutton" << endl;
            return true;
        }
        return false;  //这个地方我个人理解算是接收了事件,现象是如果在此处返回false,需要右击两次才能触发下面的mousePressEvent事件函数
    }
    return QMainWindow::event(event);  //注意:这句很重要
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton || event->button() == Qt::RightButton){
        if(event->modifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton) qDebug() << "You Clicked Left Button and Control" << endl; //鼠标键盘组合,方法一
        //if(QApplication::keyboardModifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton) qDebug() << "You Clicked Left Button and Control" << endl; //鼠标键盘组合,方法二
        if(event->button() == Qt::LeftButton) qDebug() << "You Clicked Left Button" << endl;
        if(event->button() == Qt::RightButton) qDebug() << "You Clicked Right Button" << endl;
    }else{
        QMainWindow::mousePressEvent(event);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A)  //键盘组合按键
        qDebug() << "You Press Ctrl and A" << endl;
    if(event->key() == Qt::Key_Control){
        qDebug() << "You Press Ctrl" << endl;
    }else{
        QMainWindow::keyPressEvent(event);
    }
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
    /***************************************************** 继续 进度条 Bar 方式 **********************************************/
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

void MainWindow::on_pushButton_4_clicked()
{
    qDebug() << "You Clicked 事件过滤测试" << endl;
}

void MainWindow::combobox_list_solt(int state)
{
    int nCount = listwidget->count();
    for (int i = 0; i < nCount; i++){
        QListWidgetItem *pItem = listwidget->item(i);  //以下三句是为了得到被选中的第i个复选框checbox
        QWidget *pWidget = listwidget->itemWidget(pItem);
        QCheckBox *pCheckBox = (QCheckBox *)pWidget;
        if (pCheckBox->isChecked()){
            qDebug() << "You clicked " << i << " 行" << endl;
        }
    }
}
