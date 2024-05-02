#include <QCoreApplication>
#include "logger.h"
#include "webSocketServer.h"
#include "httpServer.h"
#include "defines.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Interface *myDefines = new Defines;
    HttpServer myHttpServer = HttpServer(myDefines->getHttpServerPort(), myDefines->getUsersFilePath());
    qDebug() << "HTTP Server running, port: " << myDefines->getHttpServerPort();

    WebSocketServer myServer;
    if (!myServer.listen(QHostAddress::LocalHost, myDefines->getWebSocketPort())) {
        qDebug() << "WebSocket Server could not start!";
        return 1;
    }
    qDebug() << "WebSocket Server running, port: " << myServer.serverPort();

    Logger logger(myDefines->getLoggerFilePath());

    QObject::connect(&myHttpServer, &HttpServer::rfidMatch, &myServer, &WebSocketServer::sendNewUserDataToAdmin);
    QObject::connect(&myHttpServer, &HttpServer::rfidHistory, &logger, &Logger::addLoginHistory);

    QObject::connect(&myServer, &WebSocketServer::requestLoginHistory, &logger, &Logger::handleRequestLoginHistory);
    QObject::connect(&logger, &Logger::loggedInUsersUpdated, &myServer, &WebSocketServer::sendLoginHistoryResult);

    delete myDefines;

    return a.exec();
}
