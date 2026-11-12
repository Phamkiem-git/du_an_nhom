// ============================================================
// File      : tools/exp_hash.cpp
// Phụ trách : THÀNH VIÊN A
// Vai trò   : Thực nghiệm cho báo cáo (phần bảng băm):
//             - Thí nghiệm 1: thời gian tra cứu theo số, bảng băm so với duyệt tuần tự DSLK
//             - Thí nghiệm 2: số lần dò trung bình theo hệ số tải, so với công thức lý thuyết
// Chạy      : make exp_hash
//
// Ghi chú cho báo cáo: rand(), srand() chỉ dùng để chọn khóa tra cứu ngẫu nhiên;
// <chrono> (qua Timer.h) và <cmath> chỉ dùng để đo/tính, không phải cấu trúc dữ liệu.
// ============================================================
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <string>
#include "core/HashTable.h"
#include "core/Contact.h"
#include "core/Timer.h"

using namespace std;

// ------------------------------------------------------------
// Danh sách liên kết thường (chỉ dùng trong thực nghiệm), để so với bảng băm.
// Duyệt tuần tự từ đầu đến cuối: đúng cách tra cứu "không có cấu trúc đặc biệt".
// ------------------------------------------------------------
struct ListNode {
    string phone;
    ListNode* next;
};

static bool listFind(ListNode* head, const string& phone) {
    for (ListNode* p = head; p != nullptr; p = p->next) {
        if (p->phone == phone) return true;
    }
    return false;
}

static void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* nxt = head->next;
        delete head;
        head = nxt;
    }
}

// ------------------------------------------------------------
// Sinh số điện thoại KHÔNG TRÙNG, nhìn "lộn xộn" như số thật.
// Số thứ i -> (i * 7919 + 13) mod 10^8. 7919 là số nguyên tố, nguyên tố cùng nhau với 10^8,
// nên phép biến đổi này không bao giờ cho hai i khác nhau cùng một kết quả (không trùng).
// Đầu số "09" cho số ĐÃ CÓ trong bảng, đầu số "03" cho số KHÔNG CÓ (dùng để tra thất bại).
// ------------------------------------------------------------
static string padZero(long long v, int width) {
    string s = to_string(v);
    while ((int)s.length() < width) s = "0" + s;
    return s;
}

static string makePhone(long long i, const string& prefix) {
    long long v = (i * 7919 + 13) % 100000000LL;
    return prefix + padZero(v, 8);
}

// ------------------------------------------------------------
// Thí nghiệm 1: bảng băm so với duyệt tuần tự
// ------------------------------------------------------------
static void experiment1(Contact* dummy) {
    cout << "=== THI NGHIEM 1: tra cuu theo so dien thoai (tra THANH CONG) ===\n";
    cout << "Moi cach tra du nhieu luot de do duoc, roi chia ra thoi gian MOI LUOT (micro giay).\n\n";
    cout << left << setw(9) << "n"
         << setw(9) << "cap"
         << setw(8) << "load"
         << setw(10) << "avgProbe"
         << setw(16) << "bang bam (us)"
         << setw(18) << "tuan tu (us)"
         << "ti le\n";

    int sizes[3] = {1000, 10000, 100000};
    const int QUERIES_HASH = 200000;   // bảng băm rất nhanh nên cần nhiều lượt mới đo được
    const int QUERIES_LIST = 300;      // duyệt tuần tự chậm, ít lượt là đủ (n = 100000 đã ~ 15 triệu phép so sánh)

    for (int si = 0; si < 3; si++) {
        int n = sizes[si];

        // 1. Chèn n số vào bảng băm VÀ vào danh sách liên kết.
        HashTable table(101);
        ListNode* head = nullptr;
        for (int i = 0; i < n; i++) {
            string ph = makePhone(i, "09");
            table.insert(ph, dummy);
            ListNode* node = new ListNode;
            node->phone = ph;
            node->next = head;     // chèn đầu danh sách (O(1)), thứ tự không quan trọng ở đây
            head = node;
        }

        // 2. Chọn sẵn các số cần tra (ngẫu nhiên trong số đã có), để hai cách tra CÙNG tập số.
        srand(2024);
        int maxQ = (QUERIES_HASH > QUERIES_LIST) ? QUERIES_HASH : QUERIES_LIST;
        string* keys = new string[maxQ];
        for (int q = 0; q < maxQ; q++) {
            keys[q] = makePhone(rand() % n, "09");
        }

        // 3. Đo bảng băm.
        int found = 0;
        Timer t;
        t.start();
        for (int q = 0; q < QUERIES_HASH; q++) {
            if (table.find(keys[q]) != nullptr) found++;
        }
        double hashUs = t.elapsedMs() * 1000.0 / QUERIES_HASH;

        // 4. Đo duyệt tuần tự.
        int found2 = 0;
        t.start();
        for (int q = 0; q < QUERIES_LIST; q++) {
            if (listFind(head, keys[q])) found2++;
        }
        double listUs = t.elapsedMs() * 1000.0 / QUERIES_LIST;

        if (found != QUERIES_HASH || found2 != QUERIES_LIST) {
            cout << "LOI: co so khong tim thay (found=" << found << ", " << found2 << ")\n";
        }

        cout << left << setw(9) << n
             << setw(9) << table.capacity()
             << setw(8) << fixed << setprecision(3) << table.loadFactor()
             << setw(10) << setprecision(3) << table.avgProbes()
             << setw(16) << setprecision(4) << hashUs
             << setw(18) << setprecision(3) << listUs
             << setprecision(0) << listUs / hashUs << "x\n";

        delete[] keys;
        freeList(head);
    }
    cout << "\n";
}

// ------------------------------------------------------------
// Thí nghiệm 2: số lần dò trung bình theo hệ số tải
// Dùng bảng KÍCH THƯỚC CỐ ĐỊNH (số nguyên tố) và chèn tới đúng hệ số tải mong muốn.
// HashTable::avgProbes() chỉ đếm các lần find(), nên với mỗi mức tải:
//   - bảng 1: chỉ tra số CÓ  -> avgProbes = số lần dò khi tìm THẤY
//   - bảng 2: chỉ tra số KHÔNG CÓ -> avgProbes = số lần dò khi tìm THẤT BẠI
// Lý thuyết (băm kép lý tưởng, a = hệ số tải):
//   tìm thấy    ~ (1/a) * ln(1/(1-a))
//   tìm thất bại ~ 1/(1-a)
// ------------------------------------------------------------
static void buildTable(HashTable& table, Contact* dummy, int count) {
    for (int i = 0; i < count; i++) {
        table.insert(makePhone(i, "09"), dummy);
    }
}

static void experiment2(Contact* dummy) {
    cout << "=== THI NGHIEM 2: so lan do trung binh theo he so tai ===\n";
    cout << "Bang co dinh 100003 o; chen toi muc tai mong muon roi do bang find().\n\n";
    cout << left << setw(10) << "load"
         << setw(14) << "thay (do)"
         << setw(14) << "thay (LT)"
         << setw(16) << "that bai (do)"
         << "that bai (LT)\n";

    const int SIZE = 100003;           // số nguyên tố
    const int QUERIES = 100000;
    double loads[5] = {0.1, 0.3, 0.5, 0.6, 0.7};

    for (int li = 0; li < 5; li++) {
        int count = (int)(loads[li] * SIZE);

        // Bảng 1: tra số CÓ trong bảng.
        HashTable t1(SIZE);
        buildTable(t1, dummy, count);
        srand(77);
        for (int q = 0; q < QUERIES; q++) {
            t1.find(makePhone(rand() % count, "09"));
        }
        double okMeasured = t1.avgProbes();

        // Bảng 2: giống hệt bảng 1, nhưng tra số KHÔNG có (đầu số "03").
        HashTable t2(SIZE);
        buildTable(t2, dummy, count);
        srand(88);
        for (int q = 0; q < QUERIES; q++) {
            t2.find(makePhone(rand() % 100000000, "03"));
        }
        double failMeasured = t2.avgProbes();

        double a = t1.loadFactor();    // hệ số tải THỰC TẾ (kiểm tra bảng không bị rehash ngoài ý muốn)
        double okTheory = (1.0 / a) * log(1.0 / (1.0 - a));
        double failTheory = 1.0 / (1.0 - a);

        cout << left << setw(10) << fixed << setprecision(3) << a
             << setw(14) << setprecision(3) << okMeasured
             << setw(14) << okTheory
             << setw(16) << failMeasured
             << failTheory;
        if (t1.capacity() != SIZE) cout << "   (bang da bi rehash!)";
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    Contact* dummy = createContact("dummy", "", GROUP_FRIEND);   // chỉ để có con trỏ khác nullptr

    experiment1(dummy);
    experiment2(dummy);

    freeContact(dummy);
    return 0;
}