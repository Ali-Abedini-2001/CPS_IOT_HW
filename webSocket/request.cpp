#include "request.h"

Request::Request() {}


Request::Request(const QString &type, const QVariant  &data) : m_type(type), m_data(data) {}

QString Request::getType() const {
    return m_type;
}

void Request::setType(const QString &type) {
    m_type = type;
}

QVariant  Request::getData() const {
    return m_data;
}

void Request::setData(const QVariant  &data) {
    m_data = data;
}

QJsonObject Request::toJson() const {
    QJsonObject json;
    json["type"] = m_type;
    json["data"] = QJsonValue::fromVariant(m_data);
    return json;
}

Request Request::fromJson(const QJsonObject &json) {
    return Request(json["type"].toString(), json["data"].toVariant());
}
