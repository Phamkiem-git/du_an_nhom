// ============================================================
// File      : src/core/HashTable.h
// Phụ trách : THÀNH VIÊN A
// Review    : thành viên B
// Vai trò   : Bảng băm địa chỉ mở (open addressing). Khóa = số điện thoại,
//             giá trị = con trỏ tới Contact sở hữu số đó.
// Lưu ý     : Không dùng unordered_map / vector / array của STL. Mảng ô dùng new[].
// ============================================================
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include "core/Contact.h"

// Trạng thái của một ô trong bảng.
//   EMPTY    : chưa từng dùng  -> dừng dò khi gặp ô này
//   OCCUPIED : đang chứa dữ liệu
//   DELETED  : đã xóa ("tombstone") -> KHÔNG dừng dò, vì phía sau vẫn có thể còn khóa cần tìm
enum SlotState { EMPTY, OCCUPIED, DELETED };

struct Slot {
    string key;           // số điện thoại
    Contact* contact;     // KHÔNG sở hữu: bảng băm không được delete con trỏ này
    SlotState state;
    Slot() : contact(nullptr), state(EMPTY) {}
};

class HashTable {
public:
    // initialSize nên là số nguyên tố (băm kép chỉ đảm bảo duyệt hết bảng khi kích thước là số nguyên tố).
    HashTable(int initialSize = 101);
    ~HashTable();

    // Thêm (phone -> c). Trả về false nếu phone đã có trong bảng.
    // Nếu hệ số tải vượt 0.7 thì rehash.
    bool insert(const string& phone, Contact* c);

    // Tìm theo số điện thoại. Trả về nullptr nếu không có.
    Contact* find(const string& phone);

    // Xóa khóa phone (đánh dấu DELETED). Trả về false nếu không có.
    bool remove(const string& phone);

    int size() const { return count; }
    int capacity() const { return cap; }

    // Hệ số tải. Cần chốt trong nhóm: có tính cả ô DELETED hay không (ghi rõ lựa chọn vào báo cáo).
    double loadFactor() const;

    // Số lần dò trung bình mỗi thao tác (totalProbes / totalOps). Dùng cho thống kê và thực nghiệm.
    double avgProbes() const;

private:
    Slot* table;          // mảng ô, cấp phát bằng new Slot[cap]
    int cap;              // số ô
    int count;            // số ô OCCUPIED
    int deletedCount;     // số ô DELETED
    long totalProbes;     // tổng số lần dò (để tính trung bình)
    long totalOps;        // tổng số thao tác đã đếm

    int hash1(const string& key) const;   // hàm băm chính (gợi ý: Horner)
    int hash2(const string& key) const;   // bước nhảy cho băm kép, phải khác 0
    void rehash();                        // tạo bảng mới lớn hơn, chèn lại mọi khóa OCCUPIED
    static int nextPrime(int n);          // số nguyên tố nhỏ nhất >= n

    // Không cho sao chép bảng (tránh hai bảng cùng delete[] một mảng).
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
};

#endif
