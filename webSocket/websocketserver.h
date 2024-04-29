#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H


#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QJsonArray>
#include "loginhistory.h"

#include "request.h"
#include "response.h"

class WebSocketServer : public QObject {
    Q_OBJECT
public:
    WebSocketServer(quint16 port, QObject *parent = Q_NULLPTR);
    ~WebSocketServer();

signals:
    void authenticateSystemAdmin(const QString& username, const QString& password);
    void requestLoginHistory();
    void rfidReceived(const QString &rfid);

private slots:
    void onNewConnection();
    void processTextMessage(const QString &message);
    void socketDisconnected();

public slots:
    void handleAuthenticationResult(bool success);
    void handleLoginHistoryResult(const QVector<LoginHistory> &history);
    void handleRfidCheckResult(bool isMatch, const QDateTime &currentTime, const QString &rfid);

private:
    QWebSocketServer *m_server;
    QWebSocket * clientSocket_;
    bool authenticated;
};

#endif // WEBSOCKETSERVER_H
