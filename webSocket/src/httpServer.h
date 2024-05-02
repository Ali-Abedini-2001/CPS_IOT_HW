#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QCoreApplication>
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonObject>
#include "usersManager.h"

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(int port, const QString &initialDataPath, QObject *parent = nullptr);

signals:
    void rfidMatch(bool isMatch, const QString &rfid);
    void rfidHistory(bool isMatch, const QString Date, const QString &currentTime, const QString &rfid);

private:
    QHttpServerResponse handleRequest(const QHttpServerRequest &request);
    QHttpServer* httpServer_;
    UsersManager usersManger;
};

#endif // HTTPSERVER_H
