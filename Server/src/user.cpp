#include "user.h"

User::User() {
    rfidTag_ = nullptr;
}

User::User(const QString &rfidTag) : rfidTag_(rfidTag) {};

bool User::checkRFIDTagMatched(const QString &rfidTag) const{
    return rfidTag_ == rfidTag;
}
