#ifndef CALLEE_H
#define CALLEE_H

#include <QString>
#include <QTcpSocket>

class Callee
{
public:
    Callee();
    Callee(Callee const &);
    Callee(QString const &, QString const &);
    ~Callee();

    QString const   &getIp() const;
    QString const   &getUsername() const;
    QTcpSocket      *getSock();

    void            setSock(QTcpSocket *);

private:
    QString     _ip;
    QString     _username;
    QTcpSocket  *_sock;
};

#endif // CALLEE_H
