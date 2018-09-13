#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <dialog.h>

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
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;  //事件过滤器最好声明成public,不然在类外安装的事件过滤器可能无效

protected:
    // Event handlers
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void combobox_list_solt(int state);

private:
    Ui::MainWindow *ui;
    Dialog *dialog;
    QListWidget *listwidget;
};

#endif // MAINWINDOW_H
