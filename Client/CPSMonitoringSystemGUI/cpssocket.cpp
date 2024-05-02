// cpssocket.cpp
#include "cpssocket.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

CPSSocket::CPSSocket(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
}

QTcpSocket* CPSSocket::getSocket() const {
    return socket;
}
void CPSSocket::collectingNewData() {
    // Read the response data from the socket
    QByteArray responseData = socket->readAll();

    // Parse the response data as JSON
    QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData);

    // Check if the JSON document is valid
    if (!jsonDocument.isNull() && jsonDocument.isObject()) {
        // Extract the JSON object from the document
        QJsonObject jsonObject = jsonDocument.object();

        // Check the type of the received data and take appropriate action
        if (jsonObject.contains("type")) {
            // Check if the type is for user data
            if (jsonObject["type"].toString() == "user") {
                // Process the new user data
                ExtractNewUserData(jsonObject);
            }
            // Check if the type is for history data
            else if (jsonObject["type"].toString() == "history") {
                // Emit a signal for new history data
                emit newHistory(jsonObject);
            }
            // If the type field is not recognized
            else {
                QTextStream(stdout) << "Invalid type field in JSON data" << Qt::endl;
            }
        }
        // If the type field is missing
        else {
            QTextStream(stdout) << "No type field in JSON data" << Qt::endl;
        }
    }
    // If the JSON data cannot be parsed
    else {
        QTextStream(stdout) << "Failed to parse JSON data" << Qt::endl;
    }
}
void CPSSocket::ExtractNewUserData(const QJsonObject &jsonObject) {
    QString username = jsonObject["username"].toString();
    QString date = jsonObject["date"].toString();
    QString time = jsonObject["time"].toString();
    emit newUser(username, date, time);
}
void CPSSocket::connectToServer(const QString &serverAddr, const QString &user, const QString &pass) {
    // Connect to the server
    socket->connectToHost(serverAddr, 5050);

    // Check if connection is established
    if (socket->waitForConnected()) {
        // If connected, create the message to send
        QString msg = user + ":" + pass;

        // Log the successful connection and message sending
        QTextStream(stdout) << "Connected to the server successfully. Sending message: " << msg << Qt::endl;

        // Send the message
        socket->write(msg.toUtf8());

        // Wait for response
        if (socket->waitForReadyRead()) {
            // If response received, read the response
            QByteArray response = socket->readAll();
            QTextStream(stdout) << "Response from server received:" << response << Qt::endl;

            // Check if response is valid
            if (response == "1") {
                // If response is valid, emit signal and connect signals and slots
                emit connectionChanged(false);
                QObject::connect(socket, &QTcpSocket::readyRead, this, &CPSSocket::collectingNewData);
            }
        } else {
            // If timeout while waiting for server response
            QTextStream(stdout) << "Timeout occurred while waiting for the server response" << Qt::endl;
        }
    } else {
        // If failed to connect to the server
        QTextStream(stdout) << "Failed to establish connection to the server:" << socket->errorString() << Qt::endl;
    }
}
