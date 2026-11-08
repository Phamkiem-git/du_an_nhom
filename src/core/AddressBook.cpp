// ============================================================
// File      : src/core/AddressBook.cpp
// Phụ trách : THÀNH VIÊN A là chủ file | Review: thành viên B
// >>> FILE NỐI HAI PHẦN: gọi HashTable (A) và NameTree (B) <<<
// ============================================================
#include <iostream>
#include "core/AddressBook.h"
#include "core/PhoneList.h"
#include "core/Validator.h"
#include "core/StringUtils.h"

using namespace std;

AddressBook::AddressBook() {
    // phoneIndex và nameIndex tự khởi tạo bằng constructor mặc định.
}

AddressBook::~AddressBook() {
    nameIndex.destroyAll();   // delete mọi Contact (mỗi Contact nằm đúng một lần trong cây)
    // phoneIndex tự giải phóng mảng ô của nó; nó không sở hữu Contact.
}

bool AddressBook::addContact(Contact* c) {
    // TODO (A): theo thứ tự
    //   1. c != nullptr; isValidName(c->name) [B]; isValidGroup(c->group); mỗi số isValidPhone().
    //   2. Mỗi số trong c->phones: nếu phoneIndex.find(số) != nullptr -> trùng số -> return false.
    //   3. Nếu nameIndex.find(c->name) != nullptr -> trùng tên -> return false.
    //   4. CHỈ KHI cả hai kiểm tra đều qua: chèn từng số vào phoneIndex, rồi nameIndex.insert(c).
    //      (Kiểm tra hết trước rồi mới chèn, để không phải hoàn tác khi chèn dở.)
    return false;
}

bool AddressBook::removeContact(const string& name) {
    // TODO (A):
    //   1. Contact* c = nameIndex.find(name); nếu nullptr -> return false.
    //   2. Với mỗi số trong c->phones: phoneIndex.remove(số).
    //   3. nameIndex.remove(name);   // chỉ gỡ nút khỏi cây, KHÔNG delete Contact
    //   4. freeContact(c);           // giải phóng đúng một lần, ở đây
    return false;
}

bool AddressBook::addPhoneTo(const string& name, const string& phone) {
    // TODO (A): tìm Contact theo tên; isValidPhone; phoneIndex.find(phone) phải là nullptr;
    //   rồi addPhone(c, phone) và phoneIndex.insert(phone, c).
    return false;
}

bool AddressBook::removePhoneFrom(const string& name, const string& phone) {
    // TODO (A): tìm Contact; hasPhone(c, phone); phoneIndex.remove(phone); removePhone(c, phone).
    return false;
}

bool AddressBook::mergeInto(Contact* existing, Contact* extra) {
    // TODO (A): với mỗi số của extra: nếu số đó đã thuộc liên hệ KHÁC thì từ chối (return false);
    //   nếu không thì addPhone(existing, số) và phoneIndex.insert(số, existing).
    //   Nếu existing->email rỗng thì lấy email của extra. Cuối cùng freeContact(extra).
    return false;
}

// ---- Các hàm chuyển tiếp đơn giản (đã viết sẵn để làm mẫu cách hai phần nối nhau) ----

Contact* AddressBook::findByPhone(const string& phone) {
    return phoneIndex.find(phone);        // phần A
}

Contact* AddressBook::findByName(const string& name) {
    return nameIndex.find(name);          // phần B
}

void AddressBook::printAll() {
    nameIndex.printInOrder();             // phần B
}

void AddressBook::printByGroup(int group) {
    nameIndex.printByGroup(group);        // phần B
}

void AddressBook::searchPrefix(const string& prefix) {
    nameIndex.searchPrefix(prefix);       // phần B
}

void AddressBook::printStats() {
    // TODO (A): in ra: số liên hệ, số khóa trong bảng băm, capacity, loadFactor(),
    //   avgProbes(), và chiều cao cây tên (nameIndex.height()).
    cout << "(printStats chưa cài đặt)\n";
}
