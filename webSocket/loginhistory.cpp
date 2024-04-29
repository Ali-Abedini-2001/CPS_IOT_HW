#include "loginhistory.h"

LoginHistory::LoginHistory() {}

LoginHistory::LoginHistory(QString rfidTag, QDateTime loginTime, bool permited) : rfidTag_(rfidTag), loginTime_(loginTime), permited_(permited){}

QString LoginHistory::getRfidTag() const
{
    return rfidTag_;
}

QDateTime LoginHistory::getLoginTime() const
{
    return loginTime_;
};

bool LoginHistory::isPermitted() const
{
    return permited_;
};
