#ifndef SYSTEMADMINSHANDLER_H
#define SYSTEMADMINSHANDLER_H

#include <QObject>
#include <QMap>
#include "systemadmin.h"

class SystemAdminsHandler : public QObject
{
    Q_OBJECT
public:
    explicit SystemAdminsHandler(const QString &initialDataPath, QObject *parent = nullptr);

    const QMap<QString, SystemAdmin>& getAdminsMap() const;

signals:
    void authenticateComplete(bool success);

public slots:
    void authenticateSystemAdmin(const QString& username, const QString& password);

private:
    QMap<QString, SystemAdmin> adminsMap_;

    void readAdminsFromJson(const QString &path);
};

#endif // SYSTEMADMINSHANDLER_H
