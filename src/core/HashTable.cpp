// ============================================================
// File      : src/core/HashTable.cpp
// Phụ trách : THÀNH VIÊN A
// ============================================================
#include "core/HashTable.h"

// Hàm trợ giúp riêng của file này (không khai báo trong .h): n có phải số nguyên tố không?
static bool isPrime(int n) {
    if (n < 2) return false;
    for (int d = 2; d * d <= n; d++) {
        if (n % d == 0) return false;   // có ước khác 1 và n -> không nguyên tố
    }
    return true;
}

// Số nguyên tố nhỏ nhất >= n.
int HashTable::nextPrime(int n) {
    if (n < 2) n = 2;
    while (!isPrime(n)) n++;
    return n;
}

HashTable::HashTable(int initialSize)
    : table(nullptr), cap(0), count(0), deletedCount(0), totalProbes(0), totalOps(0) {
    cap = nextPrime(initialSize);   // luôn dùng kích thước nguyên tố
    table = new Slot[cap];          // mỗi Slot tự đặt state = EMPTY (nhờ constructor của Slot)
}

HashTable::~HashTable() {
    delete[] table;   // chỉ giải phóng mảng ô. KHÔNG delete các Contact: đó là việc của AddressBook.
}

// Hàm băm chính (Horner): coi chuỗi như một số viết theo "cơ số 31".
// Trả về vị trí bắt đầu, nằm trong [0, cap - 1].
int HashTable::hash1(const string& key) const {
    long long h = 0;
    for (int i = 0; i < (int)key.length(); i++) {
        h = (h * 31 + (unsigned char)key[i]) % cap;   // lấy % mỗi bước để không bao giờ tràn số
    }
    return (int)h;
}

// Hàm băm thứ hai: cho ra BƯỚC NHẢY khi đụng độ. Phải nằm trong [1, cap - 1], KHÔNG BAO GIỜ bằng 0.
int HashTable::hash2(const string& key) const {
    long long h = 0;
    for (int i = 0; i < (int)key.length(); i++) {
        h = (h * 131 + (unsigned char)key[i]) % (cap - 1);   // cơ số khác hash1 để hai hàm "lệch" nhau
    }
    return (int)h + 1;   // cộng 1 để chắc chắn khác 0
}

// Thêm (phone -> c). Trả về false nếu phone đã có trong bảng, hoặc bảng đã đầy.
bool HashTable::insert(const string& phone, Contact* c) {
    // 1. Bảng sắp đầy thì mở rộng trước (rehash() sẽ được viết ở lượt (c); hiện còn rỗng).
    if ((double)(count + deletedCount + 1) / cap > 0.7) rehash();

    int pos = hash1(phone);       // ô bắt đầu
    int step = hash2(phone);      // bước nhảy khi đụng độ
    int firstDeleted = -1;        // vị trí ô DELETED đầu tiên gặp trên đường dò (-1 = chưa gặp)
    int target = -1;              // ô sẽ chèn vào

    // 2. Dò tối đa cap ô. Đi qua đủ cap ô thì chắc chắn đã xem hết bảng (cap nguyên tố).
    for (int k = 0; k < cap; k++) {
        Slot& s = table[pos];
        if (s.state == OCCUPIED) {
            if (s.key == phone) return false;      // số đã tồn tại -> từ chối
        } else if (s.state == DELETED) {
            if (firstDeleted == -1) firstDeleted = pos;   // nhớ lại, nhưng VẪN dò tiếp
        } else {                                   // EMPTY: hết đường dò, khóa chắc chắn chưa có
            target = (firstDeleted != -1) ? firstDeleted : pos;
            break;
        }
        pos = (pos + step) % cap;
    }

    // 3. Dò hết cap ô mà không gặp EMPTY: chỉ còn chỗ nếu đã gặp ô DELETED.
    if (target == -1) target = firstDeleted;
    if (target == -1) return false;                // bảng đầy

    // 4. Chèn vào ô đã chọn.
    if (table[target].state == DELETED) deletedCount--;   // ô này không còn là "đã xóa" nữa
    table[target].key = phone;
    table[target].contact = c;
    table[target].state = OCCUPIED;
    count++;
    return true;
}

// Tìm theo số điện thoại. Trả về nullptr nếu không có.
// Mỗi lần gọi find được tính là một "thao tác" (totalOps) để thống kê số lần dò trung bình khi tra cứu.
Contact* HashTable::find(const string& phone) {
    totalOps++;
    int pos = hash1(phone);
    int step = hash2(phone);

    for (int k = 0; k < cap; k++) {
        totalProbes++;                              // mỗi ô xem qua là một lần dò
        Slot& s = table[pos];
        if (s.state == EMPTY) return nullptr;       // gặp ô trống: khóa không tồn tại
        if (s.state == OCCUPIED && s.key == phone) return s.contact;
        pos = (pos + step) % cap;                   // ô DELETED hoặc khác khóa: dò tiếp
    }
    return nullptr;                                 // đã xem hết cap ô mà không thấy
}

bool HashTable::remove(const string& phone) {
    // TODO (A): lượt (c)
    return false;
}

void HashTable::rehash() {
    // TODO (A): lượt (c)
}

double HashTable::loadFactor() const {
    // TODO (A): lượt (c)
    return 0.0;
}

double HashTable::avgProbes() const {
    // TODO (A): lượt (c)
    return 0.0;
}