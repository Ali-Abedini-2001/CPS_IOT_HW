#include "usersManager.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

UsersManager::UsersManager(const QString &path, QObject *parent) : QObject(parent)
{
    readUsersFromFile(path);
}

void UsersManager::readUsersFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open " + path;
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    parseJsonData(jsonData, path);
}

void UsersManager::parseJsonData(const QByteArray &jsonData, const QString &path)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isArray()) {
        qDebug() << "Invalid JSON format in " + path;
        return;
    }
    QJsonArray usersArray = doc.array();
    for (const QJsonValue& userValue : usersArray) {
        if (!userValue.isObject()) {
            qDebug() << "Invalid user data in " + path;
            continue;
        }
        QJsonObject userObject = userValue.toObject();
        QString rfidTag = userObject["rfid"].toString();

        User user(rfidTag);
        usersList.append(user);
    }
}

const QVector<User>& UsersManager::getUsersList() const
{
    return usersList;
}

bool UsersManager::handleRfidReceived(const QString &rfid) {
    bool isMatch = false;
    for (const User &user : usersList) {
        if (user.checkRFIDTagMatched(rfid)) {
            isMatch = true;
            break;
        }
    }

    return isMatch;
}
