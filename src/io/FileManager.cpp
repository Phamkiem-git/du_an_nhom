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

        // Tệp UTF-8 lưu từ Notepad có thể bắt đầu bằng BOM (3 byte vô hình) làm hỏng tên ở dòng đầu.
        if (lineNo == 1)
            stripBom(line);

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


// ============================================================
// [NÂNG CAO] NHẬP / XUẤT CSV
// ============================================================

// splitCsvLine (trợ giúp): tách một dòng CSV thành các ô, lưu vào fields (tối đa maxFields ô),
// n là số ô tìm được. Trả về false nếu nháy kép không đóng, có ký tự lạ ngay sau nháy đóng,
// hoặc nhiều hơn maxFields ô.
// Ô bọc trong "..." được tách đúng dù chứa dấu phân cách; "" bên trong ô là một dấu nháy kép.
// Ô không bọc nháy thì lấy nguyên đến dấu phân cách kế tiếp (nháy lẻ trong ô được giữ nguyên).
static bool splitCsvLine(const string& line, char delimiter, string fields[], int maxFields, int& n) {
    n = 0;
    size_t i = 0;
    while (true) {
        if (n >= maxFields)
            return false;

        string field;
        if (i < line.length() && line[i] == '"') {
            i++;                                           // bỏ nháy mở
            bool closed = false;
            while (i < line.length()) {
                if (line[i] == '"') {
                    if (i + 1 < line.length() && line[i + 1] == '"') {
                        field += '"';                      // "" -> một dấu "
                        i += 2;
                    } else {
                        i++;                               // nháy đóng
                        closed = true;
                        break;
                    }
                } else {
                    field += line[i++];
                }
            }
            if (!closed)
                return false;                              // nháy kép không đóng
            if (i < line.length() && line[i] != delimiter)
                return false;                              // sau nháy đóng phải là dấu phân cách hoặc hết dòng
        } else {
            while (i < line.length() && line[i] != delimiter)
                field += line[i++];
        }

        fields[n++] = field;

        if (i >= line.length())
            return true;                                   // hết dòng: ô cuối cùng
        i++;                                               // bỏ dấu phân cách; ô tiếp theo (có thể rỗng)
    }
}

// detectDelimiter (trợ giúp): đoán dấu phân cách của tệp CSV từ một dòng (thường là dòng đầu):
// đếm ',' và ';' NGOÀI cặp nháy kép, dấu nào nhiều hơn thì chọn; hòa thì chọn ','.
static char detectDelimiter(const string& line) {
    int commas = 0, semis = 0;
    bool inQuotes = false;
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] == '"')
            inQuotes = !inQuotes;
        else if (!inQuotes && line[i] == ',')
            commas++;
        else if (!inQuotes && line[i] == ';')
            semis++;
    }
    return (semis > commas) ? ';' : ',';
}

// isAllDigits (trợ giúp): true nếu s không rỗng và chỉ gồm chữ số.
static bool isAllDigits(const string& s) {
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}

// parseCsvLine: chuyển MỘT dòng CSV (4 ô: tên, số, email, nhóm) thành Contact mới.
// Ô "số" có thể chứa nhiều số cách nhau bằng ';' hoặc ',' (chuỗi được cắt tay từng ký tự, số rỗng bị bỏ qua).
// Trả về nullptr nếu dòng không tách được đúng 4 ô, tên rỗng, hoặc nhóm không phải số.
// Contact chỉ được cấp phát SAU khi mọi kiểm tra định dạng đã qua (tránh rò rỉ khi trả nullptr).
Contact* parseCsvLine(const string& line, char delimiter) {
    string f[4];
    int n = 0;
    if (!splitCsvLine(line, delimiter, f, 4, n) || n != 4)
        return nullptr;

    string name = trim(f[0]);
    string phones = f[1];
    string email = trim(f[2]);
    string groupStr = trim(f[3]);

    if (name.empty() || !isAllDigits(groupStr))
        return nullptr;

    int group = 0;
    for (size_t i = 0; i < groupStr.length(); i++) {
        if (group > 100000)                    // tránh tràn số khi nhóm dài bất thường; AddressBook sẽ từ chối
            break;
        group = group * 10 + (groupStr[i] - '0');
    }

    Contact* c = createContact(name, email, group);

    string phone;
    for (size_t i = 0; i <= phones.length(); i++) {
        if (i == phones.length() || phones[i] == ';' || phones[i] == ',') {
            phone = trim(phone);
            if (!phone.empty())
                addPhone(c, phone);
            phone = "";
        } else {
            phone += phones[i];
        }
    }
    return c;
}

// writeCsvField (trợ giúp): ghi một ô CSV. Ô chứa dấu phẩy, ';', nháy kép, xuống dòng, hoặc có khoảng
// trắng ở đầu/cuối thì bọc trong "..." và gấp đôi mọi dấu " bên trong.
static void writeCsvField(ostream& out, const string& s) {
    bool needQuote = false;
    for (size_t i = 0; i < s.length(); i++) {
        char ch = s[i];
        if (ch == ',' || ch == ';' || ch == '"' || ch == '\n' || ch == '\r') {
            needQuote = true;
            break;
        }
    }
    if (!s.empty() && (s[0] == ' ' || s[s.length() - 1] == ' '))
        needQuote = true;

    if (!needQuote) {
        out << s;
        return;
    }
    out << '"';
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '"')
            out << '"';                        // gấp đôi dấu nháy kép
        out << s[i];
    }
    out << '"';
}

// writeCsvLine: ghi một liên hệ thành dòng CSV "tên,số1;số2,email,nhóm" rồi xuống dòng.
void writeCsvLine(ostream& out, const Contact* c) {
    if (c == nullptr)
        return;

    writeCsvField(out, c->name);
    out << ',';
    for (const PhoneNode* p = c->phones; p != nullptr; p = p->next) {
        out << p->number;
        if (p->next != nullptr)
            out << ';';
    }
    out << ',';
    writeCsvField(out, c->email);
    out << ',' << c->group << '\n';
}

// exportCsv: ghi BOM UTF-8 + dòng tiêu đề + mọi liên hệ (theo thứ tự chữ cái nhờ duyệt LNR cây tên).
// Trả về false nếu không mở được tệp.
bool exportCsv(const AddressBook& book, const string& path) {
    ofstream out(path);
    if (!out.is_open())
        return false;

    out << "\xEF\xBB\xBF";                   // BOM: giúp Excel nhận ra UTF-8 để hiện đúng chữ có dấu
    out << "Ho ten,So dien thoai,Email,Nhom\n";
    book.names().writeAllCsv(out);
    return out.good();
}

// importCsv: đọc tệp CSV theo từng dòng và thêm vào book (xem FileManager.h).
// Dòng đầu: bỏ BOM, đoán dấu phân cách; nếu cột Nhóm không phải số thì coi là dòng tiêu đề và bỏ qua.
// Dòng trống bị bỏ qua. Dòng sai định dạng hoặc bị book từ chối chỉ in cảnh báo rồi đọc tiếp.
bool importCsv(AddressBook& book, const string& path, int& added, int& skipped) {
    added = 0;
    skipped = 0;

    ifstream in(path);
    if (!in.is_open())
        return false;

    string line;
    int lineNo = 0;
    char delimiter = ',';
    bool firstRow = true;                       // chưa gặp dòng không rỗng nào

    while (getline(in, line)) {
        lineNo++;
        if (lineNo == 1)
            stripBom(line);

        string s = trim(line);
        if (s.empty())
            continue;

        if (firstRow) {
            firstRow = false;
            delimiter = detectDelimiter(s);

            // Dòng tiêu đề: tách được 4 ô nhưng ô Nhóm không phải số -> bỏ qua, không tính là lỗi.
            string f[4];
            int n = 0;
            if (splitCsvLine(s, delimiter, f, 4, n) && n == 4 && !isAllDigits(trim(f[3])))
                continue;
        }

        Contact* c = parseCsvLine(s, delimiter);
        if (c == nullptr) {
            cout << "Canh bao: dong " << lineNo << " (CSV) sai dinh dang.\n";
            skipped++;
            continue;
        }
        // book từ chối thì Contact chưa thuộc ai -> phải freeContact để không rò rỉ bộ nhớ.
        if (!book.addContact(c)) {
            cout << "Canh bao: khong the them lien he o dong " << lineNo << " (CSV).\n";
            freeContact(c);
            skipped++;
            continue;
        }
        added++;
    }
    return true;
}
