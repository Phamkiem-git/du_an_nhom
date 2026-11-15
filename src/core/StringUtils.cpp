// ============================================================
// File      : src/core/StringUtils.cpp
// Phụ trách : THÀNH VIÊN B
// ============================================================
#include "core/StringUtils.h"

string toLower(const string& s) {
    string result =  s;
    for (size_t i = 0; i < result.length(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] + ('a' - 'A');
        }
    }
    return result;

}

// Ký tự được coi là khoảng trắng: dấu cách, tab, và \r \n (tệp Windows kết thúc dòng bằng \r\n)
static bool isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

string trim(const string& s) {
    int left = 0;
    int right = s.length() - 1;

    while (left <= right && isSpace(s[left]))
        left++;

    while (right >= left && isSpace(s[right]))
        right--;

    if (left > right) 
        return "";

    return s.substr(left, right - left + 1);

}

bool startsWith(const string& s, const string& prefix) {
    string str  = toLower(s);
    string pre = toLower(prefix);
    if (pre.length() > str.length())
        return false;
    
    for (size_t i = 0; i < pre.length(); i++) {
        if (str[i] != pre[i]) 
            return false;
    }
    return true;
}

int compareNames(const string& a, const string& b) {
    string s1 = toLower(a);
    string s2 = toLower(b);

    int n;
    
    if (s1.length() < s2.length())
        n = s1.length();
    else
        n = s2.length();

    for (int i = 0; i < n; i++) {
        unsigned char c1 = (unsigned char)s1[i];
        unsigned char c2 = (unsigned char)s2[i];

        if (c1 < c2)
            return -1;

        if (c1 > c2)
            return 1;
    }
    if (s1.length() < s2.length())
        return -1;

    if (s1.length() > s2.length())
        return 1;

    return 0;
   
}

int indexOf(const string& s, char c, int from ) {
    for ( size_t i = from; i < s.length(); i++) {
        if (s[i] == c)
            return i;
    }
    return -1;
}

bool isValidName(const string& name) {
    string s = trim(name);

    if (s.empty())
        return false;
    
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '|')
            return false;
    }

    return true;
}

bool isValidEmail(const string& email) {
    if (email.empty())
        return true;

    int countAt = 0;
    int atPos = -1;

    for (size_t i = 0; i < email.length(); i++) {
        if (email[i] == '@') {
            countAt++;
            atPos = i;
        }
    }

    if (countAt != 1)
        return false;
    if (atPos == 0)
        return false;

    if (atPos == (int)email.length() - 1)
        return false;

    return true;
}
