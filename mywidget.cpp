#include "mywidget.h"
#include "ui_mywidget.h"
#include "mips.h"

#include <QDebug>
#include <QFileDialog>

#include <QTextBlock>
#include <QMessageBox>

#include <vector>
#include <map>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    initReg();
    ui->setupUi(this);
    setWindowTitle("Assembler");

    QFont editFont("Consolas", 14);

    mipsText = new QTextEdit;
    mipsText->setFont(editFont);

    machineText = new QTextEdit;
    machineText->setFont(editFont);
    machineText->setReadOnly(true);

    memoryText = new QTextEdit;
    memoryText->setFont(editFont);
    memoryText->setAlignment(Qt::AlignHCenter);
    memoryText->setReadOnly(true);

    regLabel = new QLabel[RegisterNum];
    for (int i = 0; i < RegisterNum; ++i) {
        regLabel[i].setText(regName[i]);
        regLabel[i].setFont(editFont);
    }

    regText = new QLineEdit[RegisterNum];
    for (int i = 0; i < RegisterNum; ++i) {
        regText[i].setFont(editFont);
        regText[i].setReadOnly(true);
    }

    openButton = new QPushButton("OpenFile");
    transButton = new QPushButton("Translate");
    debugButton = new QPushButton("Debug");

    // layout
    layout = new QGridLayout;
    layout->addWidget(mipsText, 0, 0, 9, 7);
    layout->addWidget(machineText, 0, 7, 9, 7);
    layout->addWidget(memoryText, 9, 0, 4, 14);
    for (int i = 16; i <= 23; ++i) {
        layout->addWidget(regLabel+i, i-16, 19, 1, 1, Qt::AlignHCenter);
        layout->addWidget(regText+i, i-16, 20, 1, 1, Qt::AlignHCenter);
    }
    for (int i = 4; i <= 7; ++i) {
        layout->addWidget(regLabel+i, i+4, 19, 1, 1, Qt::AlignHCenter);
        layout->addWidget(regText+i, i+4, 20, 1, 1, Qt::AlignHCenter);
    }
    for (int i = 8; i <= 15; ++i) {
        layout->addWidget(regLabel+i, i-8, 21, 1, 1, Qt::AlignHCenter);
        layout->addWidget(regText+i, i-8, 22, 1, 1, Qt::AlignHCenter);
    }
    for (int i = 24; i <= 25; ++i) {
        layout->addWidget(regLabel+i, i-16, 21, 1, 1, Qt::AlignHCenter);
        layout->addWidget(regText+i, i-16, 22, 1, 1, Qt::AlignHCenter);
    }
    for (int i = 2; i <= 3; ++i) {
        layout->addWidget(regLabel+i, i+8, 21, 1, 1, Qt::AlignHCenter);
        layout->addWidget(regText+i, i+8, 22, 1, 1, Qt::AlignHCenter);
    }
    layout->addWidget(regLabel+29, 12, 19, 1, 1, Qt::AlignHCenter);
    layout->addWidget(regText+29, 12, 20, 1, 1, Qt::AlignHCenter);
    layout->addWidget(regLabel+31, 12, 21, 1, 1, Qt::AlignHCenter);
    layout->addWidget(regText+31, 12, 22, 1, 1, Qt::AlignHCenter);
    layout->addWidget(openButton, 13, 6, 1, 1);
    layout->addWidget(transButton, 13, 10, 1, 1);
    layout->addWidget(debugButton, 13, 16, 1, 1);

    setLayout(layout);

    in = new inputDialog;

    tempCursor1 = nullptr;
    tempCursor2 = nullptr;
    MyWidget::showReg();

    // connect
    connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(transButton, SIGNAL(clicked()), this, SLOT(transButton_clicked()));
    connect(debugButton, SIGNAL(clicked()), this, SLOT(debugButton_clicked()));
    connect(in, SIGNAL(accepted()), this, SLOT(getInput()));
}

MyWidget::~MyWidget()
{
    delete ui;
    delete layout;
}

void MyWidget::transButton_clicked()
{
    QTextBlockFormat empty;
    empty.setBackground(Qt::white);
    // initial
    pc = 0;
    initReg();
    showReg();
    code = new std::vector<unsigned>;
    labelSet = new std::map<QString, int>;
    if (tempCursor1 != nullptr) {
        tempCursor1->setBlockFormat(empty);
    }
    tempCursor1 = nullptr;
    if (tempCursor2 != nullptr) {
        tempCursor2->setBlockFormat(empty);
    }
    tempCursor2 = nullptr;
    machineText->clear();

    // first scan, record labels
    mipsList = new std::vector<bool>;
    int count = mipsText->document()->blockCount();
    QTextBlock block = mipsText->document()->firstBlock();
    for (int i = 0; i < count; ++i, block = block.next()) {
        bool flag = isCode(block.text());
        mipsList->push_back(flag);
    }
    // second scan, translate
    block = mipsText->document()->firstBlock();
    for (int i = 0; i < count; ++i, block = block.next()) {
        if (mipsList->at(i)) {
            unsigned ins = getIns(block.text());
            code->push_back(ins);
            machineText->append(convertToOutput(ins));
            pc+=4;
        }
    }

    pc = 0;
    showMemory();
}

void MyWidget::debugButton_clicked()
{
    // end
    if (pc/4 >= code->size()) {
        QMessageBox::warning(this, "WARNING", "Program is done", QMessageBox::Abort);
        return;
    }
    // highlight in mips code
    QTextBlockFormat highlight;
    highlight.setBackground(Qt::lightGray);
    QTextBlockFormat empty;
    empty.setBackground(Qt::white);
    int limit = pc/4;
    for (int i = 0; i <= limit; ++i) {
        if (mipsList->at(i) == false) {
            limit++;
        }
    }
    if (tempCursor1 == nullptr) {
        tempCursor1 = new QTextCursor(mipsText->document());
    }
    else {
        tempCursor1->setBlockFormat(empty);
    }
    tempCursor1->setPosition(tempCursor1->document()->findBlockByNumber(limit).position());
    tempCursor1->setBlockFormat(highlight);
    // highlight in machine code
    if (tempCursor2 == nullptr) {
        tempCursor2 = new QTextCursor(machineText->document());
    }
    else {
        tempCursor2->setBlockFormat(empty);
    }
    tempCursor2->setPosition(tempCursor2->document()->findBlockByNumber(pc/4).position());
    tempCursor2->setBlockFormat(highlight);

    unsigned oldpc = pc;
    pc += 4;
    run(code->at(oldpc/4));
    showReg();
}

bool MyWidget::isCode(QString ins)
{
    ins = ins.trimmed();
    if (ins.size() == 0) {
        return false;
    }
    QString start = ins.section(' ', 0, 0);
    if (start.endsWith(':')) {
        int count = 0;
        for (int i = 0; i < mipsList->size(); ++i) {
            if (mipsList->at(i)) {
                count++;
            }
        }
        labelSet->insert(std::map<QString, int>::value_type(start.trimmed().remove(':'), count  ));
        if (ins.section(' ', 1, 1).trimmed().size() == 0) {
            return false;
        }
    }
    return true;
}

unsigned MyWidget::getIns(QString ins)
{
    ins = ins.trimmed();
    QString start = ins.section(' ', 0, 0);
    for (int i = 0; i < RInstructionNum; ++i) {
        if(start == R_name[i]) {
            return R_translator(ins.remove(start), i);
        }
    }
    for (int i = 0; i < IInstructionNum; ++i) {
        if(start == I_name[i]){
            return I_translator(ins.remove(start), i);
        }
    }
    for (int i = 0; i < JInstructionNum; ++i) {
        if(start == J_name[i]){
            return J_translator(ins.remove(start), i);
        }
    }
    for (int i = 0; i < CInstructionNum; ++i) {
        if(start == C_name[i]){
            return C_translator(ins.remove(start), i);
        }
    }
    QMessageBox::warning(this, "WARNING", "Instruction error!", QMessageBox::Abort);
    return 0;
}

unsigned MyWidget::R_translator(QString ins, int i)
{
    unsigned ret = 0;
    QString rs, rt, rd, sa;
    unsigned rsValue = 0, rtValue = 0, rdValue = 0, saValue = 0, func = R_Func[i];

    switch (i) {
    // no arguments
    case 3:
    case 26: break;
    // rs, rt
    case 4:
    case 5:
    case 12:
    case 13:
        ins = ins.trimmed();
        rs = ins.section(',', 0, 0).trimmed();
        rt = ins.section(',', 1, 1).trimmed();
        rsValue = regName2Num(rs);
        rtValue = regName2Num(rt);
        break;
    // rd, rs
    case 6:
        ins = ins.trimmed();
        rd = ins.section(',', 0, 0).trimmed();
        rs = ins.section(',', 1, 1).trimmed();
        rdValue = regName2Num(rd);
        rsValue = regName2Num(rs);
        break;
    // rs
    case 7:
    case 10:
    case 11:
        ins = ins.trimmed();
        rs = ins.section(',', 0, 0).trimmed();
        rsValue = regName2Num(rs);
        break;
    // rd
    case 8:
    case 9:
        ins = ins.trimmed();
        rd = ins.section(',', 0, 0).trimmed();
        rdValue = regName2Num(rd);
        break;
    // rd, rt, sa
    case 16:
    case 20:
    case 22:
        ins = ins.trimmed();
        rd = ins.section(',', 0, 0).trimmed();
        rt = ins.section(',', 1, 1).trimmed();
        sa = ins.section(',', 2, 2).trimmed();
        rdValue = regName2Num(rd);
        rtValue = regName2Num(rt);
        saValue = regName2Num(sa);
        break;
    // rd, rt, rs
    case 17:
    case 21:
    case 23:
        ins = ins.trimmed();
        rd = ins.section(',', 0, 0).trimmed();
        rt = ins.section(',', 1, 1).trimmed();
        rs = ins.section(',', 2, 2).trimmed();
        rdValue = regName2Num(rd);
        rtValue = regName2Num(rt);
        rsValue = regName2Num(rs);
        break;
    // rd, rs, rt
    default:
        ins = ins.trimmed();
        rd = ins.section(',', 0, 0).trimmed();
        rs = ins.section(',', 1, 1).trimmed();
        rt = ins.section(',', 2, 2).trimmed();
        rdValue = regName2Num(rd);
        rsValue = regName2Num(rs);
        rtValue = regName2Num(rt);
    }
    ret = ret | rsValue;
    ret <<= 5;
    ret = ret | rtValue;
    ret <<= 5;
    ret = ret | rdValue;
    ret <<= 5;
    ret = ret | saValue;
    ret <<= 6;
    ret = ret | func;

    return ret;
}

unsigned MyWidget::I_translator(QString ins, int i)
{
    unsigned ret = 0;
    QString rs, rt, label;
    unsigned opcode = I_opcode[i], rsValue = 0, rtValue = 0;
    int imme = 0, labelValue = 0, ans = 0;
    std::map<QString, int>::iterator it;

    switch (i) {
    // rt, rs, imme
    case 0:
    case 1:
    case 2:
    case 16:
    case 18:
    case 19:
    case 23:
        ins = ins.trimmed();
        rt = ins.section(',', 0, 0).trimmed();
        rs = ins.section(',', 1, 1).trimmed();
        imme = (unsigned)ins.section(',', 2, 2).toInt();
        rtValue = regName2Num(rt);
        rsValue = regName2Num(rs);
        ans = imme;
        break;
    // rs, rt, label
    case 3:
    case 8:
        ins = ins.trimmed();
        rs = ins.section(',', 0, 0).trimmed();
        rt = ins.section(',', 1, 1).trimmed();
        label = ins.section(',', 2, 2).trimmed();
        rsValue = regName2Num(rs);
        rtValue = regName2Num(rt);
        it = labelSet->find(label);
        if (it == labelSet->end()) {
            QMessageBox::warning(this, "WARNING", "Label error!", QMessageBox::Abort);
        }
        else {
            labelValue = it->second * 4 - pc - 4;
        }
        ans = labelValue;
        break;
    // rs, label
    case 4:
    case 5:
    case 6:
    case 7:
        ins = ins.trimmed();
        rs = ins.section(',', 0, 0).trimmed();
        label = ins.section(',', 1, 1).trimmed();
        rsValue = regName2Num(rs);
        it = labelSet->find(label);
        if (it == labelSet->end()) {
            QMessageBox::warning(this, "WARNING", "Label error!", QMessageBox::Abort);
        }
        else {
            labelValue = it->second * 4 - pc - 4;
        }
        ans = labelValue;
        break;
    // rt, imme
    case 13:
        ins = ins.trimmed();
        rt = ins.section(',', 0, 0).trimmed();
        imme = (unsigned)ins.section(',', 1, 1).toInt();
        rtValue = regName2Num(rt);
        ans = imme;
        break;
    // rt, immediate(rs)
    default:
        ins = ins.trimmed();
        rt = ins.section(',', 0, 0).trimmed();
        imme = (unsigned)ins.section(',', 1, 1).trimmed().section('(', 0, 0).trimmed().toInt();
        rs = ins.section(',', 1, 1).trimmed().section('(', 1, 1).trimmed().section(')', 0, 0).trimmed();
        rtValue = regName2Num(rt);
        rsValue = regName2Num(rs);
        ans = imme;
    }
    ans = ans & 0x0000FFFF;
    ret = ret | opcode;
    ret <<= 5;
    ret = ret | rsValue;
    ret <<= 5;
    ret = ret | rtValue;
    ret <<= 16;
    ret = ret | ans;

    return ret;
}

unsigned MyWidget::J_translator(QString ins, int i)
{
    unsigned ret = 0, labelValue = 0;
    QString label;

    label = ins.trimmed();
    labelValue = (*labelSet)[label];
    ret = ret | J_opcode[i];
    ret <<= 26;
    ret = ret | (labelValue << 4);

    return ret;
}

unsigned MyWidget::C_translator(QString ins, int i)
{
    unsigned ret = 0, ftValue = 0, fsValue = 0, fdValue = 0;
    QString ft, fs, fd;

    switch (i) {
    // ft, fs
    case 4:
    case 6:
        ins = ins.trimmed();
        ft = ins.section(',', 0, 0).trimmed();
        fs = ins.section(',', 1, 1).trimmed();
        ftValue = regName2Num(ft);
        fsValue = regName2Num(fs);
        break;
    // fd, fs
    case 5:
        ins = ins.trimmed();
        fd = ins.section(',', 0, 0).trimmed();
        fs = ins.section(',', 1, 1).trimmed();
        fdValue = regName2Num(fd);
        fsValue = regName2Num(fs);
        break;
    // fd, fs, ft
    default:
        ins = ins.trimmed();
        fd = ins.section(',', 0, 0).trimmed();
        fs = ins.section(',', 1, 1).trimmed();
        ft = ins.section(',', 2, 2).trimmed();
        fdValue = regName2Num(fd);
        fsValue = regName2Num(fs);
        ftValue = regName2Num(ft);
    }
    ret = ret | 0x11;
    ret <<= 5;
    ret = ret | (C_Func[i] & 0x1F);
    ret <<= 5;
    ret = ret | ftValue;
    ret <<= 5;
    ret = ret | fsValue;
    ret <<= 5;
    ret = ret | fdValue;
    ret <<= 6;
    ret = ret | (C_Func[i] >> 5);

    return ret;
}

int  MyWidget::regName2Num(QString name)
{
    for (int i = 0; i < RegisterNum; ++i) {
        if (name == regName[i]) {
            return i;
        }
    }
    QMessageBox::warning(this, "WARNING", "Register error!", QMessageBox::Abort);
    return 0;
}

QString MyWidget::convertToOutput(unsigned code)
{
    QString ret;
    for (int i = 0; i < 32; ++i) {
        if ((code & (unsigned)1) == 0) {
            ret.prepend("0");
        }
        else {
            ret.prepend("1");
        }
        code >>= 1;
    }
    ret.insert(6, " ");
    ret.insert(12, " ");
    ret.insert(18, " ");
    ret.insert(24, " ");
    ret.insert(30, " ");
    return ret;
}

void MyWidget::initReg()
{
    for (int i = 0; i < RegisterNum; ++i) {
        reg[i] = 0;
    }
    reg[29] = 0xFF; // $sp
}

void MyWidget::showReg()
{
    for (int i = 0; i < RegisterNum; ++i) {
        QString num;
        num = num.setNum(reg[i], 16).toUpper();
        while(num.size() < 8) {
            num.prepend('0');
        }
        num.insert(4, ' ');
        regText[i].setText(num);
    }
}

void MyWidget::showMemory()
{
    memoryText->clear();
    for (int i = 0; i < 256; ++i) {
        QString str;
        if (i < code->size()){
            str.setNum(code->at(i), 16);
            while (str.size() < 8) {
                str.prepend("0");
            }
        }
        else {
            str = "00000000";
        }
        str.insert(2, ' ');
        str.insert(5, ' ');
        str.insert(8, ' ');
        QString index;
        index.setNum(i*4, 16);
        index = index.toUpper();
        index += ": ";
        str.prepend(index);
        if (str.size() == 14) {
            str.prepend('0');
        }
        memoryText->append(str);
    }
}

void MyWidget::getInput()
{
    reg[2] = in->inputInt;
}

void MyWidget::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "OpenFile", "I:\\Qt\\MyAssembler", "");
    QFile * file = new QFile(fileName);
    file->open(QIODevice::ReadOnly|QIODevice::Text);

    mipsText->clear();
    QString txt = QString(file->readAll());
    mipsText->append(txt);

    file->close();
}

void MyWidget::run(unsigned ins)
{
    switch (ins >> 26) {
    case 0x00: R_run(ins);
        break;
    case 0x02:
    case 0x03: J_run(ins);
        break;
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13: C_run(ins);
        break;
    default:   I_run(ins);
    }
}

void MyWidget::R_run(unsigned ins)
{
    unsigned int rs = (ins >> 21) & 0x1F;
    unsigned int rt = (ins >> 16) & 0x1F;
    unsigned int rd = (ins >> 11) & 0x1F;
    unsigned int sa = (ins >> 6) & 0x1F;
    unsigned int func = ins & 0x3F;

    switch (func) {
        // add
    case 0x20:
        if (((int)reg[rs] > 0 && (int)reg[rt] > 0 && (int)(reg[rs] + reg[rt]) <= 0) ||
            ((int)reg[rs] < 0 && (int)reg[rt] < 0 && (int)(reg[rs] + reg[rt]) >= 0)) {
            QMessageBox::warning(this, "WARNING", "Integer Overflow!", QMessageBox::Abort);
            return;
        }
        // addu
    case 0x21:
        reg[rd] = reg[rs] + reg[rt];
        break;
        // sub
    case 0x22:
        if (((int)reg[rs] > 0 && (int)reg[rt] < 0 && (int)(reg[rs] + reg[rt]) <= 0) ||
            ((int)reg[rs] < 0 && (int)reg[rt] > 0 && (int)(reg[rs] + reg[rt]) >= 0)) {
            QMessageBox::warning(this, "WARNING", "Integer Overflow!", QMessageBox::Abort);
            return;
        }
        // subu
    case 0x23:
        reg[rd] = reg[rs] - reg[rt];
        break;
        // slt
    case 0x2A:
        reg[rd] = (int)reg[rs] < (int)reg[rt] ? 1 : 0;
        break;
        // sltu
    case 0x2B:
        reg[rd] = reg[rs] < reg[rt] ? 1 : 0;
        break;
        // and
    case 0x24:
        reg[rd] = reg[rs] & reg[rt];
        break;
        // or
    case 0x25:
        reg[rd] = reg[rs] | reg[rt];
        break;
        // xor
    case 0x26:
        reg[rd] = reg[rs] ^ reg[rt];
        break;
        // nor
    case 0x27:
        reg[rd] = ~(reg[rs] | reg[rt]);
        break;
        // sll
    case 0x00:
        reg[rd] = reg[rt] << sa;
        break;
        // srl
    case 0x02:
        reg[rd] = reg[rt] >> sa;
        break;
        // sra
    case 0x03:
        reg[rd] = reg[rt] << sa;
        break;
        // mult
    case 0x18:
        {
        long temp1 = (int)reg[rs] * (int)reg[rt];
        unsigned * tempptr1 = (unsigned *)&temp1;
        reg[32] = *tempptr1;
        tempptr1++;
        reg[33] = *tempptr1;
        break;
        }
        // multu
    case 0x19:
        {
        long temp2 = reg[rs] * reg[rt];
        unsigned * tempptr2 = (unsigned *)&temp2;
        reg[32] = *tempptr2;
        tempptr2++;
        reg[33] = *tempptr2;
        break;
        }
        // div
    case 0x1A:
        reg[32] = 0;
        reg[33] = (int)reg[rs] / (int)reg[rt];
        break;
        // divu
    case 0x1B:
        reg[32] = 0;
        reg[33] = reg[rs] / reg[rt];
        break;
        // mfhi
    case 0x10:
        reg[rd] = reg[32];
        break;
        // mthi
    case 0x11:
        reg[32] = reg[rd];
        break;
        // mflo
    case 0x12:
        reg[rd] = reg[33];
        break;
        // mtlo
    case 0x13:
        reg[33] = reg[rd];
        break;
        // jr
    case 0x08:
        pc = reg[rs];
        break;
        // jalr
    case 0x09:
        pc = reg[rs];
        reg[rd] = pc + 4;
        break;
        // sllv
    case 0x04:
        reg[rd] = reg[rt] << reg[rs];
        break;
        // srlv
    case 0x06:
        reg[rd] = reg[rt] >> reg[rs];
        break;
        // srav
    case 0x07:
        reg[rd] = (int)reg[rt] >> reg[rs];
        break;
        // system call
    case 0x0C:
        switch (reg[2]) {
        case 1:
            out = new outputDialog(this);
            out->setShow(reg[4]);
            out->show();
            break;
        case 5:
            in->show();
        }
    }
}

void MyWidget::I_run(unsigned ins)
{
    unsigned int opcode = ins >> 26;
    unsigned int rs = (ins >> 21) & 0x1F;
    unsigned int rt = (ins >> 16) & 0x1F;
    unsigned int imme = ins & 0x0000FFFF;
    int s_imme = imme;
    if (imme & 0x00008000) {
        s_imme = 0xFFFF0000 | s_imme;
    }

    switch (opcode) {
    // lw
    case 0x23:
        {
        int index = 0;
        index += reg[rs]/4;
        index += (int)imme/4;
        reg[rt] = code->at(index);
        showMemory();
        break;
        }
    // sw
    case 0x2B:
        {
        int index = 0;
        index += reg[rs]/4;
        index += (int)imme/4;
        code->at(index) = reg[rt];
        showMemory();
        break;
        }
    // addi
    case 0x08:
        if (((int)reg[rs] > 0 && s_imme > 0 && reg[rs] + s_imme <= 0) ||
            ((int)reg[rs] < 0 && s_imme < 0 && reg[rs] + s_imme >= 0)) {
            QMessageBox::warning(this, "WARNING", "Integer Overflow!", QMessageBox::Abort);
            return;
        }
    // addiu
    case 0x09:
        reg[rt] = reg[rs] + s_imme;
        qDebug() << s_imme << endl;
        break;
    // ori
    case 0x0D:
        reg[rt] = reg[rs] | imme;
        break;
    // xori
    case 0x0E:
        reg[rt] = reg[rs] ^ imme;
        break;
    // lui
    case 0x0F:
        reg[rt] = imme << 16;
        break;
    // slti
    case 0x0A:
        reg[rt] = (int)reg[rs] < s_imme ? 1 : 0;
        break;
    // sltiu
    case 0x0B:
        reg[rt] = reg[rs] < imme ? 1 : 0;
        break;
    // beq
    case 0x04:
        if (reg[rs] == reg[rt]) {
            pc += s_imme;
        }
        break;
    // bne
    case 0x05:
        if (reg[rs] != reg[rt]) {
            pc += s_imme;
        }
        break;
    // blez
    case 0x06:
        if ((int)reg[rs] <= 0) {
            pc += s_imme;
        }
        break;
    // bgtz
    case 0x07:
        if ((int)reg[rs] > 0) {
            pc += s_imme;
        }
    }
}

void MyWidget::J_run(unsigned ins)
{
    int flag = 0x04000000 & ins;

    if (flag) {
        reg[31] = pc;
    }
    pc = (ins & 0x03FFFFFF) >> 2;
}

void MyWidget::C_run(unsigned ins)
{
    ins;
    return;
}
