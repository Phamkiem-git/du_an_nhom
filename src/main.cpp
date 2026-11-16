// Luồng     : đọc tệp -> chạy menu -> ghi tệp -> thoát (AddressBook tự giải phóng bộ nhớ).
#include <iostream>
#include "core/AddressBook.h"
#include "io/FileManager.h"
#include "ui/Menu.h"

using namespace std;

// main: điểm vào chương trình. Luồng: đọc tệp -> chạy menu -> ghi tệp -> thoát.
int main(int argc, char* argv[]) {
    // Đường dẫn tệp dữ liệu: lấy từ tham số dòng lệnh đầu tiên nếu có, không thì dùng data/contacts_sample.txt.
    string path = (argc > 1) ? argv[1] : "data/contacts_sample.txt";

    AddressBook book;   // khi kết thúc main, destructor sẽ giải phóng toàn bộ bộ nhớ

    // Đọc dữ liệu khi khởi động và báo số liên hệ đã nạp; không mở được tệp thì vẫn chạy với danh bạ rỗng.
    if (!loadContacts(book, path)) {
        cout << "[!] Chua doc duoc tep " << path << ", bat dau voi danh ba rong.\n";
    } else {
        cout << "Da doc " << book.size() << " lien he tu " << path << "\n";
    }

    // Giao toàn bộ việc tương tác với người dùng cho menu; chạy đến khi người dùng chọn thoát.
    runMenu(book);

    // Ghi danh bạ trở lại tệp (ghi đè) trước khi thoát; báo số liên hệ đã lưu hoặc báo lỗi.
    if (saveContacts(book, path)) {
        cout << "Da luu " << book.size() << " lien he vao " << path << "\n";
    } else {
        cout << "[!] Khong ghi duoc tep " << path << "\n";
    }
    // Khi return, destructor của book tự giải phóng toàn bộ bộ nhớ.
    return 0;
}