
#ifndef MENU_H
#define MENU_H

#include <string>
#include "core/AddressBook.h"

// Vòng lặp menu chính. Thoát khi người dùng chọn 0.
void runMenu(AddressBook& book);

// Đọc một số nguyên trong [lo, hi]; nhập sai (chữ, ngoài miền) thì báo lỗi và hỏi lại.
// Gợi ý: cin.fail(), cin.clear(), cin.ignore(...) để xóa dữ liệu thừa trong bộ đệm.
int readInt(const string& prompt, int lo, int hi);

// Đọc một dòng (cả khoảng trắng) sau khi in prompt.
string readLine(const string& prompt);

#endif
