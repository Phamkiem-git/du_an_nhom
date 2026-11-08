// ============================================================
// File      : tools/gen_data.cpp
// Phụ trách : THÀNH VIÊN A
// Vai trò   : Chương trình RIÊNG sinh tệp dữ liệu lớn (ngẫu nhiên) để đo thời gian.
// Chạy      : make gen_data        (mặc định: 10000 liên hệ -> data/contacts_large.txt)
//             ./build/gen_data <số liên hệ> <tệp đầu ra>
// Đề yêu cầu: bộ dữ liệu lớn từ 10.000 bản ghi trở lên.
// ============================================================
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Cách dùng: gen_data <số liên hệ> <tệp đầu ra>\n";
        return 1;
    }
    // TODO (A):
    //   1. Đọc số liên hệ n từ argv[1] (tự chuyển chuỗi sang số, hoặc dùng atoi trong <cstdlib>).
    //   2. Với i = 0..n-1 sinh một dòng theo định dạng trong io/FileManager.h:
    //        - Tên: ghép từ vài họ / tên đệm / tên cho sẵn + số thứ tự để KHÔNG trùng tên.
    //        - Số điện thoại: phải KHÔNG TRÙNG giữa các dòng. Gợi ý: dùng số thứ tự (có đệm số 0) để
    //          đảm bảo duy nhất, vd "09" + 8 chữ số; sau đó thỉnh thoảng thêm số thứ hai.
    //        - Email, nhóm: ngẫu nhiên bằng rand() % 3.
    //   3. Ghi ra ofstream. In thông báo khi xong.
    cout << "(gen_data chưa cài đặt)\n";
    return 0;
}
