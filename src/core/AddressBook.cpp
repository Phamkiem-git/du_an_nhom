// ============================================================
// File      : src/core/AddressBook.cpp
// Phụ trách : THÀNH VIÊN A là chủ file | Review: thành viên B
// >>> FILE NỐI HAI PHẦN: gọi HashTable (A) và NameTree (B) <<<
// ============================================================
#include <iostream>
#include <iomanip>
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

// ---- Hàm trợ giúp (chỉ dùng trong file này) ----
// c có đủ điều kiện để đưa vào danh bạ không? Kiểm tra: tên, email, nhóm,
// có ít nhất một số, mỗi số hợp lệ, và không có số nào bị lặp ngay trong c.
// (Chưa kiểm tra trùng với liên hệ khác: việc đó hỏi bảng băm và cây.)
static bool isContactValid(const Contact* c) {
    if (c == nullptr) return false;
    if (!isValidName(c->name)) return false;
    if (!isValidEmail(c->email)) return false;
    if (!isValidGroup(c->group)) return false;
    if (c->phones == nullptr) return false;           // phải có ít nhất một số

    for (const PhoneNode* p = c->phones; p != nullptr; p = p->next) {
        if (!isValidPhone(p->number)) return false;
        for (const PhoneNode* q = p->next; q != nullptr; q = q->next) {
            if (q->number == p->number) return false; // một số xuất hiện hai lần trong c
        }
    }
    return true;
}

// Quy ước sở hữu: addContact trả false thì AddressBook CHƯA nhận c (người gọi tự freeContact).
bool AddressBook::addContact(Contact* c) {
    // 1. Dữ liệu của c hợp lệ.
    if (!isContactValid(c)) return false;

    // 2. Không số nào của c đã có trong bảng băm.
    for (const PhoneNode* p = c->phones; p != nullptr; p = p->next) {
        if (phoneIndex.find(p->number) != nullptr) return false;
    }

    // 3. Tên chưa có trong cây.
    if (nameIndex.find(c->name) != nullptr) return false;

    // 4. Mọi kiểm tra đã qua: bây giờ mới chèn vào cả hai cấu trúc.
    //    Không cần hoàn tác vì ở đây chèn không thể thất bại (đã chắc chắn không trùng).
    for (const PhoneNode* p = c->phones; p != nullptr; p = p->next) {
        phoneIndex.insert(p->number, c);
    }
    nameIndex.insert(c);
    return true;
}

bool AddressBook::removeContact(const string& name) {
    Contact* c = nameIndex.find(name);
    if (c == nullptr) return false;

    string key = c->name;     // chép tên ra trước, vì cuối hàm c sẽ bị giải phóng

    // Gỡ mọi số khỏi bảng băm.
    for (const PhoneNode* p = c->phones; p != nullptr; p = p->next) {
        phoneIndex.remove(p->number);
    }
    // Gỡ nút khỏi cây (không delete Contact).
    nameIndex.remove(key);
    // Giải phóng Contact đúng một lần, ở đây.
    freeContact(c);
    return true;
}

bool AddressBook::addPhoneTo(const string& name, const string& phone) {
    Contact* c = nameIndex.find(name);
    if (c == nullptr) return false;
    if (!isValidPhone(phone)) return false;

    // insert trả false nếu số đã có trong bảng (thuộc bất kỳ liên hệ nào), nên không cần find riêng.
    if (!phoneIndex.insert(phone, c)) return false;
    addPhone(c, phone);
    return true;
}

bool AddressBook::removePhoneFrom(const string& name, const string& phone) {
    Contact* c = nameIndex.find(name);
    if (c == nullptr) return false;
    if (!hasPhone(c, phone)) return false;
    if (countPhones(c) <= 1) return false;   // mỗi liên hệ phải còn ít nhất một số

    phoneIndex.remove(phone);
    removePhone(c, phone);
    return true;
}

// Quy ước: extra CHƯA nằm trong bảng băm hay cây. Thành công thì mergeInto đã free extra;
// thất bại thì chưa free (Menu sẽ tự free), và existing không bị thay đổi gì.
bool AddressBook::mergeInto(Contact* existing, Contact* extra) {
    if (existing == nullptr || extra == nullptr || existing == extra) return false;

    // Pha 1: chỉ KIỂM TRA, chưa sửa gì. Số của extra không được thuộc về liên hệ KHÁC.
    // (Số đã thuộc chính existing thì không sao, sẽ được bỏ qua ở pha 2.)
    for (const PhoneNode* p = extra->phones; p != nullptr; p = p->next) {
        if (!isValidPhone(p->number)) return false;
        Contact* owner = phoneIndex.find(p->number);
        if (owner != nullptr && owner != existing) return false;
    }

    // Pha 2: chuyển số sang existing.
    for (const PhoneNode* p = extra->phones; p != nullptr; p = p->next) {
        if (!hasPhone(existing, p->number)) {
            phoneIndex.insert(p->number, existing);
            addPhone(existing, p->number);
        }
    }
    if (existing->email.empty()) existing->email = extra->email;

    freeContact(extra);
    return true;
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
    cout << "\n===== THONG KE =====\n";
    cout << "So lien he (nut cay)       : " << size() << "\n";
    cout << "So khoa trong bang bam     : " << phoneIndex.size() << "\n";
    cout << "Kich thuoc bang bam (cap)  : " << phoneIndex.capacity() << "\n";
    cout << fixed << setprecision(3);
    cout << "He so tai                  : " << phoneIndex.loadFactor() << "\n";
    cout << "So lan do trung binh       : " << phoneIndex.avgProbes() << "\n";
    cout.unsetf(ios::floatfield);
    cout.precision(6);
    cout << "Chieu cao cay ten          : " << nameIndex.height() << "\n";
}