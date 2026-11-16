#include "core/StringUtils.h"


// toLower: trả về BẢN SAO của s với các chữ cái ASCII A-Z đổi thành a-z; ký tự khác
// (kể cả chữ có dấu tiếng Việt) giữ nguyên. Dùng để so sánh tên không phân biệt hoa/thường.
// Độ phức tạp O(độ dài chuỗi).
string toLower(const string& s) {
    string result =  s;
    for (size_t i = 0; i < result.length(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] + ('a' - 'A');
        }
    }
    return result;

}


// isSpace (hàm trợ giúp của trim): true với dấu cách, tab, '\r' và '\n'.
// ('\r' hay còn sót ở cuối dòng khi đọc tệp tạo trên Windows.)
static bool isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}


// trim: cắt các ký tự khoảng trắng (theo isSpace) ở đầu và cuối chuỗi: "  An  " -> "An".
// Chuỗi rỗng hoặc toàn khoảng trắng -> "". left và right là chỉ số ký tự đầu và cuối phần được giữ lại.
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


// startsWith: true nếu s bắt đầu bằng prefix, không phân biệt hoa/thường (so sánh sau khi toLower).
// prefix dài hơn s thì false; prefix rỗng thì luôn true. Dùng cho tìm theo tiền tố tên.
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


// compareNames: so sánh hai tên theo thứ tự chữ cái, không phân biệt hoa/thường (qua toLower).
// Trả về -1 nếu a đứng trước b, 1 nếu a đứng sau b, 0 nếu bằng nhau.
// Đây là hàm quyết định thứ tự trong cây BST: hai tên so ra 0 được coi là TRÙNG.
// So từng byte (ép sang unsigned char); nếu một chuỗi là tiền tố của chuỗi kia thì chuỗi ngắn
// hơn đứng trước ("An" < "Anh").
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

// indexOf: trả về vị trí đầu tiên của ký tự c trong s, bắt đầu tìm từ vị trí from (mặc định 0);
// không thấy thì trả về -1. Dùng để tách dòng tệp theo '|' và ',' trong FileManager.
int indexOf(const string& s, char c, int from ) {
    for ( size_t i = from; i < s.length(); i++) {
        if (s[i] == c)
            return i;
    }
    return -1;
}


// isValidName: hợp lệ khi sau trim không rỗng và không chứa '|'
// ('|' là ký tự phân cách các trường trong tệp dữ liệu).
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


// isValidEmail: email rỗng là hợp lệ (được bỏ trống). Nếu có thì chỉ kiểm tra đúng một '@'
// và có ít nhất một ký tự ở mỗi phía của '@'; không kiểm tra phần tên miền.
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