#include "websocketserver.h"
#include <QDebug>

WebSocketServer::WebSocketServer(quint16 port, QObject *parent) : QObject(parent) {
    authenticated = false;
    clientSocket_ = nullptr;
    m_server = new QWebSocketServer(QStringLiteral("WebSocket Server"), QWebSocketServer::NonSecureMode, this);
    if (m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "WebSocket server listening on port" << port;
        connect(m_server, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
    } else {
        qFatal("Failed to listen on port %d", port);
    }
}

WebSocketServer::~WebSocketServer() {
    m_server->close();
}

void WebSocketServer::onNewConnection() {
    QWebSocket *socket = m_server->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);
    clientSocket_ = socket;
    qDebug() << "New client connected";
}

void WebSocketServer::processTextMessage(const QString &message) {
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Message received:" << message;
    if (socket) {
        // Deserialize received JSON string into ReceivingRequest object
        Request receivedRequest = Request::fromJson(QJsonDocument::fromJson(message.toUtf8()).object());

        // Extract type and data from received request
        QString type = receivedRequest.getType();
        QVariant data = receivedRequest.getData();

        // Process the request based on its type (just an example, you can implement your own logic here)
        if (type == "History") {
            // Do something with history data
            if (authenticated) {
                qDebug() << "Authenticated";
                emit requestLoginHistory();
            }
        } else if (type == "Authenticate") {
            // Do something with authentication data
            QJsonObject jsonData = data.toJsonObject();

            emit authenticateSystemAdmin(jsonData["username"].toString(), jsonData["password"].toString());
        } else if (type == "RFID") {
            // Do something with RFID data
            QJsonObject jsonData = data.toJsonObject();
            QString rfid = jsonData["rfid"].toString();
            emit rfidReceived(rfid);
        }

        // // Create SendingRequest object with the same type and data
        // Response sendingRequest(data);

        // // Serialize SendingRequest object to JSON
        // QJsonObject json = sendingRequest.toJson();
        // QJsonDocument doc(json);
        // QString jsonString = QString::fromUtf8(doc.toJson());

        // // Send JSON string back to the client
        // socket->sendTextMessage(jsonString);
    }
}

void WebSocketServer::handleAuthenticationResult(bool success)
{

    if (clientSocket_) {
        authenticated = success;
        qDebug() << "Authenticated=" <<
            authenticated;
        QJsonObject jsonResponse;
        jsonResponse["authenticated"] = success;

        // Convert JSON to string
        QJsonDocument jsonDoc(jsonResponse);
        QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);
        // Send authentication result to the client
        clientSocket_->sendTextMessage(jsonString);
        // clientSocket->close();
        // clientSocket->deleteLater();
        // clientSocket = nullptr;
    }
}

void WebSocketServer::handleLoginHistoryResult(const QVector<LoginHistory> &history) {
    if (clientSocket_) {
        QJsonArray jsonArray;

        for (const LoginHistory &entry : history) {
            QJsonObject jsonObj;
            jsonObj["rfidTag"] = entry.getRfidTag();
            jsonObj["dateTime"] = entry.getLoginTime().toString(Qt::ISODate);
            jsonObj["permitted"] = entry.isPermitted();

            jsonArray.append(jsonObj);
        }

        QJsonDocument jsonDoc(jsonArray);
        QString jsonResponse = jsonDoc.toJson();

        clientSocket_->sendTextMessage(jsonResponse);
    }
}

void WebSocketServer::handleRfidCheckResult(bool isMatch, const QDateTime &currentTime, const QString &rfid) {
    if (clientSocket_) {
        QJsonObject jsonObj;
        jsonObj["isMatch"] = isMatch;
        jsonObj["currentTime"] = currentTime.toString(Qt::ISODate);
        jsonObj["rfid"] = rfid;

        QJsonDocument jsonDoc(jsonObj);
        QString jsonResponse = jsonDoc.toJson();

        clientSocket_->sendTextMessage(jsonResponse);
    }
}

void WebSocketServer::socketDisconnected() {
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket) {
        authenticated = false;
        socket->deleteLater();
        qDebug() << "Client disconnected";
    }
}
