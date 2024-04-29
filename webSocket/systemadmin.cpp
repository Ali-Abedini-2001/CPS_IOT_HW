#include "systemadmin.h"

SystemAdmin::SystemAdmin() {}

SystemAdmin::SystemAdmin(QString username, QString password)
{
    username_ = username;
    password_ = password;
}

QString SystemAdmin::getUsername() const
{
    return username_;
}

bool SystemAdmin::checKPassword(const QString &password) const
{
    return password_ == password;
}

