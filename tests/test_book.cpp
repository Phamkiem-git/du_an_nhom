// ============================================================
// File      : tests/test_book.cpp
// Phụ trách : THÀNH VIÊN A
// Vai trò   : Test cho AddressBook (chỗ nối bảng băm A và cây tên B).
// Chạy      : make test_book   (chạy ở thư mục gốc của dự án để đọc được data/contacts_large.txt)
// ============================================================
#include <iostream>
#include "core/Contact.h"
#include "core/PhoneList.h"
#include "core/AddressBook.h"
#include "io/FileManager.h"

using namespace std;

static int failed = 0;

static void check(bool cond, const char* msg) {
    cout << (cond ? "[ OK ] " : "[FAIL] ") << msg << "\n";
    if (!cond) failed++;
}

// Tạo nhanh một Contact có tối đa ba số (số rỗng thì bỏ qua).
static Contact* make(const string& name, const string& p1, const string& p2 = "",
                     const string& email = "", int group = GROUP_FRIEND) {
    Contact* c = createContact(name, email, group);
    if (!p1.empty()) addPhone(c, p1);
    if (!p2.empty()) addPhone(c, p2);
    return c;
}

static void testAdd() {
    cout << "\n=== 1. addContact ===\n";
    AddressBook book;

    Contact* an = make("Nguyen Van An", "0901111111", "0902222222", "an@mail.com");
    check(book.addContact(an), "them lien he hop le (2 so)");
    check(book.size() == 1, "size = 1");
    check(book.findByPhone("0901111111") == an, "tra theo so thu nhat -> An");
    check(book.findByPhone("0902222222") == an, "tra theo so thu hai -> An (cung mot Contact)");
    check(book.findByName("nguyen van an") == an, "tra theo ten (khong phan biet hoa thuong)");

    // --- các trường hợp sai: addContact trả false và KHÔNG nhận c, nên test tự free ---
    Contact* dupPhone = make("Le Binh", "0903333333", "0901111111");   // số thứ hai trùng với An
    check(!book.addContact(dupPhone), "tu choi: trung so voi lien he khac");
    check(book.findByPhone("0903333333") == nullptr, "thanh nhat quan: so khong trung cua lien he bi tu choi KHONG bi chen vao bang bam");
    check(book.findByName("Le Binh") == nullptr, "lien he bi tu choi khong nam trong cay");
    freeContact(dupPhone);

    Contact* dupName = make("NGUYEN VAN AN", "0904444444");
    check(!book.addContact(dupName), "tu choi: trung ten (khong phan biet hoa thuong)");
    check(book.findByPhone("0904444444") == nullptr, "trung ten: so cua lien he bi tu choi khong bi chen vao bang bam");
    freeContact(dupName);

    Contact* badPhone = make("Cuong", "09abc12345");
    check(!book.addContact(badPhone), "tu choi: so co chu cai");
    freeContact(badPhone);

    Contact* shortPhone = make("Dung", "12345");
    check(!book.addContact(shortPhone), "tu choi: so qua ngan");
    freeContact(shortPhone);

    Contact* noPhone = make("Hoa", "");
    check(!book.addContact(noPhone), "tu choi: lien he khong co so nao");
    freeContact(noPhone);

    Contact* badGroup = make("Lan", "0905555555", "", "", 7);
    check(!book.addContact(badGroup), "tu choi: nhom ngoai mien");
    freeContact(badGroup);

    Contact* badEmail = make("Minh", "0906666666", "", "khong-co-a-cong");
    check(!book.addContact(badEmail), "tu choi: email sai dinh dang");
    freeContact(badEmail);

    Contact* sameTwice = make("Nam", "0907777777", "0907777777");
    check(!book.addContact(sameTwice), "tu choi: mot so xuat hien hai lan trong cung lien he");
    freeContact(sameTwice);

    check(!book.addContact(nullptr), "tu choi: con tro rong");
    check(book.size() == 1, "sau cac lan tu choi, size van = 1");
}

static void testPhones() {
    cout << "\n=== 2. addPhoneTo / removePhoneFrom ===\n";
    AddressBook book;
    Contact* an = make("An", "0901111111");
    Contact* binh = make("Binh", "0902222222");
    book.addContact(an);
    book.addContact(binh);

    check(book.addPhoneTo("An", "0903333333"), "them so moi cho An");
    check(book.findByPhone("0903333333") == an, "so moi tra ra An");
    check(countPhones(an) == 2, "An co 2 so");
    check(!book.addPhoneTo("An", "0902222222"), "tu choi: so da thuoc ve Binh");
    check(!book.addPhoneTo("An", "0903333333"), "tu choi: so da co san trong An");
    check(!book.addPhoneTo("An", "abc"), "tu choi: so khong hop le");
    check(!book.addPhoneTo("Khong Co", "0908888888"), "tu choi: khong co lien he ten do");
    check(book.findByPhone("0908888888") == nullptr, "lien he khong ton tai: so khong bi chen vao bang bam");

    check(book.removePhoneFrom("An", "0903333333"), "xoa bot so cua An");
    check(book.findByPhone("0903333333") == nullptr, "so da xoa khong con tra ra");
    check(countPhones(an) == 1, "An con 1 so");
    check(!book.removePhoneFrom("An", "0901111111"), "tu choi: khong cho xoa so cuoi cung");
    check(book.findByPhone("0901111111") == an, "so cuoi cung van con");
    check(!book.removePhoneFrom("An", "0902222222"), "tu choi: so khong thuoc An");
    check(book.addPhoneTo("Binh", "0903333333"), "so da xoa khoi An co the gan cho Binh");
}

static void testRemove() {
    cout << "\n=== 3. removeContact ===\n";
    AddressBook book;
    book.addContact(make("An", "0901111111", "0902222222"));
    book.addContact(make("Binh", "0903333333"));

    check(book.removeContact("an"), "xoa An (go ten khong phan biet hoa thuong)");
    check(book.size() == 1, "size = 1");
    check(book.findByName("An") == nullptr, "An khong con trong cay");
    check(book.findByPhone("0901111111") == nullptr, "so 1 cua An khong con trong bang bam");
    check(book.findByPhone("0902222222") == nullptr, "so 2 cua An khong con trong bang bam");
    check(!book.removeContact("An"), "xoa lai An -> false");
    check(book.findByPhone("0903333333") != nullptr, "Binh khong bi anh huong");

    // Số của An đã được trả tự do, thêm lại cho người khác được.
    check(book.addContact(make("Cuong", "0901111111")), "so cua An da xoa co the dung cho lien he moi");
}

static void testMerge() {
    cout << "\n=== 4. mergeInto ===\n";
    AddressBook book;
    Contact* an = make("An", "0901111111");                       // email rong
    Contact* binh = make("Binh", "0909999999");
    book.addContact(an);
    book.addContact(binh);

    // Gộp thành công: extra có số mới, một số đã thuộc chính An, và có email.
    Contact* extra = make("An", "0902222222", "0901111111", "an@mail.com");
    check(book.mergeInto(an, extra), "gop thanh cong (extra duoc mergeInto giai phong)");
    check(countPhones(an) == 2, "An co 2 so (so trung voi chinh An bi bo qua)");
    check(book.findByPhone("0902222222") == an, "so moi tra ra An");
    check(an->email == "an@mail.com", "An email rong nen lay email cua extra");

    // Gộp thất bại: extra có số thuộc về Binh. Existing không được thay đổi dở dang.
    Contact* bad = make("An", "0903333333", "0909999999");          // số thứ hai thuộc Binh
    check(!book.mergeInto(an, bad), "tu choi gop: co so thuoc lien he khac");
    check(countPhones(an) == 2, "gop that bai: An khong bi them so nao");
    check(book.findByPhone("0903333333") == nullptr, "gop that bai: so khong xung dot cung khong bi chen");
    freeContact(bad);   // mergeInto thất bại thì chưa free extra

    check(!book.mergeInto(an, an), "tu choi: gop mot lien he voi chinh no");
    check(!book.mergeInto(nullptr, nullptr), "tu choi: con tro rong");
}

static void testLarge() {
    cout << "\n=== 5. Nap 10.000 lien he tu tep ===\n";
    AddressBook book;
    if (!loadContacts(book, "data/contacts_large.txt")) {
        cout << "(bo qua: khong mo duoc data/contacts_large.txt, hay chay 'make gen_data' va chay test o thu muc goc)\n";
        return;
    }
    check(book.size() == 10000, "nap du 10000 lien he");
    check(book.findByPhone("0900000000") != nullptr, "tra duoc so dau tien");
    check(book.findByPhone("0900009999") != nullptr, "tra duoc so cuoi cung");
    check(book.findByPhone("0912345678") == nullptr, "so khong co trong du lieu -> nullptr");
    book.printStats();
}

int main() {
    testAdd();
    testPhones();
    testRemove();
    testMerge();
    testLarge();

    cout << "\n========================================\n";
    cout << (failed == 0 ? "Tất cả test đạt\n" : "Có test lỗi\n");
    return failed;
}