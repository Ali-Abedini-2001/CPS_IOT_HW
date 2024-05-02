#ifndef USERSMANAGER_H
#define USERSMANAGER_H

#include <QObject>
#include <QVector>
#include "user.h"

class UsersManager : public QObject
{
    Q_OBJECT
public:
    explicit UsersManager(const QString &initialDataPath, QObject *parent = nullptr);

    const QVector<User>& getUsersList() const;
    bool handleRfidReceived(const QString &rfid);

private:
    QVector<User> usersList;
    void readUsersFromFile(const QString &path);
    void parseJsonData(const QByteArray &jsonData, const QString &path);
};

#endif // USERSMANAGER_H
