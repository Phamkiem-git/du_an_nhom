// ============================================================
// File      : src/core/Validator.h
// Phụ trách : THÀNH VIÊN A
// Vai trò   : Kiểm tra dữ liệu liên quan số điện thoại và nhóm.
//             (Kiểm tra tên, email nằm ở StringUtils - phần B.)
// ============================================================
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
using std::string;

// Số điện thoại hợp lệ: chỉ gồm chữ số, độ dài hợp lý (tạm chọn 9 đến 11 chữ số).
bool isValidPhone(const string& phone);

// Nhóm hợp lệ: 0, 1 hoặc 2.
bool isValidGroup(int group);

#endif
