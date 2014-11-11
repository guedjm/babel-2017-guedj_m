#include <QtPrintSupport/QPrinter>
#include <list>
#include <iostream>
#include "babelwindow.h"
#include "ui_babelwindow.h"

BabelWindow::BabelWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BabelWindow)
{
	ui->setupUi(this);
	hideContent();
	displayList();
	this->ui->textEdit_seeMessage->setDisabled(true);
	this->ui->lineEdit_password->setEchoMode(QLineEdit::Password);
	this->ui->lineEdit_passwordregister->setEchoMode(QLineEdit::Password);
	this->ui->lineEdit_passwordconfirmregister->setEchoMode(QLineEdit::Password);
}

BabelWindow::~BabelWindow()
{
    delete ui;
}

void    BabelWindow::hideContent()
{
    this->ui->dockWidget->hide();
}

void BabelWindow::displayList()
{
    QStringList myList;
    myList << "DarkYukii" << "M4x0w" << "toLLL" << "Sylv";
    foreach (QString toto, myList)
    {
        this->ui->listConnected->addItem(toto);
    }
}

void BabelWindow::on_listConnected_itemClicked(QListWidgetItem *item)
{
    this->ui->label_seePseudo->setText(item->text());
}

void BabelWindow::on_pushButton_registration_clicked()
{
	this->ui->stackedWidget->setCurrentIndex(2);
}

void BabelWindow::on_pushButton_register_clicked()
{
	if (this->ui->lineEdit_pseudoregister->text().toStdString() != "" &&
		this->ui->lineEdit_passwordregister->text().toStdString() != "" &&
		this->ui->lineEdit_passwordconfirmregister->text().toStdString() != "" &&
		this->ui->lineEdit_passwordregister->text().toStdString() == this->ui->lineEdit_passwordconfirmregister->text().toStdString() &&
		registerServer(this->ui->lineEdit_passwordregister->text().toStdString(), this->ui->lineEdit_pseudoregister->text().toStdString()))
	{
		this->ui->stackedWidget->setCurrentIndex(0);
		this->ui->statusBar->showMessage("Your registration has been updated", 5000);
	}
}

bool BabelWindow::registerServer(std::string passwd, std::string pseudo)
{
	return (true);
}

void BabelWindow::on_pushButton_seconnecter_clicked()
{
    if (this->ui->lineEdit_pseudo->text().toStdString() != "" &&
            this->ui->lineEdit_password->text().toStdString() != "" &&
            this->ui->lineEdit_ip->text().toStdString() != "" &&
            this->ui->lineEdit_port->text().toStdString() != "" &&
            connectServer(this->ui->lineEdit_pseudo->text().toStdString(), this->ui->lineEdit_password->text().toStdString(), this->ui->lineEdit_ip->text().toStdString(), this->ui->lineEdit_port->text().toStdString()))
    {
        this->ui->stackedWidget->setCurrentIndex(1);
        this->ui->dockWidget->show();
    }
    else
		this->ui->statusBar->showMessage("Error identifier, try again", 5000);
}

bool	BabelWindow::connectServer(std::string pseudo, std::string password, std::string ip, std::string port)
{
    std::cout << ip << std::endl;
    std::cout << port << std::endl;
    std::cout << pseudo << std::endl;
    std::cout << password << std::endl;
	return (true);
}

void BabelWindow::on_pushButton_send_clicked()
{
	if (this->ui->textEdit_message->toPlainText().toStdString() != "")
	{
		this->tmp += this->ui->textEdit_message->toPlainText() + '\n';
		this->ui->textEdit_seeMessage->setText(tmp);
		this->ui->textEdit_message->clear();
	}
	QTextCursor c = this->ui->textEdit_seeMessage->textCursor();
	c.movePosition(QTextCursor::End);
	this->ui->textEdit_seeMessage->setTextCursor(c);
}

void BabelWindow::on_pushButton_begin_clicked()
{

}

void BabelWindow::on_pushButton_end_clicked()
{

}
