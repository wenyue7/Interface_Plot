#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtXml>
#include <qprogressdialog.h>
#include <dialog.h>
#include <qdatetime.h>

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_legenditem.h>
#include <qwt_legend.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

#include "qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /***************************************************** 窗口结构设置 **************************************************/
    //窗口名字和窗口图标
    setWindowTitle("Plot");
    setWindowIcon(QIcon(":/png/linuxlogo")); //这个不是窗口标题旁边的图标,是任务栏的
    //===============菜单栏=============
    QMenuBar *m_MenuBar = ui->menuBar;  //添加一个菜单栏
    //File菜单
    QMenu *m_MenuFile = new QMenu("&File");  //前边加个 & 是为了保证按住alt键之后操作, & 加在哪个字母前就是使用哪个字母来作为操作
    m_MenuBar->addMenu(m_MenuFile);
    QAction *fileAction = new QAction(QIcon(":/png/linuxlogo"), "Open File"); //qaction可以选择加图标
    m_MenuFile->addAction(fileAction);
    QAction *fileAction_2 = new QAction("Save File");
    m_MenuFile->addAction(fileAction_2);
    //Edit菜单  增加子菜单
    QMenu *m_MenuEdit = new QMenu("Edit");
    QMenu *m_MenuEdit_SubA = new QMenu("SubA");
    QAction *editActionA = new QAction("SubA_action");
    m_MenuEdit_SubA->addAction( editActionA );
    m_MenuEdit_SubA->setIcon(QIcon(":/png/linuxlogo"));  //菜单也可以设置图标
    m_MenuEdit->addMenu(m_MenuEdit_SubA);
    m_MenuBar->addMenu(m_MenuEdit);

    connect(fileAction, &QAction::triggered, this, &MainWindow::menuFileOpen);
    connect(fileAction_2, &QAction::triggered, this, &MainWindow::menuFileSave);

//    QMenuBar *m_MenuBar_2 = new QMenuBar(this); //可以通过这种方法自己创建,但是要先删除ui里的菜单栏,不然无法正常工作,一个主窗口只能有一个菜单栏
//    m_MenuBar_2->addMenu(m_MenuFile);

    //===============工具栏=============
    QToolBar *m_ToolBar = ui->mainToolBar;  //这里同样可以改成自己创建的工具栏
    QAction *m_ToolBarOpen = new QAction(QIcon(":/png/linuxlogo"), "message"); //第一个参数是图标,第二个是提示信息
    m_ToolBar->addAction(m_ToolBarOpen);
    m_ToolBarOpen->setShortcut(tr("ctrl+o"));  //设置快捷键

    connect(m_ToolBarOpen, &QAction::triggered, this, &MainWindow::menuFileOpen);

    //===============状态栏=============
    //说明:状态信息分为三类:临时信息(如一般的提示信息),正常信息(如页数和行号),永久信息(如版本号)
    //ui->statusBar->showMessage("Welcome",2000); //第二个参数是显示事件,可以不设置事件,指输入一个参数
    QLabel *statuelabel = new QLabel();
    statuelabel->setMinimumSize(150,20);  //设置大小
    statuelabel->setFrameShape(QFrame::WinPanel);  //设置标签形状
    statuelabel->setFrameShadow(QFrame::Sunken);  //设置标签阴影
    ui->statusBar->addWidget(statuelabel);
    statuelabel->setText("欢迎");
    //添加永久信息
    QLabel *statuelabel_2 = new QLabel();
    statuelabel_2->setMinimumSize(150,20);  //设置大小
    statuelabel_2->setFrameShape(QFrame::WinPanel);  //设置标签形状
    statuelabel_2->setFrameShadow(QFrame::Sunken);  //设置标签阴影
    ui->statusBar->addPermanentWidget(statuelabel_2);
    statuelabel_2->setText("永久信息");
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
    ui->widget->setInteractions(QCP::iRangeDrag |  //拖拽
                               QCP::iRangeZoom |   //缩放,可以对每个坐标轴进行缩放
                               QCP::iSelectAxes |  //可选择坐标轴
                               QCP::iSelectLegend |  //应该跟图例有关,但不知道有啥用
                               QCP::iSelectPlottables);
    ui->widget->axisRect()->setupFullAxesBox();  //设置四条坐标轴
    ui->widget->setBackground(QBrush(QColor(125,125,125)));  //设置背景颜色
    ui->widget->legend->setVisible(true);    //设置图例
    ui->widget->legend->setFont(QFont("Helvetica", 9));
    ui->widget->xAxis->setLabel("xAxis");  //设置坐标轴
    //ui->widget->xAxis->setTicker(false);
    ui->widget->xAxis2->setLabel("xAxis2");
    //ui->widget->xAxis2->setTicker(false);
    ui->widget->yAxis->setLabel("yAxis");
    ui->widget->yAxis2->setLabel("yAxis2");

    QVector<double> x(101),y(101);
    for(int i=0;i<101;i++)
    {
        x[i] = i/5.0-10;
        y[i] = x[i]*x[i]*x[i];
    }
    ui->widget->addGraph();  //添加一条曲线
    ui->widget->graph(0)->setPen(QPen(Qt::yellow));
    ui->widget->graph(0)->setName("曲线1");
    ui->widget->graph(0)->setData(x,y);
    QVector<double> x1(10), y1(10);
    for(int i=0;i<10;i++)
    {
        x1[i] = i;
        y1[i] = 100*i;
    }
    ui->widget->addGraph();  //添加第二条曲线
    ui->widget->graph(1)->setPen(QPen(Qt::red));
    ui->widget->graph(1)->setBrush(QBrush(QColor(0, 255, 255, 100)));
    ui->widget->graph(1)->setName("曲线2");
    ui->widget->graph(1)->setData(x1, y1);

    ui->widget->xAxis->setRange(-11, 11);
    ui->widget->yAxis->setRange(-1000, 1000);
    /***************************************************** Plot Qwt 方式 ***************************************************/
    //设置标题
    ui->qwtPlot->detachItems();
    QwtText title;
    title.setText("速度");
    QFont font1("宋体",10);
    title.setFont(font1);
    ui->qwtPlot->setTitle(title);
    ui->qwtPlot->setPalette( Qt::red );
    ui->qwtPlot->setStyleSheet( "background-color:rgb(255, 255, 0, 255);color:rgb(255, 0, 255, 255)" );  //设置背景颜色和前景色  也可以写成background-color:yellow

    //设置画布
    QwtPlotCanvas *canvas;
    canvas = new QwtPlotCanvas();
    canvas->setPalette(Qt::black);
    canvas->setFrameStyle(QFrame::NoFrame);
    ui->qwtPlot->setCanvas(canvas);

    //设置网格
    QwtPlotGrid *grid;
    grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(Qt::green, 0, Qt::DotLine);
    grid->attach(ui->qwtPlot);

    //坐标轴上的字体
    QFont font2("宋体", 12);
    ui->qwtPlot->setAxisFont(QwtPlot::xBottom, font2);
    ui->qwtPlot->setAxisFont(QwtPlot::yLeft, font2);

//    ui->qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);  //设置自动坐标轴尺度
//    ui->qwtPlot->setAxisAutoScale(QwtPlot::xBottom, true);
    //以下坐标轴刻度的选择不能与上边自动坐标轴共存,只能设置一个,后设置的会覆盖前边的设置
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, 5, 1);  //设置坐标轴刻度范围,第一个数字:最小值;第二个数字:最大值;第三个数字:轴标步进
    ui->qwtPlot->setAxisMaxMajor( QwtPlot::xBottom, 5 );  //设置坐标轴有几个数标
    ui->qwtPlot->setAxisMaxMinor( QwtPlot::xBottom, 5 ); //设置坐标轴两个数标的中间分几个小份
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 5, 1);  //设置坐标轴刻度范围,第一个数字:最小值;第二个数字:最大值;第三个数字:轴标步进
    ui->qwtPlot->setAxisMaxMajor( QwtPlot::yLeft, 5 );  //设置坐标轴有几个数标
    ui->qwtPlot->setAxisMaxMinor( QwtPlot::yLeft, 2 ); //设置坐标轴两个数标的中间分几个小份

    //计算时间差
    QTime my_start_time(0,0,0,0);
    QTime my_end_time(0,0,1,10);
    int timeElapsed = my_start_time.msecsTo(my_end_time);
    qDebug() << "时间差(时):" << timeElapsed << "ms" << endl;
    QDate my_start_date(2018, 6, 10);
    QDate my_end_date(2018, 6, 11);
    int dateElapset = my_start_date.daysTo(my_end_date);
    qDebug() << "时间差(天):" << dateElapset << "天" << endl;
    QDateTime my_start_datetime( my_start_date, my_start_time );
    QDateTime my_end_datetime( my_end_date, my_end_time );
    int datetimeElapset = my_start_datetime.secsTo( my_end_datetime );
    qDebug() << "时间差(天.时):" << datetimeElapset << "s" << endl;



    //缩放
    QwtPlotMagnifier *magnifier;
    magnifier = new QwtPlotMagnifier(ui->qwtPlot->canvas());
    magnifier->setWheelFactor( 1.1 );  //设置缩放步进,同时与鼠标滚轮上下滚动与缩放方向有关

    //平移
    QwtPlotPanner *panner;
    panner = new QwtPlotPanner(ui->qwtPlot->canvas());
    //panner->setMouseButton( Qt::RightButton );  //设置哪个键平移画布

    //局部缩放
    QwtPlotZoomer *zoomer = new QwtPlotZoomer( ui->qwtPlot->canvas() );
    zoomer->setZoomBase( QRectF(0,0,6,6) );  //矩形:左上(0,0),100为长宽,这个尺寸是图像复原的尺寸,使用这个操作需要设置坐标刻度ui->qwtPlot->setAxisScale,不然使用默认复原尺寸
    //zoomer->setRubberBandPen( QColor( Qt::blue ) );  //设置选择放大区域的边框颜色
    //zoomer->setMousePattern( QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier );  //ctrl+右键==恢复到原始状态  MouseSelect2模式:默认左键缩放,恢复方式可以自定义
    //zoomer->setMousePattern( QwtEventPattern::MouseSelect3, Qt::RightButton );  //右键==恢复到上一次扩大之前的状态,默认左键放大,右键恢复原样,恢复上一级原样可以自方式定义
    //模式一,默认右键恢复原样,放大的方式可以自定义
    //zoomer->setMousePattern( QwtEventPattern::MouseSelect1, Qt::LeftButton ); //左键放大,右键恢复原样
    zoomer->setMousePattern( QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::ControlModifier ); //ctrl+左键放大,右键恢复原样
    //模式二,默认左键放大,恢复原样的方式可以自定义
    //模式三,默认左键放大,右键恢复原样,恢复上一级的方式可以自定义

    //设置画笔
    QPen pickerpen;
    pickerpen.setColor(Qt::black);

    //显示鼠标所在位置的坐标
    QwtPlotPicker *picker;
    picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                               QwtPicker::CrossRubberBand,
                               QwtPicker::AlwaysOn, ui->qwtPlot->canvas());
    picker->setTrackerPen(pickerpen);

    //画线
    QVector<double> x2,y2,y3; //模拟数据
    x2.push_back(1);
    x2.push_back(2);
    x2.push_back(3);
    y2.push_back(1);
    y2.push_back(1.5);
    y2.push_back(3);
    y3.push_back(1);
    y3.push_back(2.5);
    y3.push_back(3);
    QwtPlotCurve *curve, *curve2;  //创建曲线
    curve = new QwtPlotCurve();
    curve->setTitle("长度");     //曲线名字
    curve->setPen(Qt::red, 1);  //曲线颜色,宽度
    //curve->setStyle(QwtPlotCurve::Dots);  //散点图
    //curve->setCurveAttribute(QwtPlotCurve::Lines, true);
    curve->setSamples(x2, y2);  //曲线数据及绘制
    curve->setRenderHint(QwtPlotItem::RenderAntialiased);  //反锯齿
    curve->setSymbol(new QwtSymbol( QwtSymbol::Cross, Qt::NoBrush, QPen( Qt::color1), QSize(10, 10)));
    //curve->setLegendAttribute( curve->LegendNoAttribute );
    curve->attach(ui->qwtPlot);
    curve2 = new QwtPlotCurve();  //第二条曲线
    curve2->setPen(Qt::cyan, 2, Qt::DashLine);  //设置线的颜色,宽度,线型
    curve2->setTitle("质量");
    curve2->setCurveAttribute( QwtPlotCurve::CurveAttribute( QwtPlotCurve::Fitted ) );  //设置为平滑曲线
    //curve2->setRenderHint( QwtPlotItem::RenderAntialiased, true );   //不知道有啥用
    curve2->setLegendAttribute( curve2->LegendShowLine );  //设置legenditem使用的是颜色区分还是别的信息区分
    curve2->setSamples(x2, y3);
    curve2->attach(ui->qwtPlot);
    //curve2->detach();  //取消线条

    //插入图例legenditem
    QwtPlotLegendItem *d_qwtplotlegenditem1;
    d_qwtplotlegenditem1 = new QwtPlotLegendItem;
    d_qwtplotlegenditem1->setAlignment( Qt::AlignRight | Qt::AlignTop);  // 右上角对齐
    d_qwtplotlegenditem1->setMaxColumns( 1 );  //设置最大列数
    d_qwtplotlegenditem1->setBorderPen( QPen(QColor(255, 0, 0)) );  //设置边缘画笔
    d_qwtplotlegenditem1->setBackgroundBrush( QBrush(QColor(255, 255, 0, 100)) );   //设置背景笔刷, 透明度也在这里设置
    d_qwtplotlegenditem1->setBackgroundMode( QwtPlotLegendItem::LegendBackground );  //画一个框还是分别画框
    d_qwtplotlegenditem1->setTextPen( QPen(QColor(255, 0, 0)) );   //设置文本颜色
    d_qwtplotlegenditem1->setItemMargin(20);  //设置边缘
    d_qwtplotlegenditem1->setItemInterest( QwtPlotItem::ScaleInterest );     //不知道有啥用
    d_qwtplotlegenditem1->setItemSpacing(20);   //下边没注释的没仔细区分过功能
    d_qwtplotlegenditem1->setMargin(10);
    d_qwtplotlegenditem1->setSpacing(20);
    d_qwtplotlegenditem1->setBorderDistance(20);
    d_qwtplotlegenditem1->setBorderRadius(9);
    d_qwtplotlegenditem1->setItemAttribute( QwtPlotItem::AutoScale );
    d_qwtplotlegenditem1->attach(ui->qwtPlot);

    //插入图例 legend
    QwtLegend *legend;
    legend = new QwtLegend;
    //legend->setFrameShape();
    ui->qwtPlot->insertLegend(legend, QwtPlot::BottomLegend);  //插入到qwtplot里面  设置插入位置

    //qwtmarker 标注图中的一个点
    QwtPlotMarker *qwtplotmarker;
    qwtplotmarker = new QwtPlotMarker( "one" );
    QwtText title1;
    title1.setText("平均值");
    QFont font3("宋体",30);
    title1.setFont(font3);
    title1.setColor( Qt::green );
    qwtplotmarker->setTitle("two");
    //qwtplotmarker->setLabel( QwtText("mark" ) ); //标记内容
    qwtplotmarker->setLabel(title1);  //如果需要更改标记内容的大小,在这里更改
    qwtplotmarker->setValue( QPointF(2, 2) );  //要标记的点
    qwtplotmarker->setItemAttribute(QwtPlotItem::AutoScale, true);  //设置marker的自动标尺
    qwtplotmarker->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );  //对齐
    qwtplotmarker->setLabelOrientation(Qt::Horizontal);  //设置字体水平垂直
    qwtplotmarker->setLineStyle( QwtPlotMarker::LineStyle::HLine
                                 );  //设置标记风格
    qwtplotmarker->setLinePen(QPen( Qt::red, 1, Qt::DashDotDotLine ));  //设置marker颜色和线型
    qwtplotmarker->setSymbol( new QwtSymbol( QwtSymbol::DTriangle, Qt::NoBrush, QPen( Qt::color1), QSize(10, 10)) );  //被标记点的符号
    qwtplotmarker->attach(ui->qwtPlot);

    //可能会有用的操作,直接操作plot中的对象
    QwtPlotItemList items = ui->qwtPlot->itemList( QwtPlotItem::Rtti_PlotCurve );  //获取程序运行时的曲线信息,可以使用size函数获得曲线数量
    items[0]->setLegendIconSize(QSize( 10, 10 ));   //设置图标大小
    items[0]->setVisible(true);   //设置折线是否可见

    //调色板  下面没用
    QPalette d_palette;
    d_palette.setColor(QPalette::Background, QColor(255, 0, 0, 255));  //背景色
    d_palette.setColor(QPalette::Foreground, QColor(0, 255, 0, 255));  //前景色

    //画多条曲线,使用for循环里的同一个curve
//    QVector<QVector<double>> y3;
//    y3.push_back(y1);
//    y3.push_back(y2);
//    for(int i = 0; i < 2; i++){
//        QwtPlotCurve *curve;
//        QColor d_color;
//        curve = new QwtPlotCurve();
//        d_color.setHsl((25*i)%255, 255, ((i/10+1)*80)%255);
//        curve->setPen( d_color, 3);
//        curve->setTitle("质量");
//        curve->setSamples(x, y3[i]);
//        curve->attach(ui->qwtPlot);
//    }

    //重新绘制
    ui->qwtPlot->replot();

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
    if(event->type() == QEvent::MouseMove){  //鼠标移动事件是指鼠标按下之后移动,而不是当前焦点窗口的移动
        QMouseEvent *m_mouseMove = static_cast<QMouseEvent *>(event);
        qDebug() << "current move position is: " << m_mouseMove->pos();
        return true;
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

void MainWindow::menuFileOpen(bool)
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Open File"), tr(""), tr("Text File (*.txt)")); //最后这个参数确定打开的格式,任意格式可以将 .txt 改成 .* ,第一个参数是新建窗口的名字
    if(filename == ""){
        return;
    }else{
        QFile file(filename); //以下写打开文件等操作
        qDebug() << "I have get open file name: " << filename;
    }
}

void MainWindow::menuFileSave(bool)
{
    if(true){  //这里的true可以改成一个新文件标志位,用来表示文件是否未保存过,进入if之后要记得更改这个状态
        QString filename;
        filename = QFileDialog::getSaveFileName(this, tr("Save File"), tr("/home"), tr("Text File (*.txt)")); //最后这个参数确定打开的格式,任意格式可以将 .txt 改成 .* ,第一个参数是新建窗口的名字
        if(filename == ""){
            return;
        }else{
            QFile file(filename); //以下写打开文件等操作
            qDebug() << "I have get save file name: " << filename;
        }
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

void MainWindow::on_pushButton_5_clicked()
{
    irglDialog = new IrregularDialog(this);
    //irglDialog->setWindowModality(Qt::WindowModal); //设置对话框是模态还是非模态
    irglDialog->setModal(true);  //也是设置模态和非模态,跟上一句作用一样,但是效果有细微差别,推荐这种方法
    irglDialog->setWindowTitle("对话框");
    //irglDialog->exec();  //只能是模态对话框,跟属性值无关
    irglDialog->show();  //既可以是模态对话框也可以是非模态对话框,取决于modal属性
}
