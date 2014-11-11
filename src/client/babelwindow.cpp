#include <QtPrintSupport/QPrinter>
#include <list>
#include <iostream>
#include <QMessageBox>
#include "babelwindow.h"
#include "ui_babelwindow.h"

BabelWindow::BabelWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BabelWindow)
{
	ui->setupUi(this);
    hideContent();
	this->ui->textEdit_seeMessage->setDisabled(true);
	this->ui->lineEdit_password->setEchoMode(QLineEdit::Password);
	this->ui->lineEdit_passwordregister->setEchoMode(QLineEdit::Password);
	this->ui->lineEdit_passwordconfirmregister->setEchoMode(QLineEdit::Password);
}

BabelWindow::~BabelWindow()
{
    delete ui;
}

void    BabelWindow::closeEvent(QCloseEvent *)
{
    this->_workerThread.exit(0);
    this->_workerThread.wait();
}

void    BabelWindow::showEvent(QShowEvent *)
{
    this->_worker = new ClientWorker(this->_data, &this->_workerThread);
    this->_worker->moveToThread(&this->_workerThread);
    connect(this, SIGNAL(onSignInRequest()), this->_worker, SLOT(onSignInRequest()));
    connect(this, SIGNAL(onSignUpRequest()), this->_worker, SLOT(onSignUpRequest()));
    connect(this, SIGNAL(onCall(QString const &, std::list<std::string> const &)), this->_worker, SLOT(onCall(QString const &, std::list<std::string> const &)));
    connect(this, SIGNAL(onHangUp()), this->_worker, SLOT(onHangUp()));
    connect(this, SIGNAL(onSend()), this->_worker, SLOT(onSend()));
    connect(this, SIGNAL(onAddFriend()), this->_worker, SLOT(onAddFriend()));
    connect(this, SIGNAL(onRemoveFriend()), this->_worker, SLOT(onRemoveFriend()));
    connect(this, SIGNAL(onAcceptRequestCall()), this->_worker, SLOT(onAcceptRequestCall()));
    connect(this, SIGNAL(onDeclineRequestCall()), this->_worker, SLOT(onDeclineRequestCall()));
    connect(this, SIGNAL(answerRequestCall(QString, bool)), this->_worker, SLOT(answerRequestCall(QString, bool)));

    connect(this->_worker, SIGNAL(connectedToServer()), this, SLOT(onConnectionSuccess()));
    connect(this->_worker, SIGNAL(friendStatusUpdate()), this, SLOT(onUpdateListFriend()));
    connect(this->_worker, SIGNAL(connectionFailed()), this, SLOT(onConnectionFailed()));
    connect(this->_worker, SIGNAL(friendRequest(QString)), this, SLOT(onFriendRequest(QString)));
    connect(this->_worker, SIGNAL(friendRequestSended()), this, SLOT(onfriendRequestSended()));
    connect(this->_worker, SIGNAL(callRequest(QString const &, std::list<QString>)), this, SLOT(onCall(QString const &, std::list<QString>)));
    connect(this->_worker, SIGNAL(callDropped(QString)), this, SLOT(onCallDropped(QString)));
    this->_workerThread.start();
}

void    BabelWindow::onConnectionSuccess()
{
    this->displayList();
    this->onUpdateListFriend();
    this->ui->stackedWidget->setCurrentIndex(2);
    this->ui->dockWidget->show();
    this->ui->pushButton_call->show();
    this->ui->pushButton_HangUp->show();
    this->ui->label_seePseudo->show();
    this->ui->pushButton_send->show();
    this->ui->textEdit_message->show();
    this->ui->textEdit_seeMessage->show();
}

void    BabelWindow::hideContent()
{
    this->ui->dockWidget->hide();
    this->ui->pushButton_send->hide();
    this->ui->textEdit_seeMessage->hide();
    this->ui->textEdit_message->hide();
    this->ui->pushButton_call->hide();
    this->ui->pushButton_HangUp->hide();
    this->ui->label_seePseudo->hide();
}

void BabelWindow::displayList()
{
    std::list<Friend> myList;
    myList = this->_data.getUser().getFriendList();
    foreach (Friend toto, myList)
    {
        this->ui->listConnected->addItem(toto.getName());
    }
    this->_data.lockUser();
}

void    BabelWindow::onUpdateListFriend()
{
    std::list<Friend>   myList;
    this->ui->listConnected->clear();
    myList = this->_data.getUser().getFriendList();
    foreach (Friend toto, myList)
    {
        if (toto.getStatus() == 0)
            this->ui->listConnected->addItem(toto.getName() + " -> (Disconnected)");
        else if (toto.getStatus() == 1)
            this->ui->listConnected->addItem(toto.getName() + " -> (Connected)");
    }
    this->_data.lockUser();
}

void    BabelWindow::onConnectionFailed()
{
    QMessageBox msgBox;
    msgBox.setText("Unable to connect to the server ...");
    msgBox.exec();
    this->ui->statusBar->clearMessage();
    this->ui->pushButton_seconnecter->setEnabled(true);
    this->ui->pushButton_registration->setEnabled(true);
    this->ui->pushButton_register->setEnabled(true);
    this->ui->btn_swap_signin->setEnabled(true);
}

void    BabelWindow::onFriendRequest(QString name)
{
    onUpdateListFriend();
    QMessageBox msgBox;
    msgBox.setText(name + " want to be your friend.");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
        emit this->onAddFriend(name);
    else
        msgBox.close();
    msgBox.exec();
}

void    BabelWindow::onCommandFailed(eErrorCode err)
{
    QMap<eErrorCode, QString> strErr;
    strErr[InvalidUsername] = "SignIn failure : Invalid username";
    strErr[InvalidPassword] = "SignIn failure : Invalid password";
    strErr[UnavailableUsername] = "SignUp failure : Username unvailable";
    strErr[UnknownUser] = "Friend requested failure : Unknow user";
    strErr[UserAlreadyConnected] = "SignIn failure : User already connected";
    strErr[AlreadyFriend] = "Friend requested failure : Already friend";
    strErr[FriendsMaxedOut] = "Friend requested failure : Too much friend";
    strErr[UserOffline] = "User is offline";
    strErr[RequestPending] = "Friend requested failure : Firend request pending";
    strErr[AuthentificationTimeOut] = "SignIn failure : Authentification timed out";

    QMessageBox msgBox;
    msgBox.setText(strErr[err]);
    msgBox.exec();

    if (err == InvalidUsername || err == InvalidPassword || err == UserAlreadyConnected
            || err == UnavailableUsername || err == AuthentificationTimeOut)
    {
        this->ui->statusBar->clearMessage();
        this->ui->pushButton_seconnecter->setEnabled(true);
        this->ui->pushButton_registration->setEnabled(true);
        this->ui->pushButton_register->setEnabled(true);
        this->ui->btn_swap_signin->setEnabled(true);
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
    if (!this->ui->txt_signUpIp->text().isEmpty() &&
        this->ui->lineEdit_pseudoregister->text().toStdString() != "" &&
		this->ui->lineEdit_passwordregister->text().toStdString() != "" &&
		this->ui->lineEdit_passwordconfirmregister->text().toStdString() != "" &&
        this->ui->lineEdit_passwordregister->text().toStdString() == this->ui->lineEdit_passwordconfirmregister->text().toStdString())
	{
        this->_data.lockUser();
        User &user = this->_data.getUser();
        user.setIdent(this->ui->lineEdit_pseudoregister->text(), this->ui->lineEdit_passwordregister->text());
        this->_data.unlockUser();
        this->_data.lockServer();
        this->_data.setServerIp(this->ui->txt_signUpIp->text());
        this->_data.unlockServer();
        emit this->onSignUpRequest();
        this->ui->statusBar->showMessage("Connecting ...", 0);
        this->ui->pushButton_register->setEnabled(false);
        this->ui->btn_swap_signin->setEnabled(false);
    }

}

void BabelWindow::on_pushButton_seconnecter_clicked()
{
    if (this->ui->lineEdit_pseudo->text().toStdString() != "" &&
            this->ui->lineEdit_password->text().toStdString() != "" &&
            this->ui->lineEdit_ip->text().toStdString() != "")
    {
        this->_data.lockUser();
        User &user = this->_data.getUser();
        user.setIdent(this->ui->lineEdit_pseudo->text(), this->ui->lineEdit_password->text());
        this->_data.unlockUser();
        this->_data.lockServer();
        this->_data.setServerIp(this->ui->lineEdit_ip->text());
        this->_data.unlockServer();
        emit this->onSignInRequest();
        this->ui->pushButton_seconnecter->setEnabled(false);
        this->ui->pushButton_registration->setEnabled(false);
        this->ui->statusBar->showMessage("Connecting ...", 0);
    }
    else
		this->ui->statusBar->showMessage("Error identifier, try again", 5000);
}

void BabelWindow::on_pushButton_send_clicked()
{
	if (this->ui->textEdit_message->toPlainText().toStdString() != "")
	{
        QString tmpLogin;
        tmpLogin = this->_data.getUser().getName();
        this->tmp += tmpLogin + " -> ";
		this->tmp += this->ui->textEdit_message->toPlainText() + '\n';
		this->ui->textEdit_seeMessage->setText(tmp);
		this->ui->textEdit_message->clear();
        //emit this->onSend();
	}
	QTextCursor c = this->ui->textEdit_seeMessage->textCursor();
	c.movePosition(QTextCursor::End);
	this->ui->textEdit_seeMessage->setTextCursor(c);
}

void BabelWindow::on_btn_swap_signin_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(0);
}

void BabelWindow::on_pushButton_HangUp_clicked()
{
    //emit this->onHangUp();
}

void BabelWindow::on_pushButton_call_clicked()
{

    onCall();
}

void BabelWindow::on_pushButton_manageFriend_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(4);
}

void BabelWindow::on_pushButton_addThisFriend_clicked()
{
    //this->AddFriendRequest(this->ui->lineEdit_addThisFriend->text())
    //onUpdateListFriend();
}

void BabelWindow::on_pushButton_removeThisFriend_clicked()
{
    //this->AddFriendRequest(this->ui->lineEdit_addThisFriend->text())
    //onUpdateListFriend();
}

void BabelWindow::on_pushButton_back_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(2);
}

void    BabelWindow::onCallRequest(const QString &name, std::list<QString>listConf)
{
    QMessageBox msgBox;
    QString tmp;
    foreach (QString toto, listConf)
    {
        tmp += toto + ' ';
    }
    tmp += "and " + name;
    msgBox.setText(name + " want to call you.");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
        emit this->answerRequestCall(name, true);
    else
    {
        emit this->answerRequestCall(name, false);
        msgBox.close();
    }
    msgBox.exec();
}

void    BabelWindow::onCallDropped(QString name)
{
    onUpdateListFriend();
    QMessageBox msgBox;
    msgBox.setText("La personne ne veut pas être en relation avec vous.");
    msgBox.exec();
}

void BabelWindow::on_pushButton_clear_clicked()
{
    this->ui->textEdit_listConf->clear();
}

void BabelWindow::on_pushButton_callList_clicked()
{
    onFriendRequest(this->ui->lineEdit_addThisFriend->text());
}
