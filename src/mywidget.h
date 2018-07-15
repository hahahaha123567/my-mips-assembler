#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "inputdialog.h"
#include "outputdialog.h"
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <map>
#include <vector>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

    bool isCode(QString ins);
    unsigned getIns(QString ins);
    unsigned R_translator(QString ins, int i);
    unsigned I_translator(QString ins, int i);
    unsigned J_translator(QString ins, int i);
    unsigned C_translator(QString ins, int i);
    QString convertToOutput(unsigned code);

    void run(unsigned ins);
    void R_run(unsigned ins);
    void I_run(unsigned ins);
    void J_run(unsigned ins);
    void C_run(unsigned ins);
    int regName2Num(QString name);

    std::map<QString, int> * labelSet;
    std::vector<unsigned> * code;
    int pc;

private:
    Ui::MyWidget *ui;
    QMenuBar * bar;
    QMenu * file;
    QAction * open_file;
    QGridLayout * layout;
    QTextEdit * mipsText;
    std::vector<bool> * mipsList;
    QTextEdit * machineText;
    QTextEdit * memoryText;
    QLabel * regLabel;
    QLineEdit * regText;
    QPushButton * openButton;
    QPushButton * transButton;
    QPushButton * debugButton;
    QTextCursor * tempCursor1;
    QTextCursor * tempCursor2;
    inputDialog * in;
    outputDialog * out;
    void initReg();
    void showReg();
    void showMemory();
private slots:
    void openFile();
    void transButton_clicked();
    void debugButton_clicked();
    void getInput();
};

#endif // MYWIDGET_H
