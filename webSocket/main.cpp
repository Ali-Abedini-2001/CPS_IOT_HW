#include <QCoreApplication>
#include "websocketserver.h"
#include "systemadminshandler.h"
#include "loginhistoryhandler.h"
#include "employeeshandler.h"
#include "httpserver.h"

const QString USERS_FILE_PATH = "../../data/employee_info.json";
const QString ADMINS_FILE_PATH = "../../data/admins_info.json";
const QString HISTORY_FILE_PATH = "../../data/history_info.json";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "App path : " << QCoreApplication::applicationDirPath();
    WebSocketServer server(1234);

    // WebSocketServer server(1234); // Replace 1234 with your desired port number
    EmployeesHandler employeesHandler(USERS_FILE_PATH);
    HttpServer httpServer(4321, &employeesHandler);

    QVector<Employee> emps =  employeesHandler.getEmployeesVector();

    LoginHistoryHandler historyHandler(HISTORY_FILE_PATH);
    QVector<LoginHistory> hiss = historyHandler.getLoginHistoriesVector();

    SystemAdminsHandler adminsHandler(ADMINS_FILE_PATH);

    Request request;
    request.setType("POST");
    request.setData(QJsonObject {
        {"param1", "value1"},
        {"param2", 123}
    });

    qDebug() << "App path : ";

    QObject::connect(&adminsHandler, &SystemAdminsHandler::authenticateComplete, &server, &WebSocketServer::handleAuthenticationResult);
    QObject::connect(&server, &WebSocketServer::authenticateSystemAdmin, &adminsHandler, &SystemAdminsHandler::authenticateSystemAdmin);

    QObject::connect(&server, &WebSocketServer::requestLoginHistory, &historyHandler, &LoginHistoryHandler::handleRequestLoginHistory);
    QObject::connect(&historyHandler, &LoginHistoryHandler::LoginHistoryResult, &server, &WebSocketServer::handleLoginHistoryResult);

    // QObject::connect(&server, &WebSocketServer::rfidReceived, &employeesHandler, &EmployeesHandler::handleRfidReceived);
    QObject::connect(&employeesHandler, &EmployeesHandler::rfidCheckResult, &server, &WebSocketServer::handleRfidCheckResult);
    QObject::connect(&employeesHandler, &EmployeesHandler::rfidCheckResult, &historyHandler, &LoginHistoryHandler::addLoginHistory);



    return a.exec();
}
