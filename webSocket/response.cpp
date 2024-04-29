#include "response.h"

Response::Response() {}

Response::Response(const QVariant &data) : m_data(data) {}

QVariant Response::getData() const {
    return m_data;
}

void Response::setData(const QVariant &data) {
    m_data = data;
}

QJsonObject Response::toJson() const {
    QJsonObject json;
    json["data"] = m_data.toJsonObject();
    return json;
}

Response Response::fromJson(const QJsonObject &json) {
    return Response(QVariant::fromValue(json["data"]));
}
