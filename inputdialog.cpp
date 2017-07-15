#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <QDebug>
#include <QValidator>

inputDialog::inputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Input");
    this->resize(300, 150);
    QFont editFont("Consolas", 14);

    label = new QLabel("Input an integer");

    lineEdit = new QLineEdit;
    lineEdit->setFont(editFont);
    QValidator *validator=new QIntValidator(0, 65535, this);
    lineEdit->setValidator(validator);

    pushButton = new QPushButton("Done");

    layout = new QGridLayout();
    layout->addWidget(label, 0, 0, 1, 1);
    layout->addWidget(lineEdit, 1, 0, 1, 1);
    layout->addWidget(pushButton, 2, 0, 1, 1);

    setLayout(layout);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButton_clicked()));
}

inputDialog::~inputDialog()
{
    delete ui;
}

void inputDialog::pushButton_clicked()
{
    QString input = lineEdit->text();
    inputInt = input.toInt();
    accept();
}

