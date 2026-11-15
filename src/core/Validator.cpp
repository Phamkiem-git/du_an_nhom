#include "core/Validator.h"
#include "core/Contact.h"   // lấy các hằng GROUP_FAMILY, GROUP_WORK

bool isValidPhone(const string& phone) {
    int len = (int)phone.length();
    if (len < 9 || len > 11) return false;      // sai độ dài

    for (int i = 0; i < len; i++) {
        if (phone[i] < '0' || phone[i] > '9') return false;   // gặp ký tự không phải chữ số
    }
    return true;
}

bool isValidGroup(int group) {
    return group >= GROUP_FAMILY && group <= GROUP_WORK;
}