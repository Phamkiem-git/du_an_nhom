// ============================================================
// File      : src/core/StringUtils.h
// Phụ trách : THÀNH VIÊN B
// Review    : thành viên A
// Vai trò   : Các hàm xử lý chuỗi tự viết (không dùng std::find / <algorithm>).
// ============================================================
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
using std::string;

// Chuyển về chữ thường (chỉ cần xử lý 'A'..'Z').
string toLower(const string& s);

// Bỏ khoảng trắng ở đầu và cuối chuỗi.
string trim(const string& s);

// s có bắt đầu bằng prefix không? (so sánh KHÔNG phân biệt hoa thường)
bool startsWith(const string& s, const string& prefix);

// So sánh hai tên KHÔNG phân biệt hoa thường: <0 nếu a đứng trước b, 0 nếu bằng, >0 nếu a đứng sau b.
// Đây là hàm quyết định thứ tự trong cây BST.
int compareNames(const string& a, const string& b);

// Vị trí đầu tiên của ký tự c trong s, bắt đầu tìm từ vị trí from. Trả về -1 nếu không có.
// Dùng khi tách dòng trong tệp theo ký tự '|' và ','.
int indexOf(const string& s, char c, int from = 0);

// Tên hợp lệ: không rỗng sau khi trim, không chứa ký tự '|' (vì '|' là ký tự phân cách trong tệp).
bool isValidName(const string& name);

// Email hợp lệ: rỗng (được phép) hoặc có đúng một '@' và có ký tự ở hai phía.
bool isValidEmail(const string& email);

#endif
