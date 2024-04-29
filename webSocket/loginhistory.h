#ifndef LOGINHISTORY_H
#define LOGINHISTORY_H

#include <QObject>
#include <QDateTime>
#include <QString>


class LoginHistory
{
public:
    LoginHistory();
    LoginHistory(QString rfidTag, QDateTime loginTime, bool permited = false);

    QString getRfidTag() const;
    QDateTime getLoginTime() const;
    bool isPermitted() const;

private:
    QString rfidTag_;
    QDateTime loginTime_;
    bool permited_;
};

#endif // LOGINHISTORY_H
