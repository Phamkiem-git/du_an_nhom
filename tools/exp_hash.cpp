// ============================================================
// File      : tools/exp_hash.cpp
// Phụ trách : THÀNH VIÊN A
// Vai trò   : Thực nghiệm cho báo cáo (phần bảng băm):
//             - thời gian tra cứu theo số: bảng băm so với duyệt tuần tự DSLK
//             - số lần dò trung bình theo hệ số tải
// Chạy      : make exp_hash
// ============================================================
#include <iostream>
#include "core/HashTable.h"
#include "core/Timer.h"

using namespace std;

int main() {
    // TODO (A): với n = 1000, 10000, 100000 (hoặc tương tự):
    //   1. Sinh n số điện thoại không trùng; chèn vào HashTable và vào một danh sách liên kết thường.
    //   2. Dùng Timer đo thời gian tra cứu cùng một tập số bằng hai cách.
    //   3. In bảng: n | thời gian bảng băm | thời gian duyệt tuần tự | avgProbes | loadFactor.
    //   Số liệu này dán thẳng vào bảng thực nghiệm trong báo cáo, kèm nhận xét so với lý thuyết.
    cout << "(exp_hash chưa cài đặt)\n";
    return 0;
}
