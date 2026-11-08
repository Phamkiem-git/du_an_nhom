// ============================================================
// File      : src/ui/Menu.cpp
// Phụ trách : THÀNH VIÊN B
// ============================================================
#include <iostream>
#include "ui/Menu.h"

using namespace std;

int readInt(const string& prompt, int lo, int hi) {
    // TODO (B): vòng lặp đến khi nhập đúng. Xử lý cin.fail() và giá trị ngoài [lo, hi].
    return lo;
}

string readLine(const string& prompt) {
    // TODO (B): in prompt, getline(cin, s), trả về s (nhớ xử lý ký tự xuống dòng còn sót từ lần nhập số trước).
    return "";
}

void runMenu(AddressBook& book) {
    // TODO (B): vòng lặp: in menu -> readInt -> switch. Gợi ý các lựa chọn:
    //   1. Thêm liên hệ          (nhập tên, email, nhóm, các số; nếu trùng tên thì hỏi gộp -> book.mergeInto)
    //   2. Sửa liên hệ           (đổi email/nhóm; thêm/xóa số -> book.addPhoneTo / removePhoneFrom)
    //   3. Xóa liên hệ           (book.removeContact)
    //   4. Tra theo số điện thoại (book.findByPhone)
    //   5. Tra theo tên          (book.findByName)
    //   6. Tìm theo tiền tố tên  (book.searchPrefix)
    //   7. In danh bạ A-Z        (book.printAll)
    //   8. In theo nhóm          (book.printByGroup)
    //   9. Thống kê bảng băm     (book.printStats)   <- phần A
    //   0. Thoát
    cout << "(Menu chưa cài đặt)\n";
}
