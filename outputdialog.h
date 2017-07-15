#ifndef OUTPUTDIALOG_H
#define OUTPUTDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class outputDialog;
}

class outputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit outputDialog(QWidget *parent = 0);
    ~outputDialog();

private:
    Ui::outputDialog *ui;
    QLabel * label;
    QLineEdit * lineEdit;
    QPushButton * pushButton;
    QGridLayout * layout;
public:
    int ans;
    void setShow(int n);
private slots:
    void pushButton_clicked();
};

#endif // OUTPUTDIALOG_H
