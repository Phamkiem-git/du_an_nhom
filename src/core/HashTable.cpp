// ============================================================
// File      : src/core/HashTable.cpp
// Phụ trách : THÀNH VIÊN A
// ============================================================
#include "core/HashTable.h"

HashTable::HashTable(int initialSize)
    : table(nullptr), cap(0), count(0), deletedCount(0), totalProbes(0), totalOps(0) {
    // TODO (A): cap = initialSize; table = new Slot[cap];
    //   (Slot đã có constructor đặt state = EMPTY nên không cần vòng lặp khởi tạo.)
}

HashTable::~HashTable() {
    delete[] table;   // chỉ giải phóng mảng ô. KHÔNG delete các Contact: đó là việc của AddressBook.
}

int HashTable::hash1(const string& key) const {
    // TODO (A): hàm băm Horner: h = h * 31 + (mã ký tự); nhớ lấy % cap để không tràn.
    return 0;
}

int HashTable::hash2(const string& key) const {
    // TODO (A): ví dụ 1 + (giá trị băm khác) % (cap - 1). Kết quả KHÔNG ĐƯỢC bằng 0.
    return 1;
}

int HashTable::nextPrime(int n) {
    // TODO (A): tăng n dần đến khi gặp số nguyên tố (kiểm tra chia thử đến căn n).
    return n;
}

bool HashTable::insert(const string& phone, Contact* c) {
    // TODO (A):
    //   1. Nếu (count + deletedCount + 1) / cap > 0.7 -> rehash().
    //   2. i = hash1(phone); step = hash2(phone).
    //   3. Dò: i, i+step, i+2*step, ... (mod cap), mỗi bước tăng totalProbes.
    //        - Gặp ô OCCUPIED có key == phone  -> trùng -> return false.
    //        - Nhớ vị trí ô DELETED đầu tiên gặp (có thể chèn vào đó).
    //        - Gặp ô EMPTY -> dừng dò.
    //   4. Chèn vào ô DELETED đã nhớ (nếu có) hoặc ô EMPTY; count++.
    return false;
}

Contact* HashTable::find(const string& phone) {
    // TODO (A): dò như insert. Bỏ qua ô DELETED, dừng khi gặp EMPTY.
    //   Cộng totalProbes theo số ô đã xem; totalOps++.
    return nullptr;
}

bool HashTable::remove(const string& phone) {
    // TODO (A): tìm ô OCCUPIED có key == phone, đặt state = DELETED, count--, deletedCount++.
    //   KHÔNG đặt EMPTY, vì sẽ làm đứt dây dò của các khóa nằm phía sau.
    return false;
}

void HashTable::rehash() {
    // TODO (A):
    //   1. Giữ con trỏ bảng cũ và cap cũ.
    //   2. cap mới = nextPrime(2 * cap cũ); cấp phát bảng mới; đặt lại count, deletedCount.
    //   3. Với mỗi ô OCCUPIED của bảng cũ, chèn lại vào bảng mới (băm lại với cap mới).
    //   4. delete[] bảng cũ.
}

double HashTable::loadFactor() const {
    // TODO (A): trả về (double)count / cap (hoặc cộng thêm deletedCount, tùy nhóm chốt).
    return 0.0;
}

double HashTable::avgProbes() const {
    // TODO (A): tránh chia cho 0 khi totalOps == 0.
    return 0.0;
}
