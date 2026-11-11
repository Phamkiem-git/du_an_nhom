// ============================================================
// File      : src/core/HashTable.cpp
// Phụ trách : THÀNH VIÊN A
// ============================================================
#include "core/HashTable.h"

static bool isPrime(int n) {
    if (n < 2) return false;
    for (int d = 2; d * d <= n; d++) {
        if (n % d == 0) return false;
    }
    return true;
}

HashTable::HashTable(int initialSize)
    : table(nullptr), cap(0), count(0), deletedCount(0), totalProbes(0), totalOps(0) {
    cap = nextPrime(initialSize);   // luôn dùng kích thước nguyên tố
    table = new Slot[cap];  
}

HashTable::~HashTable() {
    delete[] table;   // chỉ giải phóng mảng ô. KHÔNG delete các Contact: đó là việc của AddressBook.
}

int HashTable::hash1(const string& key) const {
    long long h = 0;
    for (int i = 0; i < (int)key.length(); i++) {
        h = (h * 31 + (unsigned char)key[i]) % cap;
    }
    return (int)h;
}

int HashTable::hash2(const string& key) const {
    long long h = 0;
    for (int i = 0; i < (int)key.length(); i++) {
        h = (h * 131 + (unsigned char)key[i]) % (cap - 1);
    }
    return (int)h + 1;   // cộng 1 để chắc chắn khác 0
}

int HashTable::nextPrime(int n) {
    if (n < 2) n = 2;
    while (!isPrime(n)) n++;
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
