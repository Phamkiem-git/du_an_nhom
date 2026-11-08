// ============================================================
// File      : src/io/FileManager.h
// Phụ trách : THÀNH VIÊN B
// Review    : thành viên A
// Vai trò   : Đọc danh bạ từ tệp khi khởi động, ghi lại khi thoát.
//
// ĐỊNH DẠNG TỆP (mỗi liên hệ một dòng, các trường cách nhau bằng '|'):
//     Họ tên|số1,số2,số3|email|nhóm
//   Ví dụ: Nguyen Van An|0901234567,0912345678|an@mail.com|1
//   - email có thể để trống:  Le Hoang Cuong|0933333333||2
//   - nhóm: 0 = gia đình, 1 = bạn bè, 2 = công việc
//   - dòng trống hoặc bắt đầu bằng '#' thì bỏ qua
// ============================================================
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iosfwd>
#include <string>
#include "core/Contact.h"

class AddressBook;   // khai báo trước, tránh include vòng

// Đọc tệp path, thêm từng liên hệ vào book bằng book.addContact().
// Dòng sai định dạng hoặc dữ liệu không hợp lệ: in cảnh báo kèm số dòng rồi bỏ qua, KHÔNG dừng chương trình.
// Trả về false nếu không mở được tệp.
bool loadContacts(AddressBook& book, const string& path);

// Ghi toàn bộ danh bạ ra tệp path (qua book.names().writeAll(out)). Trả về false nếu không ghi được.
bool saveContacts(const AddressBook& book, const string& path);

// Ghi MỘT liên hệ thành một dòng theo định dạng trên. NameTree::writeAll gọi hàm này.
void writeContactLine(std::ostream& out, const Contact* c);

// Phân tích MỘT dòng thành Contact mới (createContact + addPhone). Trả về nullptr nếu dòng sai.
Contact* parseContactLine(const string& line);

#endif
