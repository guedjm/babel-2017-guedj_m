#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QMainWindow>
#include "ClientData.h"
#include "ClientWorker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *);

private slots:
    void on_btn_connect_clicked();

signals:
    void onSignInRequest();

private:
    Ui::MainWindow  *ui;

    QThread         _workerThread;
    ClientData      _data;
    ClientWorker    *_worker;
};

#endif // MAINWINDOW_H
