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
#include "core/Validator.h"

static int failed = 0;

static void check(bool cond, const char* msg) {
    std::cout << (cond ? "[ OK ] " : "[FAIL] ") << msg << "\n";
    if (!cond) failed++;
}

// ---- Test PhoneList ----
static void testPhoneList() {
    Contact* c = createContact("An", "", GROUP_FRIEND);

    check(countPhones(c) == 0, "liên hệ mới chưa có số nào");
    check(!hasPhone(c, "0901234567"), "danh sách rỗng thì hasPhone = false");
    check(!removePhone(c, "0901234567"), "xóa trên danh sách rỗng trả về false");

    addPhone(c, "0901111111");   // danh sách: 0901111111
    addPhone(c, "0902222222");   // danh sách: 0902222222 -> 0901111111
    addPhone(c, "0903333333");   // danh sách: 0903333333 -> 0902222222 -> 0901111111
    check(countPhones(c) == 3, "thêm 3 số thì đếm được 3");
    check(hasPhone(c, "0902222222"), "tìm thấy số nằm giữa");
    check(!hasPhone(c, "0999999999"), "không thấy số chưa thêm");

    check(removePhone(c, "0902222222"), "xóa nút ở giữa thành công");
    check(countPhones(c) == 2 && !hasPhone(c, "0902222222"), "sau khi xóa giữa: còn 2 số, số đó biến mất");

    check(removePhone(c, "0903333333"), "xóa nút ĐẦU thành công");
    check(countPhones(c) == 1 && hasPhone(c, "0901111111"), "sau khi xóa đầu: còn đúng số cuối");

    check(!removePhone(c, "0999999999"), "xóa số không tồn tại trả về false");

    check(removePhone(c, "0901111111"), "xóa nút CUỐI (cũng là nút duy nhất) thành công");
    check(countPhones(c) == 0 && c->phones == nullptr, "danh sách về rỗng, phones = nullptr");

    addPhone(c, "0904444444");
    addPhone(c, "0905555555");
    freeContact(c);   // phải giải phóng sạch danh sách; kiểm tra bằng make leaks
}

// ---- Test Validator ----
static void testValidator() {
    check(isValidPhone("0901234567"), "số 10 chữ số hợp lệ");
    check(isValidPhone("090123456"), "số 9 chữ số (biên dưới) hợp lệ");
    check(isValidPhone("09012345678"), "số 11 chữ số (biên trên) hợp lệ");
    check(!isValidPhone("09012345"), "số 8 chữ số bị từ chối");
    check(!isValidPhone("090123456789"), "số 12 chữ số bị từ chối");
    check(!isValidPhone(""), "chuỗi rỗng bị từ chối");
    check(!isValidPhone("09012abc67"), "số có chữ cái bị từ chối");
    check(!isValidPhone("0901 234 567"), "số có khoảng trắng bị từ chối");
    check(!isValidPhone("+84901234567"), "số có dấu + bị từ chối");

    check(isValidGroup(GROUP_FAMILY), "nhóm 0 hợp lệ");
    check(isValidGroup(GROUP_FRIEND), "nhóm 1 hợp lệ");
    check(isValidGroup(GROUP_WORK), "nhóm 2 hợp lệ");
    check(!isValidGroup(-1), "nhóm -1 bị từ chối");
    check(!isValidGroup(3), "nhóm 3 bị từ chối");
}

// ---- Test HashTable, lượt (a): tạo bảng ----
static void testHashTableCreate() {
    HashTable h1(7);
    check(h1.capacity() == 7, "kích thước 7 đã là số nguyên tố nên giữ nguyên");

    HashTable h2(100);
    check(h2.capacity() == 101, "100 không nguyên tố, được đổi thành 101");

    HashTable h3;
    check(h3.capacity() == 101, "kích thước mặc định là 101");
    check(h3.size() == 0, "bảng mới tạo chưa có phần tử nào");
}

int main() {
    testPhoneList();
    testValidator();
    testHashTableCreate();

    // TODO (A): lượt (b), (c): thêm test insert / find / remove / rehash cho HashTable, ví dụ:
    //   HashTable h(7);                                    // bảng nhỏ để dễ thấy đụng độ và rehash
    //   check(h.insert("0901234567", c) == true,  "thêm số mới thành công");
    //   check(h.insert("0901234567", c) == false, "thêm số trùng bị từ chối");
    //   ... xóa rồi tìm, xóa rồi chèn lại, chèn nhiều số để kích hoạt rehash ...

    std::cout << (failed == 0 ? "Tất cả test đạt\n" : "Có test lỗi\n");
    return failed;
}