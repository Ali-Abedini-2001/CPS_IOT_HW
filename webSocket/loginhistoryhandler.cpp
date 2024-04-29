#include "loginhistoryhandler.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

LoginHistoryHandler::LoginHistoryHandler(const QString &initialDataPath, QObject *parent) : QObject(parent)
{
    readLoginHistoriesFromJson(initialDataPath);
}

const QVector<LoginHistory>& LoginHistoryHandler::getLoginHistoriesVector() const
{
    return loginHistoryVector_;
}

void LoginHistoryHandler::readLoginHistoriesFromJson(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open " + path;
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isArray()) {
        qDebug() << "Invalid JSON format in " + path;
        return;
    }

    QJsonArray loginHistoryArray = doc.array();
    for (const QJsonValue& loginHistoryValue : loginHistoryArray) {
        if (!loginHistoryValue.isObject()) {
            qDebug() << "Invalid loginHistory data in " + path;
            continue;
        }

        QJsonObject loginHistoryObject = loginHistoryValue.toObject();
        QString rfidTag = loginHistoryObject["rfidTag_"].toString();
        QDateTime loginTime = QDateTime::fromString(loginHistoryObject["loginTime_"].toString(), Qt::ISODate);
        bool permitted = loginHistoryObject["permited_"].toBool();

        LoginHistory loginHistory(rfidTag, loginTime, permitted);
        loginHistoryVector_.append(loginHistory);
    }
}

void LoginHistoryHandler::handleRequestLoginHistory() {
    // Process the request for login history
    emit LoginHistoryResult(loginHistoryVector_);
}

void LoginHistoryHandler::addLoginHistory(bool isMatch, const QDateTime &currentTime, const QString &rfid) {
    LoginHistory newHistory(rfid, currentTime, isMatch); // Assuming true for permitted, otherwise modify as needed
    loginHistoryVector_.append(newHistory);
    qDebug() << "new History " << newHistory.getRfidTag() << " " << newHistory.getLoginTime() << " " << newHistory.isPermitted();
}
