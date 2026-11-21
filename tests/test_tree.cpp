#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include "core/AddressBook.h"
#include "core/Contact.h"
#include "core/NameTree.h"
#include "core/PhoneList.h"
#include "core/StringUtils.h"
#include "io/FileManager.h"

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
      check(compareNames("An", "Anh") < 0,
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

    /* 2. Chen du lieu tao cau truc cay de test du 3 truong hop xoa:
    //          "Dung" (Goc)
    //         /            \
    //     "Binh"          "Lan"
    //     /    \          /
    //  "An"  "Cuong"   "Hoa"
    */
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

// ------------------------------------------------------------
// [NÂNG CAO] Tìm kiếm không phân biệt dấu tiếng Việt + nhập/xuất CSV
// ------------------------------------------------------------

// Chạy f() với std::cout bị chuyển hướng vào chuỗi, trả về những gì f() đã in ra (để kiểm tra các hàm in).
// Dùng bằng cách gọi captureSearch / captureInOrder bên dưới.
static std::string captureSearch(const NameTree& t, const std::string& prefix) {
    std::ostringstream buf;
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
    t.searchPrefix(prefix);
    std::cout.rdbuf(old);
    return buf.str();
}

static std::string captureInOrder(const NameTree& t) {
    std::ostringstream buf;
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
    t.printInOrder();
    std::cout.rdbuf(old);
    return buf.str();
}

// Số dòng (số liên hệ) trong kết quả in ra.
static int countLines(const std::string& s) {
    int n = 0;
    for (size_t i = 0; i < s.length(); i++)
        if (s[i] == '\n') n++;
    return n;
}

// Tạo Contact với một danh sách số (kết thúc bằng nullptr), tiện cho test.
static Contact* makeContact(const char* name, const char* email, int group,
                            const char* p1, const char* p2 = nullptr) {
    Contact* c = createContact(name, email, group);
    addPhone(c, p1);
    if (p2 != nullptr) addPhone(c, p2);
    return c;
}

void testVietnameseSearch() {
    std::cout << "\n=== 3. [NANG CAO] TIM KIEM KHONG PHAN BIET DAU ===\n";

    // --- removeDiacritics ---
    check(removeDiacritics("Nguy\xE1\xBB\x85n V\xC4\x83n \xC4\x90\xE1\xBB\xA9" "c") == "Nguyen Van Duc",
          "removeDiacritics: Nguyễn Văn Đức -> Nguyen Van Duc");
    check(removeDiacritics("\xC4\x90\xE1\xBA\xB6NG TH\xE1\xBB\x8A H\xC6\xAF\xC6\xA0NG") == "DANG THI HUONG",
          "removeDiacritics: giu nguyen chu HOA (ĐẶNG THỊ HƯƠNG)");
    check(removeDiacritics("Anna-Lee 123") == "Anna-Lee 123",
          "removeDiacritics: chuoi khong dau giu nguyen");
    check(removeDiacritics("") == "", "removeDiacritics: chuoi rong");

    // --- compareNames / startsWith ---
    check(compareNames("Nguy\xE1\xBB\x85n", "nguyen") == 0,
          "compareNames: Nguyễn == nguyen (khong phan biet dau va hoa thuong)");
    check(compareNames("\xC4\x90\xE1\xBB\xA9" "c", "Duc") == 0,
          "compareNames: Đức == Duc (đ -> d)");
    check(compareNames("\xC4\x90\xE1\xBB\xA9" "c", "Dung") < 0,
          "compareNames: Đức dung truoc Dung (duc < dung)");
    check(compareNames("\xC3\x82n", "Anh") < 0,
          "compareNames: Ân dung truoc Anh (an < anh)");
    check(startsWith("Nguy\xE1\xBB\x85n An", "nguyen"),
          "startsWith: 'nguyen' khop 'Nguyễn An'");
    check(startsWith("Nguyen An", "NGUY\xE1\xBB\x84N"),
          "startsWith: 'NGUYỄN' khop 'Nguyen An' (go co dau, ten khong dau)");
    check(startsWith("L\xC3\xAA", "L\xE1\xBB\x87"),
          "startsWith: Lê khop tien to Lệ (cung chu goc e)");
    check(!startsWith("L\xC3\xAA", "L\xC3\xAAx"),
          "startsWith: tien to dai hon chuoi -> false");
    check(startsWith("An", ""), "startsWith: tien to rong -> true");
    // Chữ viết kiểu tổ hợp: "e" + dấu mũ rời + dấu ngã rời (NFD) phải bằng "e".
    check(compareNames("Nguye\xCC\x82\xCC\x83n", "Nguyen") == 0,
          "compareNames: chu to hop (NFD) == chu khong dau");
    // Chuỗi UTF-8 hỏng không được làm chương trình dừng hay đọc tràn.
    check(compareNames("abc\xE1\xBB", "abc") != 0 && !startsWith("\xFF\xFE", "x"),
          "UTF-8 hong: khong crash, khong coi la bang nhau");

    std::string withBom = "\xEF\xBB\xBF" "An";
    stripBom(withBom);
    check(withBom == "An", "stripBom: bo 3 byte BOM o dau");
    std::string noBom = "An";
    stripBom(noBom);
    check(noBom == "An", "stripBom: khong co BOM thi giu nguyen");

    // --- Cây tên ---
    NameTree t;
    t.insert(makeContact("Nguy\xE1\xBB\x85n V\xC4\x83n An", "", GROUP_FRIEND, "0901000001"));
    t.insert(makeContact("Nguy\xE1\xBB\x85n Th\xE1\xBB\x8B B\xC3\xACnh", "", GROUP_FAMILY, "0901000002"));
    t.insert(makeContact("Nguyen Minh Khoa", "", GROUP_WORK, "0901000003"));
    t.insert(makeContact("\xC4\x90\xE1\xBB\xA9" "c", "", GROUP_WORK, "0901000004"));
    t.insert(makeContact("D\xC5\xA9ng", "", GROUP_FRIEND, "0901000005"));
    t.insert(makeContact("D\xC6\xB0\xC6\xA1ng", "", GROUP_FRIEND, "0901000006"));
    t.insert(makeContact("Duy", "", GROUP_FRIEND, "0901000007"));
    t.insert(makeContact("Lan", "", GROUP_FAMILY, "0901000008"));

    check(t.find("nguyen van an") != nullptr, "find: 'nguyen van an' tim ra 'Nguyễn Văn An'");
    check(t.find("NGUY\xE1\xBB\x84N V\xC4\x82N AN") != nullptr, "find: ten viet hoa co dau");
    check(t.find("Nguyen Van Anh") == nullptr, "find: ten gan giong nhung khac -> khong thay");

    Contact* dup = makeContact("nguyen van an", "", GROUP_FRIEND, "0909999999");
    check(!t.insert(dup), "insert: 'nguyen van an' bi tu choi vi trung 'Nguyễn Văn An'");
    freeContact(dup);   // insert bị từ chối thì cây không giữ dup -> phải tự giải phóng
    check(t.size() == 8, "insert bi tu choi: size van la 8");

    std::string r = captureSearch(t, "nguyen");
    check(countLines(r) == 3, "searchPrefix: 'nguyen' ra dung 3 nguoi (Nguyễn x2 va Nguyen)");
    check(captureSearch(t, "NGUY\xE1\xBB\x84N") == r, "searchPrefix: go co dau cho ket qua giong go khong dau");
    check(countLines(captureSearch(t, "d")) == 4, "searchPrefix: 'd' ra Đức, Dũng, Dương, Duy");
    check(countLines(captureSearch(t, "\xC4\x91")) == 4, "searchPrefix: 'đ' cung ra 4 nguoi nhu 'd'");
    check(countLines(captureSearch(t, "duo")) == 1, "searchPrefix: 'duo' chi ra Dương");
    check(countLines(captureSearch(t, "zzz")) == 0, "searchPrefix: khong khop -> khong in gi");

    // Thứ tự A-Z theo chữ gốc: duc < dung < duong < duy
    std::string all = captureInOrder(t);
    size_t pDuc = all.find("\xC4\x90\xE1\xBB\xA9" "c");
    size_t pDung = all.find("D\xC5\xA9ng");
    size_t pDuong = all.find("D\xC6\xB0\xC6\xA1ng");
    size_t pDuy = all.find("Duy");
    check(pDuc < pDung && pDung < pDuong && pDuong < pDuy,
          "printInOrder: Đức < Dũng < Dương < Duy (sap theo chu goc)");

    Contact* removedContact = t.find("duc");
    check(t.remove("duc"), "remove: 'duc' go duoc 'Đức'");
    check(t.find("\xC4\x90\xE1\xBB\xA9" "c") == nullptr && t.size() == 7, "remove: Đức da bi go, size = 7");
    freeContact(removedContact);   // NameTree::remove KHÔNG free Contact (quy ước sở hữu) nên test tự free
    t.destroyAll();
}

void testCsv() {
    std::cout << "\n=== 4. [NANG CAO] NHAP / XUAT CSV ===\n";

    // --- parseCsvLine ---
    Contact* c = parseCsvLine("Nguy\xE1\xBB\x85n V\xC4\x83n An,0901234567;0912345678,an@mail.com,1");
    check(c != nullptr, "parseCsvLine: dong hop le");
    if (c != nullptr) {
        check(c->name == "Nguy\xE1\xBB\x85n V\xC4\x83n An" && c->email == "an@mail.com" && c->group == GROUP_FRIEND,
              "parseCsvLine: dung ten, email, nhom");
        check(countPhones(c) == 2 && hasPhone(c, "0901234567") && hasPhone(c, "0912345678"),
              "parseCsvLine: tach du 2 so tu o 'a;b'");
        freeContact(c);
    }

    c = parseCsvLine("\"Le, \"\"Bo\"\" Van\",0933333333,,2");
    check(c != nullptr && c->name == "Le, \"Bo\" Van" && c->email.empty() && c->group == GROUP_WORK,
          "parseCsvLine: o co dau phay va nhay kep (\"\" -> \"), email rong");
    freeContact(c);

    c = parseCsvLine("Nguy\xE1\xBB\x85n An;0901234567,0912345678;an@mail.com;0", ';');
    check(c != nullptr && countPhones(c) == 2 && c->group == GROUP_FAMILY,
          "parseCsvLine: dau phan cach ';' (Excel tieng Viet), so cach nhau bang ','");
    freeContact(c);

    check(parseCsvLine("An,0901234567,an@mail.com") == nullptr, "parseCsvLine: thieu cot -> nullptr");
    check(parseCsvLine("An,0901234567,an@mail.com,1,them") == nullptr, "parseCsvLine: thua cot -> nullptr");
    check(parseCsvLine("\"An,0901234567,an@mail.com,1") == nullptr, "parseCsvLine: nhay kep khong dong -> nullptr");
    check(parseCsvLine("\"An\"x,0901234567,,1") == nullptr, "parseCsvLine: ky tu la sau nhay dong -> nullptr");
    check(parseCsvLine(",0901234567,,1") == nullptr, "parseCsvLine: ten rong -> nullptr");
    check(parseCsvLine("An,0901234567,,x") == nullptr, "parseCsvLine: nhom khong phai so -> nullptr");
    check(parseCsvLine("An,0901234567,,") == nullptr, "parseCsvLine: nhom rong -> nullptr");

    // --- writeCsvLine ---
    Contact* w = makeContact("Le, \"Bo\" Van", "", GROUP_WORK, "0933333333", "0944444444");
    std::ostringstream out;
    writeCsvLine(out, w);
    check(out.str() == "\"Le, \"\"Bo\"\" Van\",0944444444;0933333333,,2\n",
          "writeCsvLine: bo nhay kep o ten, so cach nhau ';', email rong");
    freeContact(w);

    // --- Nhập/xuất qua tệp thật ---
    const char* file = "test_roundtrip.csv";
    AddressBook a;
    check(a.addContact(makeContact("Nguy\xE1\xBB\x85n V\xC4\x83n An", "an@mail.com", GROUP_FRIEND, "0901000001", "0912000001")), "chuan bi: them An");
    check(a.addContact(makeContact("Le, \"Bo\" Van", "", GROUP_WORK, "0901000002")), "chuan bi: them ten co dau phay");
    check(a.addContact(makeContact("\xC4\x90\xE1\xBB\x97 Minh \xC4\x90\xE1\xBB\xA9" "c", "duc@mail.com", GROUP_FAMILY, "0901000003")), "chuan bi: them Đỗ Minh Đức");

    check(exportCsv(a, file), "exportCsv: ghi tep thanh cong");
    check(!exportCsv(a, "khong_co_thu_muc/x.csv"), "exportCsv: duong dan khong ghi duoc -> false");

    AddressBook b;
    int added = -1, skipped = -1;
    check(importCsv(b, file, added, skipped), "importCsv: mo duoc tep vua xuat");
    check(added == 3 && skipped == 0, "importCsv: them 3, bo qua 0 (dong tieu de khong tinh la loi)");
    check(b.size() == 3, "importCsv: danh ba moi co 3 lien he");
    Contact* an = b.findByName("nguyen van an");
    check(an != nullptr && countPhones(an) == 2 && an->email == "an@mail.com" && an->group == GROUP_FRIEND,
          "round-trip: Nguyễn Văn An giu du 2 so, email, nhom (tim bang ten khong dau)");
    check(b.findByName("Le, \"Bo\" Van") != nullptr, "round-trip: ten co dau phay va nhay kep con nguyen");
    check(b.findByPhone("0901000003") != nullptr && b.findByPhone("0901000003")->name == "\xC4\x90\xE1\xBB\x97 Minh \xC4\x90\xE1\xBB\xA9" "c",
          "round-trip: tra theo so van ra Đỗ Minh Đức");

    check(importCsv(b, file, added, skipped) && added == 0 && skipped == 3,
          "importCsv lan 2: ca 3 lien he trung -> them 0, bo qua 3, khong ro ri");
    check(b.size() == 3, "importCsv lan 2: danh ba van 3 lien he");
    check(!importCsv(b, "khong_ton_tai.csv", added, skipped), "importCsv: tep khong ton tai -> false");
    std::remove(file);

    // Tệp kiểu Excel bản tiếng Việt: có BOM, ngăn cách bằng ';', không dòng tiêu đề, có dòng trống và dòng lỗi.
    {
        std::ofstream f(file);
        f << "\xEF\xBB\xBF" "Ph\xE1\xBA\xA1m Qu\xE1\xBB\xB3nh Anh;0901234567;;1\n";
        f << "\n";
        f << "dong loi khong du cot\n";
        f << "Tr\xE1\xBA\xA7n Thi Ngoc;0912345678,0923456789;ngoc@mail.com;0\n";
        f << "Tran Thi Ngoc;0933333333;;1\n";      // trùng tên với dòng trên
        f << "Vo Van Tam;0912345678;;2\n";          // trùng số với Trần Thi Ngoc
        f << "Hoang Van E;12;;2\n";                  // số điện thoại không hợp lệ
    }
    AddressBook x;
    check(importCsv(x, file, added, skipped), "importCsv (Excel VN): mo duoc tep");
    check(added == 2 && skipped == 4, "importCsv (Excel VN): them 2; bo qua 4 (loi cot, trung ten, trung so, so sai)");
    check(x.findByName("pham quynh anh") != nullptr, "importCsv (Excel VN): BOM o dong dau khong lam hong ten");
    check(x.findByName("Tran Thi Ngoc") != nullptr && countPhones(x.findByName("Tran Thi Ngoc")) == 2,
          "importCsv (Excel VN): lien he nhieu so duoc doc du");
    std::remove(file);

    // Tệp .txt lưu từ Notepad có BOM ở đầu: dòng đầu vẫn đọc đúng.
    {
        std::ofstream f(file);
        f << "\xEF\xBB\xBF" "Ph\xE1\xBA\xA1m Qu\xE1\xBB\xB3nh Anh|0901234567||1\n";
    }
    AddressBook y;
    check(loadContacts(y, file) && y.findByName("Pham Quynh Anh") != nullptr,
          "loadContacts: tep .txt co BOM van doc dung dong dau");
    std::remove(file);
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
    testVietnameseSearch();   // [NÂNG CAO]
    testCsv();                // [NÂNG CAO]

    std::cout << "\n========================================\n";
    std::cout << (failed == 0 ? "Tất cả test đạt\n" : "Có test lỗi\n");
    return failed;
}
