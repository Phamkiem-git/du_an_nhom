// ============================================================
// File      : src/io/FileManager.cpp
// Phụ trách : THÀNH VIÊN B
// ============================================================
#include <fstream>
#include <iostream>
#include "io/FileManager.h"
#include "core/AddressBook.h"
#include "core/PhoneList.h"
#include "core/StringUtils.h"
#include "core/Validator.h"

using namespace std;

Contact* parseContactLine(const string& line) {
    // TODO (B): dùng indexOf() tìm ba dấu '|' để cắt thành: tên | chuỗi số | email | nhóm.
    //   Chuỗi số lại cắt theo ',' để addPhone(c, từng số).
    //   Đổi nhóm từ chuỗi sang int (tự viết, không dùng thư viện lạ). Trả về nullptr nếu thiếu trường.
    return nullptr;
}

bool loadContacts(AddressBook& book, const string& path) {
    // TODO (B): ifstream in(path); nếu !in.is_open() thì return false.
    //   Đọc từng dòng bằng getline; bỏ dòng trống và dòng bắt đầu bằng '#'.
    //   parseContactLine -> book.addContact(c). Nếu addContact trả về false thì freeContact(c) để không rò rỉ,
    //   và in cảnh báo kèm số dòng.
    return false;
}

void writeContactLine(ostream& out, const Contact* c) {
    // TODO (B): ghi: tên|số1,số2,...|email|nhóm rồi xuống dòng.
}

bool saveContacts(const AddressBook& book, const string& path) {
    // TODO (B): ofstream out(path); nếu không mở được thì return false;
    //   book.names().writeAll(out); return true.
    return false;
}
