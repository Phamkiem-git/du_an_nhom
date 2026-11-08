// ============================================================
// File      : tests/test_tree.cpp
// Phụ trách : THÀNH VIÊN B
// Vai trò   : Test RIÊNG cho StringUtils + NameTree, không cần chờ phần của A.
// Chạy      : make test_tree
// ============================================================
#include <iostream>
#include "core/Contact.h"
#include "core/NameTree.h"
#include "core/StringUtils.h"

static int failed = 0;

// [[maybe_unused]]: tạm tránh cảnh báo khi chưa có test nào gọi check().
[[maybe_unused]] static void check(bool cond, const char* msg) {
    std::cout << (cond ? "[ OK ] " : "[FAIL] ") << msg << "\n";
    if (!cond) failed++;
}

int main() {
    // TODO (B): viết test, ví dụ:
    //   check(compareNames("an", "Binh") < 0, "an đứng trước Binh (không phân biệt hoa thường)");
    //   check(startsWith("Nguyen Van An", "nguyen"), "tiền tố không phân biệt hoa thường");
    //   NameTree t;
    //   check(t.insert(createContact("Binh", "", GROUP_FRIEND)) == true, "thêm Binh");
    //   check(t.insert(createContact("Binh", "", GROUP_FRIEND)) == false, "tên trùng bị từ chối");
    //     (!) khi insert bị từ chối, Contact vừa tạo phải được freeContact để không rò rỉ.
    //   ... thêm test: xóa nút lá / một con / hai con, chiều cao cây, searchPrefix ...
    //   t.destroyAll();

    std::cout << (failed == 0 ? "Tất cả test đạt\n" : "Có test lỗi\n");
    return failed;
}
