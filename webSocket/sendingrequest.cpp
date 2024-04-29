#include "sendingrequest.h"

SendingRequest::SendingRequest() {}

SendingRequest::SendingRequest(const QString &data) : m_data(data) {}

QString SendingRequest::getData() const {
    return m_data;
}

void SendingRequest::setData(const QString &data) {
    m_data = data;
}

QJsonObject SendingRequest::toJson() const {
    QJsonObject json;
    json["data"] = m_data;
    return json;
}

SendingRequest SendingRequest::fromJson(const QJsonObject &json) {
    return SendingRequest(json["data"].toString());
}
