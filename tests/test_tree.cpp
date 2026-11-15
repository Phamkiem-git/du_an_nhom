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

void testStringUtils() {
    std::cout << "=== 1. KIEM THU STRINGUTILS ===\n";

      check(toLower("AnH") == "anh", "toLower: AnH -> anh");
      check(trim("  An  ") == "An", "trim: bo dau cach hai dau");
      check(trim("   ") == "", "trim: toan dau cach");
      check(trim("") == "", "trim: chuoi rong");
      check(trim("An\r") == "An", "trim: bo ky tu \\r cuoi dong");
      check(trim("\tAn \r\n") == "An", "trim: bo tab va xuong dong");

      check(startsWith("Nguyen Van An", "nguyen"),
            "startsWith: khong phan biet hoa thuong");
      check(startsWith("Nguyen Van An", "Van") == false,
            "startsWith: khong nam o dau chuoi");

      check(compareNames("An", "Binh") < 0,
            "compareNames: An dung truoc Binh");
      check(compareNames("Nam", "An") > 0,
            "compareNames: Nam dung sau An");
      check(compareNames("AN", "an") == 0,
            "compareNames: khong phan biet hoa thuong");
      (compareNames("An", "Anh") < 0,
            "compareNames: tien to ngan hon");

      check(indexOf("abcde", 'c') == 2,
            "indexOf: tim thay c");
      check(indexOf("abcde", 'x') == -1,
            "indexOf: khong tim thay");
      check(indexOf("abcabc", 'a', 2) == 3,
            "indexOf: from");

      check(isValidName("Nguyen Van A"),
            "isValidName: ten hop le");
      check(!isValidName(""),
            "isValidName: chuoi rong");
      check(!isValidName("     "),
            "isValidName: toan khoang trang");
      check(!isValidName("Nguyen|Van"),
            "isValidName: co dau gach dung");

      check(isValidEmail(""),
            "isValidEmail: rong");
      check(isValidEmail("abc@gmail.com"),
            "isValidEmail: hop le");
      check(!isValidEmail("@gmail.com"),
            "isValidEmail: thieu ben trai");
      check(!isValidEmail("abc@"),
            "isValidEmail: thieu ben phai");
      check(!isValidEmail("abc@@gmail.com"),
            "isValidEmail: nhieu @");
}

void testNameTree() {
    std::cout << "\n=== 2. KIEM THU NAMETREE ===\n";
    NameTree t;

    // 1. Kiem thu cay rong (ca bien)
    check(t.size() == 0, "Cay rong: size = 0");
    check(t.height() == 0, "Cay rong: height = 0");
    check(t.find("An") == nullptr, "Cay rong: find tra ve nullptr");
    check(t.remove("An") == false, "Cay rong: remove tra ve false");

    // 2. Chen du lieu tao cau truc cay de test du 3 truong hop xoa:
    //          "Dung" (Goc)
    //         /            \
    //     "Binh"          "Lan"
    //     /    \          /
    //  "An"  "Cuong"   "Hoa"
    Contact* cDung  = createContact("Dung", "dung@mail.com", GROUP_WORK);
    Contact* cBinh  = createContact("Binh", "binh@mail.com", GROUP_FRIEND);
    Contact* cLan   = createContact("Lan", "lan@mail.com", GROUP_FAMILY);
    Contact* cAn    = createContact("An", "an@mail.com", GROUP_FAMILY);
    Contact* cCuong = createContact("Cuong", "cuong@mail.com", GROUP_WORK);
    Contact* cHoa   = createContact("Hoa", "hoa@mail.com", GROUP_FRIEND);

    check(t.insert(cDung) == true, "Insert goc: Dung");
    check(t.insert(cBinh) == true, "Insert trai Dung: Binh");
    check(t.insert(cLan) == true, "Insert phai Dung: Lan");
    check(t.insert(cAn) == true, "Insert trai Binh: An");
    check(t.insert(cCuong) == true, "Insert phai Binh: Cuong");
    check(t.insert(cHoa) == true, "Insert trai Lan: Hoa");

    check(t.size() == 6, "Size sau khi chen 6 nut = 6");
    check(t.height() == 3, "Height cay hien tai = 3");

    // 3. Kiem thu chen trung ten (ca sai)
    Contact* cDup = createContact("BINH", "dup@mail.com", GROUP_FRIEND);
    bool okDup = t.insert(cDup);
    check(okDup == false, "Tu choi ten trung (khong phan biet hoa thuong)");
    if (!okDup) {
        freeContact(cDup); // Cay khong nhan nen phai tu giai phong
    }
    check(t.size() == 6, "Size van = 6 sau khi tu choi ten trung");

    // 4. Kiem thu tim kiem chinh xac
    check(t.find("cuong") == cCuong, "Find: tim thay Cuong (khong phan biet hoa thuong)");
    check(t.find("KhongCo") == nullptr, "Find: ten khong ton tai -> nullptr");

    // 5. In kiem tra truc quan (LNR, theo nhom, tien to)
    std::cout << "\n[In danh ba A-Z (LNR) - Ky vong: An, Binh, Cuong, Dung, Hoa, Lan]:\n";
    t.printInOrder();

    std::cout << "\n[In nhom Ban be (GROUP_FRIEND) - Ky vong: Binh, Hoa]:\n";
    t.printByGroup(GROUP_FRIEND);

    std::cout << "\n[Tim tien to 'B' - Ky vong: Binh]:\n";
    t.searchPrefix("B");
    std::cout << "\n";

    // 6. Kiem thu xoa 3 truong hop
    // Luu y: NameTree::remove chi delete TreeNode, KHONG delete Contact -> ta tu freeContact
    // Truong hop 1: Xoa nut la ("An")
    check(t.remove("An") == true, "Remove (TH1 - nut la): An");
    check(t.find("An") == nullptr, "Kiem tra An khong con tren cay");
    check(t.size() == 5, "Size sau khi xoa An = 5");
    freeContact(cAn);

    // Truong hop 2: Xoa nut co 1 con ("Lan" chi co con trai la "Hoa")
    check(t.remove("Lan") == true, "Remove (TH2 - 1 con): Lan");
    check(t.find("Lan") == nullptr, "Kiem tra Lan khong con tren cay");
    check(t.find("Hoa") == cHoa, "Con cua Lan (Hoa) van con tren cay");
    check(t.size() == 4, "Size sau khi xoa Lan = 4");
    freeContact(cLan);

    // Truong hop 3: Xoa nut co 2 con & la nut goc ("Dung")
    check(t.remove("Dung") == true, "Remove (TH3 - 2 con / goc): Dung");
    check(t.find("Dung") == nullptr, "Kiem tra Dung khong con tren cay");
    check(t.find("Binh") == cBinh && t.find("Cuong") == cCuong && t.find("Hoa") == cHoa,
          "Cac nut con lai van giu dung lien ket");
    check(t.size() == 3, "Size sau khi xoa Dung = 3");
    freeContact(cDung);

    // 7. Giai phong toan bo cay con lai (Binh, Cuong, Hoa)
    t.destroyAll();
    check(t.size() == 0, "destroyAll: size = 0");
    check(t.height() == 0, "destroyAll: height = 0");
    check(t.find("Binh") == nullptr, "destroyAll: find tra ve nullptr");

    // 8. Ca bien: Chen theo thu tu tang dan (A, B, C, D) -> cay lech thanh "cai que"
    NameTree skewed;
    skewed.insert(createContact("A", "", GROUP_FAMILY));
    skewed.insert(createContact("B", "", GROUP_FAMILY));
    skewed.insert(createContact("C", "", GROUP_FAMILY));
    skewed.insert(createContact("D", "", GROUP_FAMILY));
    check(skewed.size() == 4 && skewed.height() == 4,
          "Cay lech (A, B, C, D): height = 4 (truong hop xau nhat O(n))");
    skewed.destroyAll();
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
    testStringUtils();
    testNameTree();

    std::cout << "\n========================================\n";
    std::cout << (failed == 0 ? "Tất cả test đạt\n" : "Có test lỗi\n");
    return failed;
}
