#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include "logger.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>

class WebSocketServer : public QTcpServer {
    Q_OBJECT

public:
    WebSocketServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
    bool isValidRFID(const QString &rfid);
    void processData(QByteArray &data, QTcpSocket *clientSocket);
    bool isValidUser(const QString &username, const QString &password);
    QByteArray retrieveHistoryData();
    void matchUser(const QByteArray &data, QTcpSocket *clientSocket);

signals:
    void requestLoginHistory();

public slots:
    void sendNewUserDataToAdmin(bool isMatch, const QString &rfid);
    void sendJsonDataToAdmin(const QByteArray &jsonData);
    QByteArray convertJsonDocument(const QJsonObject &jsonObject);
    QJsonObject createMessageObject(const QString &rfid, const QString &dateString, const QString &timeString);
    void createDateTimeStrings(QString &dateString, QString &timeString);

    void sendLoginHistoryResult(const QVector<Logger::LoggedInUserHistory> &loginHistories);
    QJsonObject createJsonObject(const QVector<Logger::LoggedInUserHistory> &loginHistories);
    void sendJsonData(const QByteArray &jsonData);

private:
    QStringList hardcodedRFIDs;
    QTcpSocket* clientSocketpointer; // Store pointer to client socket
};

#endif // WEBSOCKETSERVER_H
