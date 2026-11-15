#include "core/PhoneList.h"

// Thêm số vào ĐẦU danh sách.
void addPhone(Contact* c, const string& number) {
    PhoneNode* node = new PhoneNode;   // 1. tạo nút mới trên heap
    node->number = number;             // 2. ghi số vào nút
    node->next = c->phones;            // 3. nút mới trỏ vào đầu cũ (rỗng thì là nullptr)
    c->phones = node;                  // 4. đầu danh sách bây giờ là nút mới
}

// Xóa số khỏi danh sách. Trả về true nếu tìm thấy và đã xóa.
bool removePhone(Contact* c, const string& number) {
    PhoneNode* prev = nullptr;         // nút đứng trước cur
    PhoneNode* cur = c->phones;        // nút đang xét

    while (cur != nullptr) {
        if (cur->number == number) {
            if (prev == nullptr) {
                c->phones = cur->next;     // xóa nút ĐẦU
            } else {
                prev->next = cur->next;    // xóa nút giữa/cuối
            }
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

bool hasPhone(const Contact* c, const string& number) {
    for (const PhoneNode* p = c->phones; p != nullptr; p = p->next) {
        if (p->number == number) return true;
    }
    return false;
}

int countPhones(const Contact* c) {
    int count = 0;
    for (const PhoneNode* p = c->phones; p != nullptr; p = p->next) {
        count++;
    }
    return count;
}

void freePhones(Contact* c) {
    PhoneNode* cur = c->phones;
    while (cur != nullptr) {
        PhoneNode* next = cur->next;   // LƯU nút kế tiếp TRƯỚC khi delete
        delete cur;
        cur = next;
    }
    c->phones = nullptr;
}