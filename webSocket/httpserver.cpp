// httpserver.cpp
#include "httpserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

HttpServer::HttpServer(int port, EmployeesHandler* employeesHandler, QObject *parent) :
    QObject(parent), server_(new QHttpServer()), employeesHandler_(employeesHandler)
{
    server_->route("/rfidcheck",QHttpServerRequest::Method::Post, [this](const QHttpServerRequest& request) {
        return this->handleRequest(request);
    });

    start(port);
}

bool HttpServer::start(int port)
{
    return server_->listen(QHostAddress::Any, port);
}

QHttpServerResponse HttpServer::handleRequest(const QHttpServerRequest& request)
{
    // Read the JSON data from the request
    QByteArray requestData = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(requestData);
    qDebug() << "Null " << jsonDoc.isNull();
    qDebug() << "type " << jsonDoc["rfid"].toString();
    Request receivedRequest = Request::fromJson(jsonDoc.object());

    QString type = receivedRequest.getType();
    QVariant data = receivedRequest.getData();

    if (type == "RFID") {
        QJsonObject jsonData = data.toJsonObject();
        QString rfid = jsonData["rfid"].toString();
        bool permitted = employeesHandler_->handleRfidReceived(rfid);
        QJsonObject resultJsonObject;
        QHttpServerResponse::StatusCode statusCode;
        if (permitted) {
            resultJsonObject["status"] = "Succeed";
            resultJsonObject["message"] = "RFID accepted";
            resultJsonObject["data"] = NULL;
            statusCode = QHttpServerResponse::StatusCode::Ok;
        }
        else {
            resultJsonObject["status"] = "Failed";
            resultJsonObject["message"] = "RFID is invalid";
            resultJsonObject["data"] = NULL;
            statusCode = QHttpServerResponse::StatusCode::Unauthorized;
        }

        QJsonDocument resultJsonDoc(resultJsonObject);
        QByteArray resultJsonByte = resultJsonDoc.toJson();

        return QHttpServerResponse(resultJsonByte, statusCode);
    }
}
