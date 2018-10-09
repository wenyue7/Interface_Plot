#ifndef IRREGULARDIALOG_H
#define IRREGULARDIALOG_H

#include <QDialog>

namespace Ui {
class IrregularDialog;
}

class IrregularDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IrregularDialog(QWidget *parent = 0);
    ~IrregularDialog();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

private:
    Ui::IrregularDialog *ui;

    QPixmap *pixw; //用于存储窗口的图片
    QPoint relativePos;
};

#endif // IRREGULARDIALOG_H
