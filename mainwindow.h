#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <dialog.h>
#include <irregulardialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    // 返回值:不想让它继续转发，就返回 true，否则返回 false。
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;  //事件过滤器最好声明成public,不然在类外安装的事件过滤器可能无效

protected:
    // Event handlers
    // 返回值:类似于 eventfilter() 指示是否处理了事件。 true值表示进行了处理,可防止事件发送到其他对象。
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:

    void menuFileOpen(bool);

    void menuFileSave(bool);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void combobox_list_solt(int state);

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    Dialog *dialog;
    IrregularDialog *irglDialog;
    QListWidget *listwidget;
};

#endif // MAINWINDOW_H


/*
 * ==================================== 关于Qt事件 ====================================
 * 来源:https://zhuanlan.zhihu.com/p/50053079
 *
 * 在 Qt 里，一个事件就是一个对象，所有事件的祖先都来自于 QEvent。意思就是说，只要有一个事件发生（如鼠标单击事件），
 * 此时就会有一个 QEvent 对象被创建出来，然后开始各种传送。由于 Qt 事件系统是依托于元对象系统的，所以所有的 QObject 类
 * 都可以接收/处理 QEvent 事件。
 * 说起事件，其实无非就是围绕着“产生-发送-处理”这个基本流程来说的。
 *
 * ---- 如何产生一个事件
 *
 * 这是最简单的知识点了，当然是创建一个对象啦。这问题和“如何创建一个整数”一样简单。我创建一个整数是“int i”，那我创建一个
 * 事件就是“MyEvent event(1, 2, 3)”啦。其中 MyEvent 是我自定义的一个事件类，123是我往这个事件传入的参数。当你不知道
 * 怎么创建一个事件对象时，更多的是不知道一个事件对象长啥样。自己写的事件需要继承QEvent.
 *     class MyEvent : public QEvent
 *     {
 *         public:
 *             MyEvent();
 *             MyEvent(int x, int y, int z);
 *
 *             static const Type type;
 *
 *             int x;
 *             int y;
 *             int z;
 *     };
 *
 * 每一个事件类都有个唯一身份值：type值.这个 type 值的作用就是我们在处理事件时用于识别事件类的代号。在官方文档可以看到，
 * Qt 自带的事件类的 type 值都已经在 QEvent::type 中有了，数值范围在 0 - 999 之间。而我自己定义的事件类也有个type值，
 * 如上文的代码“static const Type type”。为了保证我的这个值不和 Qt 的冲突，所以数值要大于 999。Qt 给我们规定了两个
 * 边界值：QEvent::User 和 QEvent::MaxUser，即 1000 - 65535。但这也太难记了，怎么办？Qt 提供了一个函数
 * registerEventType() 专门用于自定义事件的注册。如下：
 *     const QEvent::Type MyEvent::type = (QEvent::Type)QEvent::registerEventType()
 * 好了，现在我们已经认识了 Qt 事件类长什么样了。接下来就是怎么把它发送出去了。
 *
 *
 * ---- 如何发送一个事件
 *
 * Qt 提供了三个 static 事件发送函数：sendEvent、postEvent、sendPostEvents。函数原型如下：
 *     bool QCoreApplication::sendEvent(QObject *receiver, QEvent *event)
 *     void QCoreApplication::postEvent(QObject *receiver, QEvent *event, int priority = Qt::NormalEventPriority)
 *     void QCoreApplication::sendPostedEvents(QObject *receiver = Q_NULLPTR, int event_type = 0)
 *
 * //直接发送：sendEvent
 * 这是最好理解的，两个参数中一个是要发给谁，另一个是发送什么事件。这种方式的发送我们要等待对方处理，所以返回值是一个 bool 量来判断。
 * 对于许多事件类，有一个名为 isAccepted() 来告诉你是否被处理。因为事件被发送的时候，event 对象并不会被销毁，因此我们要在栈上创建
 * event 对象。
 * 举例:
 *     int main(int argc, char *argv[])
 *     {
 *         QCoreApplication a(argc, argv);
 *
 *         MainWindow w;
 *         w.show();
 *
 *         MyEvent event(1, 2, 3);
 *         QCoreApplication::sendEvent(&w, &event);
 *
 *         return a.exec();
 *     };
 *
 * //发到队列：postEvent
 * 我们创建一个 Qt 程序的时候，一般在 main 下面会看到 QCoreApplication a(argc, argv) 以及 return a.exec() 的字样。这其实就是
 * 开启了 Qt 事件循环来维护一个事件队列，exec 的本质就是不停的调用 processEvent() 函数从队列中获取事件来处理。而 postEvent() 的
 * 作用就是把事件发送到这个队列中去。
 * 这种方式不需要等待处理结果，只要把事件发到队列中就可以了，所以返回值是 void。由于事件队列会持有发送的事件对象，在 post 的时候会将
 * 事件 delete 掉，所以我们必须在堆上创建 event 对象。
 *
 * //在队列中立即处理：sendPostedEvents
 * 看参数我们就可以知道，这个函数的作用就是立刻、马上将队列中的 event_type 类型的事件立马交给 receiver 进行处理。需要注意的是，来自
 * 窗口系统的事件并不由这个函数进行处理，而是 processEvent()。
 *
 *
 * ----如何处理一个事件  !!!
 * 我们发送的事件传给了 QObject 对象，更具体点是传给了 QObject 对象的 event() 函数。根据官方文档的说明，所有的事件都会进到这个函数
 * 里面，那么我们处理事件就要重写这个 event() 函数。也就是说在进行编程时,事件首先到达最底层子类的 event() 函数.
 *
 * event() 函数本身不会去处理事件，而是根据事件类型（type值）调用不同的事件处理器函数。
 *
 * 如果一个对象 A 安装了定义在对象 B 中的事件过滤器，则事件在到达目标对象(A)之前先被对象 B 的事件过滤器截获，至于是否要继续传递给目标
 * 对象需要根据返回值判断,如果在 eventfilter() 中返回 true ,则该事件不再继续传递,如果返回 false 则继续传递。
 *
 * 需要注意的是，重写事件处理器函数时，如果不实现任何功能，最好调用基类的实现。
 *
 * //事件接收者是 QObject 类型的，而 QObject 类有个 eventFilter() 函数，有什么用呢？
 * 比如在主界面上有一个按钮叫“A”，有一个按钮叫“B”。当 B->installEventFilter(MainWindow) 的时候，B 产生的事件就不会进入接收者
 * 的 event() 函数中了，而是进入接收者的 eventFilter() 函数中。
 * 注意：如果你在事件过滤器中 delete 了某个接收组件，务必将函数返回值设为 true。否则，Qt 还是会将事件分发给这个接收组件，从而导致程序
 * 崩溃。事件过滤器和被安装过滤器的组件必须在同一线程，否则，过滤器将不起作用。另外，如果在安装过滤器之后，这两个组件到了不同的线程，那么，
 * 只有等到二者重新回到同一线程的时候过滤器才会有效。
 *
 *
 * //接收/忽略事件
 * 所有的事件的祖先都是 QEvent 类，而该类里有个 isAccepted() 函数来判断这个事件是否被接受了。看到这里我们就能推测出 QEvent 类身上有
 * 一个私有变量，或者称为标签吧，用来指示 QEvent 是不是被人处理过了。我一个小小的事件处理没处理有啥关系？很有关系！顺序是这样的：当收到
 * 一个事件对象时，我先拿到这个事件。如果处理它，那么事情也就到此为止了；如果不去处理，它会传到我爸爸（基类）那儿去；他如果也不处理，就
 * 继续传给我的爷爷。
 * 事实上，我们很少会用到 accept()、ignore() 函数的，如果忽略事件（即自己不想去处理这个事件），只要调用父类的响应函数就可以了。
 *
 * 一个特殊的例子：QWidget
 * Qt 中大部分的事件处理函数都是 accept() 接收的事件对象，而 QWidget 却是 ignore() 的。这样的话如果某个 QWidget 子类想忽略某个事件，
 * 既可以调用 ignore() 函数，也可以调用基类 QWidget 的默认实现。另外还有一点需要知道，在图形编辑中，如果指定了父类（注意是父类，不是基类），
 * 事件的传播是在组件层面的，而不是靠类继承机制。
 *
 */
