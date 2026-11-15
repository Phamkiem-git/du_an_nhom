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

// ---- Test HashTable, lượt (b): insert và find ----
static void testHashTableInsertFind() {
    Contact* an = createContact("An", "", GROUP_FRIEND);
    Contact* binh = createContact("Binh", "", GROUP_FAMILY);

    HashTable h(7);
    check(h.find("0901234567") == nullptr, "bảng rỗng thì find trả về nullptr");

    check(h.insert("0901234567", an) == true, "thêm số mới thành công");
    check(h.size() == 1, "sau khi thêm, size = 1");
    check(h.find("0901234567") == an, "find trả về đúng liên hệ vừa thêm");
    check(h.find("0999999999") == nullptr, "find số chưa thêm trả về nullptr");

    check(h.insert("0901234567", binh) == false, "thêm số trùng bị từ chối");
    check(h.size() == 1, "thêm trùng không làm tăng size");
    check(h.find("0901234567") == an, "thêm trùng không ghi đè liên hệ cũ");

    // Bốn số dưới đây đã được kiểm tra bằng tay: trong bảng 7 ô, 0901111111 và 0908888888
    // cùng bắt đầu ở ô 0; 0902222222 và 0909999999 cùng bắt đầu ở ô 1 -> có đụng độ.
    HashTable g(7);
    check(g.insert("0901111111", an), "chèn số thứ nhất (ô 0)");
    check(g.insert("0908888888", binh), "chèn số đụng độ với số thứ nhất");
    check(g.insert("0902222222", an), "chèn số thứ ba (ô 1)");
    check(g.insert("0909999999", binh), "chèn số đụng độ với số thứ ba");
    check(g.find("0901111111") == an, "tìm lại số thứ nhất");
    check(g.find("0908888888") == binh, "tìm lại số đụng độ thứ nhất (phải dò thêm)");
    check(g.find("0902222222") == an, "tìm lại số thứ ba");
    check(g.find("0909999999") == binh, "tìm lại số đụng độ thứ ba (phải dò thêm)");
    check(g.find("0907777777") == nullptr, "số chưa thêm vẫn trả về nullptr dù bảng có đụng độ");

    // Nhiều phần tử trong bảng lớn hơn (60/101 < 0,7 nên chưa cần rehash)
    Contact* nhieu = createContact("Nhieu", "", GROUP_WORK);
    HashTable big(101);
    bool allInserted = true;
    for (int i = 0; i < 60; i++) {
        string key = "090000" + std::to_string(1000 + i);
        if (!big.insert(key, nhieu)) allInserted = false;
    }
    check(allInserted, "chèn 60 số khác nhau đều thành công");
    check(big.size() == 60, "size = 60 sau khi chèn 60 số");

    bool allFound = true;
    for (int i = 0; i < 60; i++) {
        string key = "090000" + std::to_string(1000 + i);
        if (big.find(key) != nhieu) allFound = false;
    }
    check(allFound, "tìm lại được cả 60 số");
    check(big.find("0900009999") == nullptr, "số chưa chèn trả về nullptr");

    freeContact(an);
    freeContact(binh);
    freeContact(nhieu);
}

// ---- Test HashTable, lượt (c): remove, rehash, loadFactor, avgProbes ----
// So sánh hai số thực: coi là bằng nhau nếu chênh lệch rất nhỏ.
static bool near(double a, double b) {
    return a - b < 1e-9 && b - a < 1e-9;
}

static void testHashTableRemoveRehash() {
    Contact* an = createContact("An", "", GROUP_FRIEND);
    Contact* binh = createContact("Binh", "", GROUP_FAMILY);

    // --- remove ---
    // Trong bảng 7 ô: 0901111111 ở ô 0; 0908888888 cũng bắt đầu ở ô 0 nên bị đẩy sang ô 6.
    HashTable h(7);
    h.insert("0901111111", an);
    h.insert("0908888888", binh);
    h.insert("0902222222", an);

    check(h.remove("0901111111") == true, "xóa số đang có thành công");
    check(h.size() == 2, "sau khi xóa, size giảm còn 2");
    check(h.find("0901111111") == nullptr, "số đã xóa không còn tìm thấy");
    check(h.find("0908888888") == binh, "số nằm SAU ô bị xóa trên đường dò vẫn tìm thấy (nhờ ô DELETED)");
    check(h.remove("0901111111") == false, "xóa lần hai cùng số trả về false");
    check(h.remove("0999999999") == false, "xóa số chưa từng có trả về false");

    check(h.insert("0901111111", binh) == true, "thêm lại số vừa xóa thành công");
    check(h.find("0901111111") == binh, "tìm thấy số vừa thêm lại");
    check(h.size() == 3, "size trở lại 3");

    // --- loadFactor ---
    HashTable e(7);
    check(near(e.loadFactor(), 0.0), "bảng rỗng có hệ số tải 0");
    e.insert("0901111111", an);
    e.insert("0908888888", binh);
    e.insert("0902222222", an);
    e.insert("0909999999", binh);
    check(near(e.loadFactor(), 4.0 / 7), "4 phần tử trong 7 ô: hệ số tải 4/7");
    e.remove("0901111111");
    check(near(e.loadFactor(), 4.0 / 7), "xóa một phần tử: ô DELETED vẫn được tính, hệ số tải vẫn 4/7");

    // --- avgProbes ---
    HashTable p(7);
    check(near(p.avgProbes(), 0.0), "chưa tra cứu lần nào thì số lần dò trung bình là 0");
    p.insert("0901111111", an);      // ô 0
    p.insert("0908888888", binh);    // ô 6 (bị đẩy từ ô 0)
    p.find("0901111111");            // 1 lần dò: ô 0
    p.find("0908888888");            // 2 lần dò: ô 0 rồi ô 6
    p.find("0907777777");            // 2 lần dò: ô 6 rồi ô 4 (trống) -> không có
    check(near(p.avgProbes(), 5.0 / 3), "3 lần tra cứu, tổng 5 lần dò: trung bình 5/3");

    // --- rehash ---
    HashTable g(7);
    bool allInserted = true;
    for (int i = 0; i < 30; i++) {
        string key = "090000" + std::to_string(1000 + i);
        if (!g.insert(key, an)) allInserted = false;
    }
    check(allInserted, "chèn 30 số vào bảng 7 ô đều thành công (bảng tự mở rộng)");
    check(g.capacity() > 7, "kích thước bảng đã tăng");
    check(g.size() == 30, "size = 30");
    check(g.loadFactor() <= 0.7 + 1e-9, "hệ số tải không vượt 0,7");
    bool allFound = true;
    for (int i = 0; i < 30; i++) {
        string key = "090000" + std::to_string(1000 + i);
        if (g.find(key) != an) allFound = false;
    }
    check(allFound, "sau rehash, cả 30 số vẫn tìm thấy");

    // rehash cũng dọn sạch ô DELETED
    HashTable d(11);
    for (int i = 1; i <= 7; i++) d.insert("090000000" + std::to_string(i), an);
    d.remove("0900000001");
    d.remove("0900000002");
    d.remove("0900000003");
    check(d.capacity() == 11, "chưa rehash: kích thước vẫn 11");
    d.insert("0900000008", an);      // (4 + 3 + 1) / 11 > 0,7 nên rehash trước khi chèn
    check(d.capacity() == 23, "đủ điều kiện thì rehash: 11 -> 23");
    check(near(d.loadFactor(), 5.0 / 23), "sau rehash ô DELETED biến mất: hệ số tải = 5/23");
    check(d.find("0900000004") == an && d.find("0900000008") == an && d.find("0900000002") == nullptr,
          "sau rehash: số còn lại vẫn tìm thấy, số đã xóa vẫn không thấy");

    // nhiều lần rehash liên tiếp
    HashTable big(7);
    for (int i = 0; i < 1000; i++) big.insert("0911" + std::to_string(100000 + i), binh);
    bool ok1000 = (big.size() == 1000);
    for (int i = 0; i < 1000; i++) {
        if (big.find("0911" + std::to_string(100000 + i)) != binh) ok1000 = false;
    }
    check(ok1000, "chèn 1000 số (nhiều lần rehash), tìm lại đủ 1000 số");

    freeContact(an);
    freeContact(binh);
}

int main() {
    testPhoneList();
    testValidator();
    testHashTableCreate();
    testHashTableInsertFind();
    testHashTableRemoveRehash();

    std::cout << (failed == 0 ? "Tất cả test đạt\n" : "Có test lỗi\n");
    return failed;
}