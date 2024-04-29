#ifndef REQUEST_H
#define REQUEST_H

#include <QJsonObject>
#include <QJsonDocument>

class Request
{
public:
    Request();
    Request(const QString &type, const QVariant  &data);

    QString getType() const;
    void setType(const QString &type);

    QVariant  getData() const;
    void setData(const QVariant  &data);

    QJsonObject toJson() const;
    static Request fromJson(const QJsonObject &json);

private:
    QString m_type;
    QVariant  m_data;
};

#endif // REQUEST_H
