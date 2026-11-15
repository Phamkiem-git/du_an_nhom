// ============================================================
// File      : src/ui/Menu.cpp
// Phụ trách : THÀNH VIÊN B
// ============================================================
#include <iostream>
#include <limits>
#include "ui/Menu.h"
#include "core/StringUtils.h"
#include "core/PhoneList.h"
#include "core/Validator.h"

using namespace std;

// ------------------------------------------------------------
// Nhập liệu an toàn
// ------------------------------------------------------------
// readInt: nhập một số nguyên trong khoảng [lo, hi], lặp lại đến khi hợp lệ.
// Nhập chữ (cin.fail) hoặc ngoài khoảng thì báo lỗi và hỏi lại; phần ký tự thừa sau số bị bỏ qua.
// Hết dữ liệu nhập (EOF) thì trả về lo để chương trình thoát êm thay vì lặp vô hạn.
int readInt(const string& prompt, int lo, int hi) {
    int value;

    while (true) {
        cout << prompt;

        cin >> value;

        if (cin.eof()) {                    // hết dữ liệu nhập (Ctrl+D / Ctrl+Z / pipe đóng)
            cout << "\nHet du lieu nhap.\n";
            return lo;                      // menu chính có lo = 0, tức "Thoat"
        }

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Du lieu khong hop le. Vui long nhap lai!\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (value < lo || value > hi) {
            cout << "Gia tri phai nam trong [" << lo << ", " << hi << "]\n";
            continue;
        }

        return value;
    }
}

// readLine: in lời nhắc rồi đọc nguyên một dòng (có thể chứa dấu cách);
// hết dữ liệu nhập thì trả về chuỗi rỗng.
string readLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

// ------------------------------------------------------------
// Hàm trợ giúp hiển thị (chỉ dùng trong file này)
// ------------------------------------------------------------
// groupName: đổi mã nhóm (0/1/2) thành chữ để hiển thị; mã lạ trả về "Khac".
static string groupName(int group) {
    if (group == GROUP_FAMILY) return "Gia dinh";
    if (group == GROUP_FRIEND) return "Ban be";
    if (group == GROUP_WORK)   return "Cong viec";
    return "Khac";
}

// In đầy đủ thông tin một liên hệ (kể cả tất cả số điện thoại).
static void showContact(const Contact* c) {
    cout << "Ten   : " << c->name << '\n';

    cout << "SDT   : ";
    if (c->phones == nullptr) cout << "(khong co)";
    for (const PhoneNode* p = c->phones; p != nullptr; p = p->next) {
        cout << p->number;
        if (p->next != nullptr) cout << ", ";
    }
    cout << '\n';

    cout << "Email : " << (c->email.empty() ? "(khong co)" : c->email) << '\n';
    cout << "Nhom  : " << groupName(c->group) << '\n';
}

// ------------------------------------------------------------
// Case 1: Thêm liên hệ
//
// QUY ƯỚC BỘ NHỚ trong hàm này - mỗi Contact c được tạo bằng createContact
// phải kết thúc bằng ĐÚNG MỘT trong ba cách:
//   (a) book.addContact(c) trả true  -> book sở hữu c, Menu KHÔNG được free
//   (b) book.mergeInto(...) trả true -> mergeInto đã free c, Menu KHÔNG được free
//   (c) mọi trường hợp còn lại       -> Menu phải freeContact(c)
// ------------------------------------------------------------
static void addContactUI(AddressBook& book) {
    // 1. Họ tên (để trống = hủy)
    string name;
    while (true) {
        name = trim(readLine("Ten (de trong de huy): "));
        if (name.empty()) {
            cout << "Da huy them lien he.\n";
            return;
        }
        if (isValidName(name)) break;
        cout << "Ten khong hop le (khong duoc chua dau '|'). Nhap lai!\n";
    }

    // 2. Email (được phép rỗng)
    string email;
    while (true) {
        email = trim(readLine("Email (co the de trong): "));
        if (isValidEmail(email)) break;
        cout << "Email khong hop le (can dung mot dau '@' va co ky tu hai phia). Nhap lai!\n";
    }

    // 3. Nhóm
    int group = readInt("Nhom (0 = Gia dinh, 1 = Ban be, 2 = Cong viec): ", 0, 2);

    // 4. Các số điện thoại. Từ đây c đã được cấp phát nên mọi lối thoát phải xử lý c.
    Contact* c = createContact(name, email, group);
    cout << "Nhap cac so dien thoai (de trong de ket thuc).\n";
    while (true) {
        string phone = trim(readLine("  So dien thoai: "));

        if (phone.empty()) {
            if (countPhones(c) == 0) {
                cout << "Lien he phai co it nhat mot so. Da huy them lien he.\n";
                freeContact(c);                       // (c)
                return;
            }
            break;
        }
        if (!isValidPhone(phone)) {
            cout << "  So khong hop le (chi gom chu so, 9 den 11 chu so).\n";
            continue;
        }
        if (hasPhone(c, phone)) {
            cout << "  Ban da nhap so nay roi.\n";
            continue;
        }
        Contact* owner = book.findByPhone(phone);
        if (owner != nullptr) {
            cout << "  So nay da thuoc ve lien he: " << owner->name << "\n";
            continue;
        }
        addPhone(c, phone);
    }

    // 5. Trùng tên -> hỏi người dùng có gộp không
    Contact* existing = book.findByName(name);
    if (existing != nullptr) {
        cout << "\nTen nay da co trong danh ba:\n";
        showContact(existing);
        int choice = readInt("Gop cac so moi vao lien he nay? (1 = Co, 0 = Khong): ", 0, 1);

        if (choice == 1) {
            if (book.mergeInto(existing, c)) {
                cout << "Da gop thanh cong.\n";        // (b) mergeInto đã free c
            } else {
                cout << "Khong the gop lien he.\n";
                freeContact(c);                       // (c) THEO QUY ƯỚC: mergeInto thất bại thì chưa free c
            }
        } else {
            cout << "Da huy, khong them lien he.\n";
            freeContact(c);                           // (c)
        }
        return;
    }

    // 6. Không trùng tên -> thêm mới
    if (book.addContact(c)) {
        cout << "Them thanh cong!\n";                 // (a) book sở hữu c
    } else {
        cout << "Khong the them lien he.\n";
        freeContact(c);                               // (c)
    }
}

// ------------------------------------------------------------
// Case 2: Sửa liên hệ (đổi email / nhóm, thêm / xóa số)
//
// Email và nhóm KHÔNG phải khóa của cây hay bảng băm, nên sửa trực tiếp trên Contact
// là an toàn. Còn thêm/xóa SỐ phải đi qua AddressBook để bảng băm luôn nhất quán.
// ------------------------------------------------------------
static void editContactUI(AddressBook& book) {
    string name = trim(readLine("Ten lien he can sua (de trong de huy): "));
    if (name.empty()) return;

    Contact* c = book.findByName(name);
    if (c == nullptr) {
        cout << "Khong tim thay lien he!\n";
        return;
    }

    while (true) {
        cout << "\n--- Thong tin hien tai ---\n";
        showContact(c);
        cout << "1. Doi email\n";
        cout << "2. Doi nhom\n";
        cout << "3. Them so dien thoai\n";
        cout << "4. Xoa so dien thoai\n";
        cout << "0. Xong\n";

        int choice = readInt("Chon: ", 0, 4);

        switch (choice) {
        // 0: Kết thúc sửa, quay về menu chính.
        case 0:
            return;

        // 1: Đổi email (để trống = xóa email; email sai thì giữ nguyên).
        case 1: {
            string email = trim(readLine("Email moi (de trong de xoa email): "));
            if (isValidEmail(email)) {
                c->email = email;
                cout << "Da cap nhat email.\n";
            } else {
                cout << "Email khong hop le, giu nguyen.\n";
            }
            break;
        }

        // 2: Đổi nhóm (readInt bảo đảm giá trị 0..2).
        case 2:
            c->group = readInt("Nhom moi (0 = Gia dinh, 1 = Ban be, 2 = Cong viec): ", 0, 2);
            cout << "Da cap nhat nhom.\n";
            break;

        // 3: Thêm số: kiểm tra hợp lệ và chưa thuộc liên hệ nào, rồi đi qua book.addPhoneTo.
        case 3: {
            string phone = trim(readLine("So dien thoai can them: "));
            if (!isValidPhone(phone)) {
                cout << "So khong hop le (chi gom chu so, 9 den 11 chu so).\n";
                break;
            }
            Contact* owner = book.findByPhone(phone);
            if (owner != nullptr) {
                cout << "So nay da thuoc ve lien he: " << owner->name << "\n";
            } else if (book.addPhoneTo(c->name, phone)) {
                cout << "Da them so.\n";
            } else {
                cout << "Khong the them so.\n";
            }
            break;
        }

        // 4: Xóa số: phải có trong liên hệ và không phải số cuối cùng, rồi đi qua book.removePhoneFrom.
        case 4: {
            string phone = trim(readLine("So dien thoai can xoa: "));
            if (!hasPhone(c, phone)) {
                cout << "Lien he khong co so nay.\n";
            } else if (countPhones(c) <= 1) {
                cout << "Khong the xoa: lien he phai con it nhat mot so.\n";
            } else if (book.removePhoneFrom(c->name, phone)) {
                cout << "Da xoa so.\n";
            } else {
                cout << "Khong the xoa so.\n";
            }
            break;
        }
        }
    }
}

// ------------------------------------------------------------
// Vòng lặp menu chính
// ------------------------------------------------------------
// runMenu: vòng lặp menu chính: in menu -> readInt (0..9) -> switch gọi chức năng, đến khi chọn 0.
// Mọi thao tác thêm/xóa/tra cứu đều đi qua AddressBook; Menu không gọi trực tiếp HashTable hay NameTree.
void runMenu(AddressBook& book) {

    while (true) {
        cout << "\n=================================\n";
        cout << "      QUAN LY DANH BA\n";
        cout << "=================================\n";
        cout << "1. Them lien he\n";
        cout << "2. Sua lien he\n";
        cout << "3. Xoa lien he\n";
        cout << "4. Tra theo so dien thoai\n";
        cout << "5. Tra theo ten\n";
        cout << "6. Tim theo tien to ten\n";
        cout << "7. In danh ba A-Z\n";
        cout << "8. In theo nhom\n";
        cout << "9. Thong ke\n";
        cout << "0. Thoat\n";

        int choice = readInt("Chon chuc nang: ", 0, 9);

        switch (choice) {

        // 0: Thoát vòng lặp; main sẽ ghi tệp rồi kết thúc.
        case 0:
            cout << "Hen gap lai!\n";
            return;

        // 1: Thêm liên hệ mới (addContactUI).
        case 1:
            addContactUI(book);
            break;

        // 2: Sửa liên hệ (editContactUI).
        case 2:
            editContactUI(book);
            break;

        // 3: Xóa liên hệ theo tên: book.removeContact.
        case 3: {
            string name = readLine("Nhap ten can xoa: ");

            if (book.removeContact(name))
                cout << "Xoa thanh cong!\n";
            else
                cout << "Khong tim thay lien he!\n";

            break;
        }

        // 4: Tra theo số điện thoại: book.findByPhone (bảng băm).
        case 4: {
            string phone = trim(readLine("Nhap so dien thoai: "));

            Contact* c = book.findByPhone(phone);

            if (c == nullptr) {
                cout << "Khong tim thay lien he!\n";
            } else {
                cout << "Tim thay:\n";
                showContact(c);
            }

            break;
        }

        // 5: Tra theo tên đầy đủ: book.findByName (cây BST).
        case 5: {
            string name = trim(readLine("Nhap ten can tim: "));

            Contact* c = book.findByName(name);

            if (c == nullptr) {
                cout << "Khong tim thay lien he!\n";
            } else {
                cout << "Tim thay:\n";
                showContact(c);
            }

            break;
        }

        // 6: Tìm theo tiền tố tên: book.searchPrefix (cây BST, có cắt nhánh).
        case 6: {
            string prefix = trim(readLine("Nhap tien to: "));
            book.searchPrefix(prefix);
            break;
        }

        // 7: In danh bạ A-Z: book.printAll (duyệt LNR).
        case 7:
            cout << "\n===== DANH BA A-Z =====\n";
            book.printAll();
            break;

        // 8: In theo nhóm: chọn nhóm 0..2 rồi gọi book.printByGroup.
        case 8: {
            cout << "0. Gia dinh\n";
            cout << "1. Ban be\n";
            cout << "2. Cong viec\n";

            int group = readInt("Nhap nhom: ", 0, 2);

            cout << "\n===== DANH SACH NHOM =====\n";
            book.printByGroup(group);
            break;
        }

        // 9: Thống kê bảng băm: book.printStats (phần của A).
        case 9:
            cout << "\n===== THONG KE =====\n";
            book.printStats();
            break;
        }
    }
}