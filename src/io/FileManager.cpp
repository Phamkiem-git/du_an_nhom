#include <fstream>
#include <iostream>
#include "io/FileManager.h"
#include "core/AddressBook.h"
#include "core/PhoneList.h"
#include "core/StringUtils.h"
#include "core/Validator.h"

using namespace std;

// parseContactLine: chuyển MỘT dòng tệp dạng "tên|số1,số2|email|nhóm" thành Contact mới (createContact).
// Trả về nullptr nếu dòng có ít hơn 3 dấu '|', tên rỗng, hoặc nhóm rỗng / chứa ký tự không phải chữ số.
// Hàm KHÔNG kiểm tra số điện thoại, email hay miền giá trị của nhóm: phần đó AddressBook::addContact lo.
// Contact trả về chưa thuộc danh bạ: người gọi phải thêm vào book hoặc freeContact.
Contact* parseContactLine(const string& line) {
    // Tìm ba dấu '|' đầu tiên để cắt dòng thành 4 trường: tên | các số | email | nhóm.
    // Mọi thứ sau dấu '|' thứ ba được coi là trường nhóm.
    int p1 = indexOf(line, '|');
    if (p1 == -1) return nullptr;

    int p2 = indexOf(line, '|', p1 + 1);
    if (p2 == -1) return nullptr;

    int p3 = indexOf(line, '|', p2 + 1);
    if (p3 == -1) return nullptr;

    string name = trim(line.substr(0, p1));

    string phonesStr =
        trim(line.substr(p1 + 1, p2 - p1 - 1));

    string email =
        trim(line.substr(p2 + 1, p3 - p2 - 1));

    string groupStr =
        trim(line.substr(p3 + 1));

    if (name.empty())
        return nullptr;

    // Đổi chuỗi nhóm sang số nguyên bằng tay: mỗi chữ số làm group = group * 10 + chữ số;
    // gặp ký tự không phải chữ số thì trả về nullptr.
    int group = 0;

    if (groupStr.empty())
        return nullptr;

    for (size_t i = 0; i < groupStr.length(); i++) {
        if (groupStr[i] < '0' || groupStr[i] > '9')
            return nullptr;

        group = group * 10 + (groupStr[i] - '0');
    }

    // Các kiểm tra định dạng ở trên đã qua (đủ 4 trường, tên không rỗng, nhóm là số) nên mới cấp phát
    // Contact; nhờ vậy các lần return nullptr phía trên không gây rò rỉ bộ nhớ.
    Contact* c = createContact(name, email, group);

    // Tách chuỗi số theo dấu ',': mỗi số được trim, số rỗng bị bỏ qua, rồi thêm vào danh sách liên kết bằng addPhone.
    int start = 0;

    while (true) {
        int comma = indexOf(phonesStr, ',', start);

        if (comma == -1) {
            string phone =
                trim(phonesStr.substr(start));

            if (!phone.empty())
                addPhone(c, phone);

            break;
        }

        string phone =
            trim(phonesStr.substr(start,
                                comma - start));

        if (!phone.empty())
            addPhone(c, phone);

        start = comma + 1;
    }

    return c;
   
}


// loadContacts: đọc tệp path theo từng dòng và thêm các liên hệ vào book.
// Dòng trống và dòng bắt đầu bằng '#' (sau khi trim) bị bỏ qua. Dòng sai định dạng hoặc bị book từ chối
// (trùng số/tên, dữ liệu không hợp lệ...) chỉ in cảnh báo kèm số dòng rồi đọc tiếp, không dừng chương trình.
// Trả về false nếu không mở được tệp, ngược lại trả về true.
bool loadContacts(AddressBook& book, const string& path) {
    ifstream in(path);

    if (!in.is_open())
        return false;

    string line;
    int lineNo = 0;

    while (getline(in, line)) {
        lineNo++;

        // Cắt khoảng trắng (kể cả '\r' cuối dòng của tệp Windows) trước khi xử lý.
        string s = trim(line);

        if (s.empty())
            continue;

        if (s[0] == '#')
            continue;

        Contact* c = parseContactLine(s);

        if (c == nullptr) {
            cout << "Canh bao: dong "
                 << lineNo
                 << " sai dinh dang.\n";
            continue;
        }
        // book từ chối thì Contact chưa thuộc ai -> phải freeContact để không rò rỉ bộ nhớ.
        if (!book.addContact(c)) {
            cout << "Canh bao: khong the them lien he o dong "
                 << lineNo
                 << ".\n";

            freeContact(c);
        }
    }

    return true;
    
}


// writeContactLine: ghi một liên hệ ra out theo định dạng "tên|số1,số2|email|nhóm" rồi xuống dòng.
// Email rỗng thì để trống giữa hai dấu '|'. NameTree::writeAll gọi hàm này cho từng nút.
void writeContactLine(ostream& out, const Contact* c) {
    if (c == nullptr)
        return;

    out << c->name << "|";

    PhoneNode* p = c->phones;

    bool first = true;

    while (p != nullptr) {
        if (!first)
            out << ",";

        out << p->number;

        first = false;
        p = p->next;
    }

    out << "|"
        << c->email
        << "|"
        << c->group
        << '\n';
   
}


// saveContacts: ghi toàn bộ danh bạ ra tệp path (ghi đè), theo thứ tự chữ cái nhờ duyệt cây tên.
// Trả về false nếu không mở được tệp.
bool saveContacts(const AddressBook& book, const string& path) {
     ofstream out(path);

    if (!out.is_open())
        return false;

    book.names().writeAll(out);

    return true;
    
}