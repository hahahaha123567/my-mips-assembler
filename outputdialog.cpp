#include "outputdialog.h"
#include "ui_outputdialog.h"
#include <QDebug>

outputDialog::outputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::outputDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Output");
    this->resize(300, 150);
    QFont editFont("Consolas", 14);

    label = new QLabel("Answer");

    lineEdit = new QLineEdit;
    lineEdit->setFont(editFont);
    lineEdit->setReadOnly(true);

    pushButton = new QPushButton("Done");

    layout = new QGridLayout();
    layout->addWidget(label, 0, 0, 1, 1);
    layout->addWidget(lineEdit, 1, 0, 1, 1);
    layout->addWidget(pushButton, 2, 0, 1, 1);

    setLayout(layout);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButton_clicked()));
}



outputDialog::~outputDialog()
{
    delete ui;
}

void outputDialog::setShow(int n)
{
    QString str;
    lineEdit->setText(str.number(n));
    qDebug() << str << endl;
}

void outputDialog::pushButton_clicked()
{
    accept();
}
