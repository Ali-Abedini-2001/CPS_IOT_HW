#include "httpServer.h"

HttpServer::HttpServer(int port, const QString &initialDataPath, QObject *parent) :
    QObject(parent), usersManger(UsersManager(initialDataPath)), httpServer_(new QHttpServer())
{
    httpServer_->route("/rfid",QHttpServerRequest::Method::Post, [this](const QHttpServerRequest& request) {
        return this->handleRequest(request);
    });

    httpServer_->listen(QHostAddress::Any, port);
}

QHttpServerResponse HttpServer::handleRequest(const QHttpServerRequest& request)
{
    QByteArray requestData = request.body();
    QString rfid = requestData;

    QDateTime currentDateTime = QDateTime::currentDateTime();

    QDate currentDate = currentDateTime.date();
    QTime currentTime = currentDateTime.time();

    QString dateString = currentDate.toString("yyyy-MM-dd");
    QString timeString = currentTime.toString("HH:mm:ss");

    bool isAuthorized = usersManger.handleRfidReceived(rfid);

    emit rfidMatch(isAuthorized, rfid);
    emit rfidHistory(isAuthorized, dateString, timeString, rfid);

    QByteArray result = isAuthorized ? "1" : "0";
    QHttpServerResponse::StatusCode statusCode =
        isAuthorized ?
        QHttpServerResponse::StatusCode::Ok :
        QHttpServerResponse::StatusCode::Unauthorized;

    return QHttpServerResponse(result, statusCode);
}
