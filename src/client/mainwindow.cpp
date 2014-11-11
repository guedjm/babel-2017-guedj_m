#include "Mainwindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->_worker;
}

void        MainWindow::showEvent(QShowEvent *)
{
    this->_worker = new ClientWorker(this->_data, this);
    this->_worker->moveToThread(&this->_workerThread);
    connect(this, SIGNAL(onSignInRequest()), this->_worker, SLOT(onSignInRequest()));
    this->_workerThread.start();
}

void MainWindow::on_btn_connect_clicked()
{
    this->_data.lockUser();
    User &user = this->_data.getUser();
    user.setIdent(this->ui->txt_user->text(), this->ui->txt_pwd->text());
    this->_data.unlockUser();
    this->_data.lockServer();
    this->_data.setServerIp(this->ui->txt_ip->text());
    this->_data.unlockServer();
    emit this->onSignInRequest();
}
