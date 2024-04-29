#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H

#include <QJsonObject>
#include <QJsonDocument>

class SystemAdmin
{
public:
    SystemAdmin();
    SystemAdmin(QString username, QString password);
    QString getUsername() const;
    bool checKPassword(const QString &password) const;

private:
    QString username_;
    QString password_;
};

#endif // SYSTEMADMIN_H
