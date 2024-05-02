#ifndef USER_H
#define USER_H

#include <QJsonObject>
#include <QJsonDocument>

class User
{
public:
    User();
    User(const QString &rfidTag);
    bool checkRFIDTagMatched(const QString &rfidTag) const;
private:
    QString rfidTag_;
};

#endif // USER_H
