// ============================================================
// File      : tests/test_hash.cpp
// Phụ trách : THÀNH VIÊN A
// Vai trò   : Test RIÊNG cho PhoneList + HashTable, không cần chờ phần của B.
// Chạy      : make test_hash
// ============================================================
#include <iostream>
#include "core/Contact.h"
#include "core/PhoneList.h"
#include "core/HashTable.h"

static int failed = 0;

// [[maybe_unused]]: tạm tránh cảnh báo khi chưa có test nào gọi check().
[[maybe_unused]] static void check(bool cond, const char* msg) {
    std::cout << (cond ? "[ OK ] " : "[FAIL] ") << msg << "\n";
    if (!cond) failed++;
}

int main() {
    // TODO (A): viết test, ví dụ:
    //   HashTable h(7);                                    // bảng nhỏ để dễ thấy đụng độ và rehash
    //   Contact* c = createContact("An", "", GROUP_FRIEND);
    //   addPhone(c, "0901234567");
    //   check(h.insert("0901234567", c) == true,  "thêm số mới thành công");
    //   check(h.insert("0901234567", c) == false, "thêm số trùng bị từ chối");
    //   check(h.find("0901234567") == c,          "tìm thấy số vừa thêm");
    //   check(h.find("0999999999") == nullptr,    "không tìm thấy số chưa thêm");
    //   ... thêm test: xóa rồi tìm, xóa rồi chèn lại, chèn nhiều số để kích hoạt rehash, ...
    //   freeContact(c);

    std::cout << (failed == 0 ? "Tất cả test đạt\n" : "Có test lỗi\n");
    return failed;
}
