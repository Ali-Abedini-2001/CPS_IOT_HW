#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QVector>
#include <QString>

class Logger : public QObject
{
    Q_OBJECT

public:
    struct LoggedInUserHistory{
        QString username;
        QString date;
        QString time;
        bool permitted;
    };
    explicit Logger(const QString& initialDataPath, QObject* parent = nullptr);

    const QVector<Logger::LoggedInUserHistory>& getLoginHistories() const;

signals:
    void loggedInUsersUpdated(const QVector<Logger::LoggedInUserHistory>& history); // Changed signal name to follow naming convention

public slots:
    void handleRequestLoginHistory();
    void addLoginHistory(bool isMatch, const QString& date, const QString& currentTime, const QString& rfid);

private:
    QVector<LoggedInUserHistory> loggedInUserInfo;
    void readLoginHistoriesFromJson(const QString& path);
    void parseJsonData(const QByteArray& jsonData);
    void parseLoggedInUser(const QJsonObject& loggedInUserObj);
};

#endif // LOGGER_H
