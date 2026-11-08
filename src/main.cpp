// ============================================================
// File      : src/main.cpp
// Phụ trách : THÀNH VIÊN B (A đọc để nắm luồng chạy của cả chương trình)
// Luồng     : đọc tệp -> chạy menu -> ghi tệp -> thoát (AddressBook tự giải phóng bộ nhớ).
// ============================================================
#include <iostream>
#include "core/AddressBook.h"
#include "io/FileManager.h"
#include "ui/Menu.h"

using namespace std;

int main(int argc, char* argv[]) {
    string path = (argc > 1) ? argv[1] : "data/contacts_sample.txt";

    AddressBook book;   // khi kết thúc main, destructor sẽ giải phóng toàn bộ bộ nhớ

    if (!loadContacts(book, path)) {
        cout << "[!] Chưa đọc được tệp " << path << ", bắt đầu với danh bạ rỗng.\n";
    }

    runMenu(book);

    if (!saveContacts(book, path)) {
        cout << "[!] Không ghi được tệp " << path << "\n";
    }
    return 0;
}
