#include "aboutprog.h"
#include "ui_aboutprog.h"

aboutProg::aboutProg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::aboutProg)
{
    ui->setupUi(this);
}

aboutProg::~aboutProg()
{
    delete ui;
}

//Закрытие окна
void aboutProg::on_buttClose_clicked()
{
    close();
}
