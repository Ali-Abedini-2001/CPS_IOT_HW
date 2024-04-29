#include "systemadminshandler.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

SystemAdminsHandler::SystemAdminsHandler(const QString &initialDataPath, QObject *parent) : QObject(parent)
{
    readAdminsFromJson(initialDataPath);
}

const QMap<QString, SystemAdmin>& SystemAdminsHandler::getAdminsMap() const
{
    return adminsMap_;
}

void SystemAdminsHandler::readAdminsFromJson(const QString &path)
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

    QJsonArray adminsArray = doc.array();
    for (const QJsonValue& adminValue : adminsArray) {
        if (!adminValue.isObject()) {
            qDebug() << "Invalid admin data in " + path;
            continue;
        }

        QJsonObject adminObject = adminValue.toObject();
        QString username = adminObject["username_"].toString();
        QString password = adminObject["password_"].toString();

        SystemAdmin admin(username, password);
        adminsMap_[username] = admin;
    }
}

void SystemAdminsHandler::authenticateSystemAdmin(const QString& username, const QString& password)
{
    bool success = false;
    // Check if username and password match stored admin credentials
    if (adminsMap_.contains(username)) {
        SystemAdmin admin = adminsMap_[username];
        success = admin.checKPassword(password);
    }

    // Emit signal indicating authentication result
    emit authenticateComplete(success);
}


