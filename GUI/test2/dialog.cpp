#include "ui_dialog.h"
#include "dialog.h"
#include <iostream>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    std::cout << this->ui->label << std::endl;
    std::cout << this->ui->label_2 << std::endl;
    std::cout << this->ui->name << std::endl;
    if (this->ui->textEdit->toPlainText() != NULL && this->ui->textEdit_2->toPlainText() != NULL && this->ui->name->toPlainText() != NULL)
    {
        this->close();
    }
    else
    {
        this->ui->label_3->setText("Error bad ip/port OR try to reconnect the server");
    }
}
