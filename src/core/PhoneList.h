// ============================================================
// File      : src/core/PhoneList.h
// Phụ trách : THÀNH VIÊN A
// Review    : thành viên B
// Vai trò   : Danh sách liên kết đơn các số điện thoại của MỘT liên hệ.
// ============================================================
#ifndef PHONELIST_H
#define PHONELIST_H

#include "core/Contact.h"

// Thêm số vào danh sách của c. (Việc kiểm tra trùng giữa các liên hệ do AddressBook làm.)
void addPhone(Contact* c, const string& number);

// Xóa số khỏi danh sách của c. Trả về true nếu có số đó và đã xóa.
bool removePhone(Contact* c, const string& number);

// Liên hệ c có số này chưa?
bool hasPhone(const Contact* c, const string& number);

// Đếm số lượng số điện thoại của c.
int countPhones(const Contact* c);

// Giải phóng toàn bộ danh sách (delete từng PhoneNode), đặt c->phones = nullptr.
void freePhones(Contact* c);

#endif
