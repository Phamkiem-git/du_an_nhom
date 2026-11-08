// ============================================================
// File      : src/core/Contact.cpp
// Phụ trách : CHUNG (A + B). File nhỏ, làm cùng nhau để cả hai hiểu rõ.
// ============================================================
#include "core/Contact.h"
#include "core/PhoneList.h"   // freePhones() là của A

Contact* createContact(const string& name, const string& email, int group) {
    Contact* c = new Contact;
    c->name = name;
    c->email = email;
    c->group = group;
    c->phones = nullptr;
    return c;
}

void freeContact(Contact* c) {
    if (c == nullptr) return;
    freePhones(c);   // [A] giải phóng danh sách số điện thoại trước
    delete c;        // rồi mới giải phóng chính Contact
}
