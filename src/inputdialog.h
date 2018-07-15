#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

namespace Ui {
class inputDialog;
}

class inputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit inputDialog(QWidget *parent = 0);
    ~inputDialog();
    int inputInt;

private:
    Ui::inputDialog *ui;
    QLabel * label;
    QLineEdit * lineEdit;
    QPushButton * pushButton;
    QGridLayout * layout;
public slots:
    void pushButton_clicked();
};

#endif // INPUTDIALOG_H
