#ifndef BABELWINDOW_H
#define BABELWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QKeyEvent>
#include <QtGui>
#include <QVBoxLayout>
#include <iostream>
#include <QThread>
#include "ClientData.h"
#include "ClientWorker.h"

namespace Ui {
class BabelWindow;
}

class BabelWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BabelWindow(QWidget *parent = 0);
    ~BabelWindow();
    void    displayList();
    void    hideContent();
    bool	registerServer(std::string, std::string);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:

    void onConnectionSuccess();

    void onConnectionFailed();

    void onCommandFailed(eErrorCode);

    void onUpdateListFriend();


    void onFriendRequest(QString);

    void onCallDropped(QString);

    void onCallRequest(QString const &, std::list<QString>);

    void on_listConnected_itemClicked(QListWidgetItem *item);

    void on_pushButton_seconnecter_clicked();

    void on_pushButton_send_clicked();

	void on_pushButton_registration_clicked();

	void on_pushButton_register_clicked();

    void on_btn_swap_signin_clicked();

    void on_pushButton_HangUp_clicked();

    void on_pushButton_call_clicked();

    void on_pushButton_manageFriend_clicked();

    void on_pushButton_addThisFriend_clicked();

    void on_pushButton_removeThisFriend_clicked();

    void on_pushButton_back_clicked();

    void on_pushButton_callList_clicked();

    void on_pushButton_addToConf_clicked();

signals:
    void onSignInRequest();
    void onSignUpRequest();
    void onCall();
    void onHangUp();
    void onSend();
    void onAddFriend(QString);
    void onRemoveFriend();
    void onAcceptRequestCall();
    void onDeclineRequestCall();
    void answerRequestCall(QString const &, bool);

private:
    Ui::BabelWindow *ui;
	QString tmp;
    QString _tmpCall;

    QThread         _workerThread;
    ClientData      _data;
    ClientWorker    *_worker;
};

#endif // BABELWINDOW_H
