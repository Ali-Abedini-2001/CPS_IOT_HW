#ifndef LOGINHISTORYHANDLER_H
#define LOGINHISTORYHANDLER_H

#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "loginhistory.h"

class LoginHistoryHandler : public QObject
{
    Q_OBJECT
public:
    explicit LoginHistoryHandler(const QString &initialDataPath, QObject *parent = nullptr);

    const QVector<LoginHistory>& getLoginHistoriesVector() const;

signals:
    void LoginHistoryResult(const QVector<LoginHistory> &history);

public slots:
    void handleRequestLoginHistory();
    void addLoginHistory(bool isMatch, const QDateTime &currentTime, const QString &rfid);

private:
    QVector<LoginHistory> loginHistoryVector_;

    void readLoginHistoriesFromJson(const QString &path);
};

#endif // LOGINHISTORYHANDLER_H
