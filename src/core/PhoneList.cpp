// ============================================================
// File      : src/core/PhoneList.cpp
// Phụ trách : THÀNH VIÊN A
// ============================================================
#include "core/PhoneList.h"

void addPhone(Contact* c, const string& number) {
    // TODO (A): tạo PhoneNode mới bằng new, gắn vào danh sách c->phones.
    //   Gợi ý: thêm vào đầu danh sách là đơn giản nhất.
    //   Nhớ xử lý trường hợp danh sách đang rỗng (c->phones == nullptr).
}

bool removePhone(Contact* c, const string& number) {
    // TODO (A): duyệt danh sách, tìm nút có number, nối nút trước với nút sau, delete nút đó.
    //   Hai trường hợp cần tách riêng: xóa nút ĐẦU và xóa nút Ở GIỮA/CUỐI.
    return false;
}

bool hasPhone(const Contact* c, const string& number) {
    // TODO (A): duyệt danh sách, so sánh từng number.
    return false;
}

int countPhones(const Contact* c) {
    // TODO (A): duyệt danh sách và đếm.
    return 0;
}

void freePhones(Contact* c) {
    // TODO (A): duyệt danh sách, delete từng nút (nhớ lưu con trỏ next TRƯỚC khi delete).
    //   Cuối cùng đặt c->phones = nullptr.
}
