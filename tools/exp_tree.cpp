#include <iostream>
#include <cstdlib>      // rand, srand
#include "core/NameTree.h"
#include "core/Timer.h"

using namespace std;

const int TRIALS  = 5;      // số lần lặp cho thứ tự ngẫu nhiên (lấy trung bình)
const int QUERIES = 1000;   // số lần tìm dùng để đo thời gian tìm

// ------------------------------------------------------------
// Sinh tên
// ------------------------------------------------------------
// Tên thứ i gồm đúng 4 chữ cái (hệ cơ số 26), ví dụ 0 -> "Aaaa", 1 -> "Aaab".
// Vì độ dài cố định nên thứ tự theo i cũng chính là thứ tự chữ cái;
// 26^4 = 456976 tên khác nhau, đủ cho mọi n trong thực nghiệm.
static string makeName(int i) {
    string s(4, 'a');
    for (int pos = 3; pos >= 0; pos--) {
        s[pos] = (char)('a' + i % 26);
        i /= 26;
    }
    s[0] = (char)(s[0] - 'a' + 'A');   // viết hoa chữ đầu cho giống tên thật
    return s;
}

// Xáo trộn mảng a[0..n-1] bằng thuật toán Fisher-Yates.
// Lưu ý: rand() trên một số hệ thống chỉ tới 32767, vẫn đủ cho n <= 10000.
static void shuffle(int* a, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}

// Chiều cao nhỏ nhất có thể của cây nhị phân có n nút = floor(log2 n) + 1.
static int minHeight(int n) {
    int h = 0;
    while (n > 0) {
        h++;
        n /= 2;
    }
    return h;
}

// ------------------------------------------------------------
// Một lần thử trên MỘT thứ tự chèn
// ------------------------------------------------------------
struct Result {
    int    height;      // chiều cao cây sau khi chèn xong
    double insertMs;    // tổng thời gian chèn n tên (gồm cả thời gian createContact)
    double findUs;      // thời gian trung bình của một lần find, tính bằng micro-giây
};

// Chèn các tên order[0..n-1] vào cây mới, đo chiều cao, đo thời gian tìm, rồi giải phóng cây.
static Result runOnce(const int* order, int n) {
    NameTree t;
    Result r;
    Timer timer;

    // 1. Chèn
    timer.start();
    for (int k = 0; k < n; k++) {
        t.insert(createContact(makeName(order[k]), "", GROUP_FRIEND));
    }
    r.insertMs = timer.elapsedMs();

    // 2. Chiều cao
    r.height = t.height();

    // 3. Thời gian tìm: QUERIES tên rải đều trong khoảng [0, n), tên nào cũng CÓ trong cây
    int q = (n < QUERIES) ? n : QUERIES;
    int notFound = 0;
    timer.start();
    for (int k = 0; k < q; k++) {
        int idx = (int)((long long)k * n / q);
        if (t.find(makeName(idx)) == nullptr) notFound++;
    }
    double totalMs = timer.elapsedMs();
    r.findUs = totalMs * 1000.0 / q;

    if (notFound != 0 || t.size() != n) {
        cout << "[LOI] cay khong du ten: size = " << t.size()
             << ", khong tim thay " << notFound << " ten\n";
    }

    t.destroyAll();     // giải phóng toàn bộ nút và Contact
    return r;
}

// ------------------------------------------------------------
// In bảng
// ------------------------------------------------------------
static void cell(int value, int width) {
    cout.width(width);
    cout << value;
}

static void cell(double value, int width) {
    cout.width(width);
    cout << value;
}

int main() {
    const int sizes[] = {1000, 5000, 10000};
    const int numSizes = 3;

    // Cố định hạt giống để kết quả lặp lại được giữa các lần chạy.
    srand(2024);

    // Lưu kết quả của từng kích thước để in thành hai bảng sau cùng.
    int    n_[numSizes];
    int    minH[numSizes];
    double randHeight[numSizes], randInsert[numSizes], randFind[numSizes];
    Result sorted[numSizes];

    cout << "THUC NGHIEM BST: thu tu nhap NGAU NHIEN (trung binh " << TRIALS
         << " lan) so voi DA SAP XEP\n";
    cout << "Dang chay... (thu tu da sap xep co the mat vai giay vi moi lan chen la O(n))\n";

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        n_[s]  = n;
        minH[s] = minHeight(n);

        int* order = new int[n];

        // (a) Thứ tự ĐÃ SẮP XẾP: chèn 0, 1, 2, ..., n-1
        for (int i = 0; i < n; i++) order[i] = i;
        sorted[s] = runOnce(order, n);

        // (b) Thứ tự NGẪU NHIÊN: lặp TRIALS lần, mỗi lần một hoán vị mới, lấy trung bình
        double sumH = 0, sumIns = 0, sumFind = 0;
        for (int trial = 0; trial < TRIALS; trial++) {
            for (int i = 0; i < n; i++) order[i] = i;
            shuffle(order, n);
            Result r = runOnce(order, n);
            sumH    += r.height;
            sumIns  += r.insertMs;
            sumFind += r.findUs;
        }
        randHeight[s] = sumH / TRIALS;
        randInsert[s] = sumIns / TRIALS;
        randFind[s]   = sumFind / TRIALS;

        delete[] order;
        cout << "  xong n = " << n << "\n";
    }

    cout.setf(ios::fixed);
    cout.precision(2);

    // ---- Bảng 1: chiều cao ----
    cout << "\nBANG 1. CHIEU CAO CAY\n";
    cout << "      n | toi thieu | ngau nhien (TB) | da sap xep\n";
    cout << "--------+-----------+-----------------+-----------\n";
    for (int s = 0; s < numSizes; s++) {
        cell(n_[s], 7);
        cout << " |";
        cell(minH[s], 10);
        cout << " |";
        cell(randHeight[s], 16);
        cout << " |";
        cell(sorted[s].height, 11);
        cout << "\n";
    }
    cout << "(toi thieu = floor(log2 n) + 1 = chieu cao cua cay can bang hoan hao)\n";

    // ---- Bảng 2: thời gian ----
    cout << "\nBANG 2. THOI GIAN\n";
    cout << "      n | chen ngau nhien (ms) | chen da sap xep (ms) | tim ngau nhien (us) | tim da sap xep (us)\n";
    cout << "--------+----------------------+----------------------+---------------------+--------------------\n";
    for (int s = 0; s < numSizes; s++) {
        cell(n_[s], 7);
        cout << " |";
        cell(randInsert[s], 21);
        cout << " |";
        cell(sorted[s].insertMs, 21);
        cout << " |";
        cell(randFind[s], 20);
        cout << " |";
        cell(sorted[s].findUs, 20);
        cout << "\n";
    }
    cout << "(thoi gian chen = tong n lan chen; thoi gian tim = trung binh mot lan find)\n";

    cout << "\nNHAN XET GOI Y CHO BAO CAO:\n";
    cout << " - Nhap ngau nhien: chieu cao chi khoang gap doi toi thieu, O(log n) -> chen/tim nhanh.\n";
    cout << " - Nhap da sap xep: chieu cao = n (cay thanh \"cai que\"), moi thao tac O(n);\n";
    cout << "   tong thoi gian chen tang theo n^2.\n";
    cout << " - Huong phat trien: AVL tu quay de giu chieu cao O(log n) o moi thu tu nhap.\n";
    return 0;
}