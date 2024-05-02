#include "logger.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Logger::Logger(const QString& initialDataPath, QObject* parent)
    : QObject(parent)
{
    readLoginHistoriesFromJson(initialDataPath);
}

const QVector<Logger::LoggedInUserHistory>& Logger::getLoginHistories() const
{
    return loggedInUserInfo;
}

void Logger::readLoginHistoriesFromJson(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Failed to open %s", qPrintable(path));
        return;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    parseJsonData(jsonData);
}

void Logger::parseJsonData(const QByteArray& jsonData)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning("Invalid JSON format: %s", qPrintable(error.errorString()));
        return;
    }
    if (!doc.isArray()) {
        qWarning("Root of JSON file is not an array");
        return;
    }
    loggedInUserInfo.clear();
    const QJsonArray infoArray = doc.array();
    for (const QJsonValue& val : infoArray) {
        if (!val.isObject()) {
            qWarning("Invalid login history data");
            continue;
        }
        parseLoggedInUser(val.toObject());
    }
}

void Logger::parseLoggedInUser(const QJsonObject& loggedInUserObj)
{
    LoggedInUserHistory entry;
    entry.username = loggedInUserObj["username"].toString();
    entry.date = loggedInUserObj["date"].toString();
    entry.time = loggedInUserObj["time"].toString();
    entry.permitted = loggedInUserObj["permited_"].toBool();
    loggedInUserInfo.append(entry);
}

void Logger::handleRequestLoginHistory()
{
    emit loggedInUsersUpdated(loggedInUserInfo);
}

void Logger::addLoginHistory(bool isMatch, const QString& date, const QString& currentTime, const QString& rfid)
{
    LoggedInUserHistory newEntry = {rfid, date, currentTime, isMatch};
    loggedInUserInfo.append(newEntry);
    qInfo("New history: %ls %ls %d", qUtf16Printable(newEntry.username), qUtf16Printable(newEntry.time), newEntry.permitted);
}
