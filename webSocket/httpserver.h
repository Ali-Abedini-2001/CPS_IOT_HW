#ifndef HTTPSERVER_H
#define HTTPSERVER_H


#include <QObject>
#include <QHttpServer>
#include <QVariant>
#include "employeeshandler.h"
#include "request.h"

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(int port, EmployeesHandler* employeesHandler, QObject *parent = nullptr);

    // Start the server and listen on the specified port
    bool start(int port);

    QHttpServerResponse handleRequest(const QHttpServerRequest& request);

signals:


private:
    QHttpServer* server_;
    EmployeesHandler* employeesHandler_;
};

#endif // HTTPSERVER_H
