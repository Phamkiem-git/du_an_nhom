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

// ============================================================
// [NÂNG CAO] NHẬP / XUẤT CSV
//
// ĐỊNH DẠNG CSV (dòng đầu là tiêu đề, mỗi liên hệ một dòng, 4 cột):
//     Ho ten,So dien thoai,Email,Nhom
//     "Nguyễn Văn An",0901234567;0912345678,an@mail.com,1
//   - Nhiều số của một liên hệ nằm CÙNG MỘT ô, cách nhau bằng ';' (khi nhập chấp nhận cả ',').
//   - Ô chứa dấu phẩy, dấu nháy kép hoặc khoảng trắng đầu/cuối được bọc trong "..."; dấu " bên
//     trong được gấp đôi thành "" (quy tắc chuẩn RFC 4180).
//   - Nhóm là số 0/1/2 như tệp .txt. Email có thể để trống.
//   - Khi xuất có ghi BOM UTF-8 ở đầu tệp để Excel hiển thị đúng chữ có dấu; khi nhập BOM bị bỏ qua.
//   - Khi nhập tự nhận ra dấu phân cách ',' hoặc ';' (Excel bản tiếng Việt thường lưu CSV bằng ';').
// ============================================================

// Xuất toàn bộ danh bạ ra tệp CSV path (ghi đè), theo thứ tự chữ cái. Trả về false nếu không ghi được.
bool exportCsv(const AddressBook& book, const string& path);

// Nhập tệp CSV path: thêm từng liên hệ vào book bằng book.addContact(). Dòng sai định dạng hoặc bị
// book từ chối (trùng số / trùng tên / dữ liệu sai) thì in cảnh báo kèm số dòng rồi bỏ qua.
// added / skipped là số liên hệ đã thêm / bị bỏ qua. Trả về false nếu không mở được tệp.
bool importCsv(AddressBook& book, const string& path, int& added, int& skipped);

// Ghi MỘT liên hệ thành một dòng CSV (xuống dòng ở cuối). NameTree::writeAllCsv gọi hàm này.
void writeCsvLine(std::ostream& out, const Contact* c);

// Phân tích MỘT dòng CSV (delimiter là ',' hoặc ';') thành Contact mới. Trả về nullptr nếu dòng sai
// (không đủ 4 cột, nháy kép không đóng, tên rỗng, nhóm không phải số). Giống parseContactLine, hàm này
// không kiểm tra số điện thoại/email: phần đó AddressBook::addContact lo.
Contact* parseCsvLine(const string& line, char delimiter = ',');

#endif
