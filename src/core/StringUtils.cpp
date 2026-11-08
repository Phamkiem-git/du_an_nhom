// ============================================================
// File      : src/core/StringUtils.cpp
// Phụ trách : THÀNH VIÊN B
// ============================================================
#include "core/StringUtils.h"

string toLower(const string& s) {
    // TODO (B): sao chép s, đổi mỗi ký tự 'A'..'Z' thành chữ thường (cộng 'a' - 'A').
    return s;
}

string trim(const string& s) {
    // TODO (B): tìm vị trí ký tự khác ' ' đầu tiên và cuối cùng rồi dùng s.substr(...).
    return s;
}

bool startsWith(const string& s, const string& prefix) {
    // TODO (B): nếu prefix dài hơn s thì false; ngược lại so sánh từng ký tự (sau khi toLower).
    return false;
}

int compareNames(const string& a, const string& b) {
    // TODO (B): toLower cả hai rồi so sánh từng ký tự; ký tự khác nhau đầu tiên quyết định kết quả;
    //   nếu một chuỗi là tiền tố của chuỗi kia thì chuỗi ngắn hơn đứng trước.
    return 0;
}

int indexOf(const string& s, char c, int from) {
    // TODO (B): vòng lặp từ from đến hết chuỗi, gặp c thì trả về chỉ số; không gặp trả về -1.
    return -1;
}

bool isValidName(const string& name) {
    // TODO (B): xem mô tả trong StringUtils.h.
    return false;
}

bool isValidEmail(const string& email) {
    // TODO (B): xem mô tả trong StringUtils.h.
    return false;
}
