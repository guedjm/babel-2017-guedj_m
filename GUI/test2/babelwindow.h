#ifndef BABELWINDOW_H
#define BABELWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QKeyEvent>
#include <QtGui>
#include <QVBoxLayout>
#include <iostream>

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
    bool	connectServer(std::string, std::string, std::string, std::string);

private slots:

    void on_listConnected_itemClicked(QListWidgetItem *item);

    void on_pushButton_seconnecter_clicked();

    void on_pushButton_send_clicked();

	void on_pushButton_begin_clicked();

	void on_pushButton_end_clicked();

	void on_pushButton_registration_clicked();

	void on_pushButton_register_clicked();

private:
    Ui::BabelWindow *ui;
	QString tmp;
};

#endif // BABELWINDOW_H
