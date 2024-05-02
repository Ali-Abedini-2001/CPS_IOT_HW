#include "webSocketServer.h"

WebSocketServer::WebSocketServer(QObject *parent) : QTcpServer(parent) {
    hardcodedRFIDs << "RFID1" << "RFID2" << "RFID3";
}

void WebSocketServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *clientSocket = new QTcpSocket(this);
    if (!clientSocket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Error setting socket descriptor";
        return;
    }

    qDebug() << "New connection from:" << clientSocket->peerAddress().toString();

    connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket]() {

        QByteArray data = clientSocket->readAll();
        qDebug() << "Received:" << data;
        processData(data, clientSocket);
        clientSocketpointer = clientSocket;

    });

    connect(clientSocket, &QTcpSocket::disconnected, this, [clientSocket]() {
        qDebug() << "Connection closed for:" << clientSocket->peerAddress().toString();
        clientSocket->deleteLater();
    });
}

bool WebSocketServer::isValidRFID(const QString &rfid) {
    return hardcodedRFIDs.contains(rfid);
}

void WebSocketServer::processData(QByteArray &data, QTcpSocket *clientSocket) {
    qDebug() << "in  SocketServer::processData";
    if(data == "historyRequest"){
        qDebug() << "i am still in historyRequest.";
        emit requestLoginHistory();
        qDebug() << "i have sent message in historyRequest.";
    }
    else{
        matchUser(data, clientSocket);
    }
}

void WebSocketServer::matchUser(const QByteArray &data, QTcpSocket *clientSocket){
    QList<QByteArray> parts = data.split(':');

    if (parts.size() >= 2) {
        QString username = QString::fromUtf8(parts[0]);
        QString password = QString::fromUtf8(parts[1]);
        bool isMatch = isValidUser(username, password);
        if (isMatch) {
            clientSocket->write("1");
        } else {
            clientSocket->write("0");
            clientSocket->close();
        }
        qDebug() << "username: " << username << ", password: " << password;
    } else {
        qDebug() << "Encountered an error setting the socket descriptor.";
    }
}

bool WebSocketServer::isValidUser(const QString &username, const QString &password) {
    return (username == "ali" && password == "1111");
}

void WebSocketServer::sendNewUserDataToAdmin(bool isMatch, const QString &rfid) {
    QString dateString, timeString;
    createDateTimeStrings(dateString, timeString);

    QJsonObject messageObj = createMessageObject(rfid, dateString, timeString);
    if (!messageObj.isEmpty()) {
        QByteArray jsonData = convertJsonDocument(messageObj);
        sendJsonDataToAdmin(jsonData);
    }
}

void WebSocketServer::createDateTimeStrings(QString &dateString, QString &timeString) {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDate currentDate = currentDateTime.date();
    QTime currentTime = currentDateTime.time();
    dateString = currentDate.toString("yyyy-MM-dd");
    timeString = currentTime.toString("HH:mm:ss");
}

QJsonObject WebSocketServer::createMessageObject(const QString &rfid, const QString &dateString, const QString &timeString) {
    QJsonObject messageObj;
    messageObj["type"] = "user";
    messageObj["username"] = rfid;
    messageObj["date"] = dateString;
    messageObj["time"] = timeString;
    return messageObj;
}

void WebSocketServer::sendJsonDataToAdmin(const QByteArray &jsonData) {
    qint64 bytesWritten = clientSocketpointer->write(jsonData);
    if (bytesWritten == -1) {
        qDebug() << "Failed to write data to socket:" << clientSocketpointer->errorString();
    } else {
        qDebug() << "WebSocket message sent to client:" << jsonData;
    }
}

void WebSocketServer::sendLoginHistoryResult(const QVector<Logger::LoggedInUserHistory> &loginHistories){
    if (this->clientSocketpointer) {
        QJsonObject mainObj = createJsonObject(loginHistories);
        if (!mainObj.isEmpty()) {
            QByteArray jsonData = convertJsonDocument(mainObj);
            sendJsonData(jsonData);
        }
    }
}

QJsonObject WebSocketServer::createJsonObject(const QVector<Logger::LoggedInUserHistory> &loginHistories) {
    QJsonArray dataArray;
    for (const Logger::LoggedInUserHistory &entry : loginHistories) {
        QJsonObject obj;
        obj["username"] = entry.username;
        obj["date"] = entry.date;
        obj["time"] = entry.time;
        obj["permitted"] = entry.permitted;
        dataArray.append(obj);
    }
    QJsonObject mainObj;
    mainObj["type"] = "history";
    mainObj["data"] = dataArray;
    return mainObj;
}

QByteArray WebSocketServer::convertJsonDocument(const QJsonObject &jsonObject) {
    QJsonDocument jsonDocument(jsonObject);
    return jsonDocument.toJson();
}

void WebSocketServer::sendJsonData(const QByteArray &jsonData) {
    clientSocketpointer->write(jsonData);
}

