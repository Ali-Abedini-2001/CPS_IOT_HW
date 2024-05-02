#ifndef DEFINES_H
#define DEFINES_H

#include <QCoreApplication>
#include "interface.h"

class Defines : public Interface {
public:
    Defines();

    QString getUsersFilePath() const override { return "../../../database/users.json"; }
    QString getAdminsFilePath() const override { return "../../../database/admins.json"; }
    QString getLoggerFilePath() const override { return "../../../database/logger.json"; }
    int getHttpServerPort() const override { return 80; }
    int getWebSocketPort() const override { return 5050; }
};

#endif // DEFINES_H
