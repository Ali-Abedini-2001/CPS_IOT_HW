#ifndef EMPLOYEESHANDLER_H
#define EMPLOYEESHANDLER_H

#include <QObject>
#include <QVector>
#include "employee.h"

class EmployeesHandler : public QObject
{
    Q_OBJECT
public:
    explicit EmployeesHandler(const QString &initialDataPath, QObject *parent = nullptr);

    const QVector<Employee>& getEmployeesVector() const;
    bool handleRfidReceived(const QString &rfid);



signals:
    void rfidCheckResult(bool isMatch, const QDateTime &currentTime, const QString &rfid);



private:
    QVector<Employee> employeesVector_;

    void readEmployeesFromJson(const QString &path);
};

#endif // EMPLOYEESHANDLER_H
