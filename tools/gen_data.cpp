// ============================================================
// File      : tools/gen_data.cpp
// Phụ trách : THÀNH VIÊN A
// Vai trò   : Chương trình RIÊNG sinh tệp dữ liệu lớn (ngẫu nhiên) để đo thời gian.
// Chạy      : make gen_data        (mặc định: 10000 liên hệ -> data/contacts_large.txt)
//             ./build/gen_data <số liên hệ> <tệp đầu ra>
// Đề yêu cầu: bộ dữ liệu lớn từ 10.000 bản ghi trở lên.
//
// Ghi chú cho báo cáo: rand() và srand() ở đây chỉ dùng để SINH DỮ LIỆU,
// không phải cấu trúc dữ liệu nên không vi phạm quy định của đề.
// ============================================================
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

// Các từ dùng để ghép tên (viết không dấu).
static const char* HO[]  = {"Nguyen", "Tran", "Le", "Pham", "Hoang",
                            "Huynh", "Vu", "Dang", "Bui", "Do"};
static const char* DEM[] = {"Van", "Thi", "Minh", "Quoc", "Ngoc",
                            "Thanh", "Duc", "Hoai", "Gia", "Anh"};
static const char* TEN[] = {"An", "Binh", "Cuong", "Dung", "Giang",
                            "Hanh", "Hieu", "Khoa", "Lan", "Linh",
                            "Minh", "Nam", "Phuc", "Quyen", "Son",
                            "Tam", "Thao", "Trang", "Tuan", "Vy"};
static const int SO_HO  = 10;
static const int SO_DEM = 10;
static const int SO_TEN = 20;

// Chuyển số v thành chuỗi đúng "width" chữ số, đệm số 0 bên trái.
// Ví dụ: padZero(42, 8) -> "00000042"
static string padZero(long long v, int width) {
    string s = to_string(v);
    while ((int)s.length() < width) s = "0" + s;
    return s;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Cách dùng: gen_data <số liên hệ> <tệp đầu ra>\n";
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        cout << "Số liên hệ phải là số nguyên dương.\n";
        return 1;
    }
    if (n >= 100000000) {   // số điện thoại dùng 8 chữ số nên tối đa 99.999.999 liên hệ
        cout << "Số liên hệ quá lớn (tối đa 99999999).\n";
        return 1;
    }

    ofstream out(argv[2]);
    if (!out) {
        cout << "Không mở được tệp để ghi: " << argv[2] << "\n";
        return 1;
    }

    srand(12345);   // seed cố định: chạy lại luôn ra cùng một tệp, dễ lặp lại thực nghiệm

    out << "# Du lieu sinh ngau nhien boi tools/gen_data.cpp\n";

    for (int i = 0; i < n; i++) {
        // --- Tên: ghép ngẫu nhiên + số thứ tự (i + 1) để KHÔNG BAO GIỜ trùng tên.
        string name = string(HO[rand() % SO_HO]) + " " +
                      DEM[rand() % SO_DEM] + " " +
                      TEN[rand() % SO_TEN] + " " +
                      to_string(i + 1);

        // --- Số điện thoại: dựa vào i nên chắc chắn không trùng.
        //     Số chính bắt đầu "09", số phụ (khoảng 1/4 người có) bắt đầu "03".
        //     Hai đầu số khác nhau nên số phụ không bao giờ trùng số chính của người khác.
        string phones = "09" + padZero(i, 8);
        if (rand() % 4 == 0) {
            phones += ",03" + padZero(i, 8);
        }

        // --- Email: khoảng 1/3 người để trống.
        string email = "";
        if (rand() % 3 != 0) {
            email = "user" + to_string(i + 1) + "@mail.com";
        }

        // --- Nhóm: 0 gia đình, 1 bạn bè, 2 công việc.
        int group = rand() % 3;

        out << name << "|" << phones << "|" << email << "|" << group << "\n";
    }

    out.close();
    cout << "Đã sinh " << n << " liên hệ vào " << argv[2] << "\n";
    return 0;
}