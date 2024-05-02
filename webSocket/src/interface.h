#ifndef INTERFACE_H
#define INTERFACE_H

#include <QString>

class Interface {
public:
    virtual QString getUsersFilePath() const = 0;
    virtual QString getLoggerFilePath() const = 0;
    virtual QString getAdminsFilePath() const = 0;
    virtual int getHttpServerPort() const = 0;
    virtual int getWebSocketPort() const = 0;
};

#endif // INTERFACE_H
