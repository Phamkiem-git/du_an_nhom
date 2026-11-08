// ============================================================
// File      : src/core/Contact.h
// Phụ trách : CHUNG (A + B ngồi cùng nhau làm, rồi commit lên main trước)
// Vai trò   : Struct dùng chung. Mọi file khác đều include file này.
// Lưu ý     : Muốn đổi struct phải báo người kia, vì hai phần đều dựa vào nó.
//
// QUY ƯỚC SỞ HỮU BỘ NHỚ (cả hai phải nhớ):
//   - Mỗi Contact được cấp phát (new) ĐÚNG MỘT LẦN, bằng createContact().
//   - Bảng băm (A) và cây tên (B) chỉ GIỮ CON TRỎ tới Contact, không tự delete.
//   - Contact chỉ được giải phóng bằng freeContact(), do AddressBook gọi.
// ============================================================
#ifndef CONTACT_H
#define CONTACT_H

#include <string>
using std::string;

// Một số điện thoại, nằm trong danh sách liên kết đơn của một liên hệ.
struct PhoneNode {
    string number;
    PhoneNode* next;
};

// Mã nhóm liên hệ
const int GROUP_FAMILY = 0;   // gia đình
const int GROUP_FRIEND = 1;   // bạn bè
const int GROUP_WORK   = 2;   // công việc

struct Contact {
    string name;          // họ tên (khóa của cây BST - phần B)
    string email;         // có thể rỗng
    int group;            // GROUP_FAMILY / GROUP_FRIEND / GROUP_WORK
    PhoneNode* phones;    // danh sách các số điện thoại (phần A: PhoneList)
};

// Tạo một Contact mới, chưa có số điện thoại nào (phones = nullptr).
Contact* createContact(const string& name, const string& email, int group);

// Giải phóng Contact và toàn bộ danh sách số điện thoại của nó.
void freeContact(Contact* c);

#endif
