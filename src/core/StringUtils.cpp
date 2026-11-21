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


// ------------------------------------------------------------
// [NÂNG CAO] Xử lý chữ tiếng Việt có dấu (UTF-8)
//
// Chuỗi trong chương trình là UTF-8: chữ có dấu chiếm 2-3 byte, nên không thể so sánh
// từng byte như với ASCII. Các hàm dưới đây giải mã từng ký tự (code point) rồi "gấp"
// chữ có dấu về chữ cái gốc: ễ -> e, Đ -> d, ... Mọi thao tác chỉ duyệt chuỗi một lần
// và không cấp phát thêm chuỗi tạm, độ phức tạp O(độ dài chuỗi).
// ------------------------------------------------------------

// Giải mã MỘT ký tự UTF-8 bắt đầu tại s[i], tăng i qua các byte của nó, trả về code point.
// Byte lỗi (sai đầu, thiếu byte tiếp theo) được trả về như một ký tự riêng 0x110000 + byte,
// để chuỗi hỏng không làm chương trình dừng hay đọc tràn.
static unsigned nextCodePoint(const string& s, size_t& i) {
    unsigned char c = (unsigned char)s[i];
    int extra;          // số byte tiếp theo
    unsigned cp;
    if (c < 0x80)                { i++; return c; }
    else if ((c & 0xE0) == 0xC0) { extra = 1; cp = c & 0x1F; }
    else if ((c & 0xF0) == 0xE0) { extra = 2; cp = c & 0x0F; }
    else if ((c & 0xF8) == 0xF0) { extra = 3; cp = c & 0x07; }
    else                         { i++; return 0x110000u + c; }   // byte đầu không hợp lệ

    if (i + extra >= s.length()) {   // cần các chỉ số i+1..i+extra đều nằm trong chuỗi
        i++;
        return 0x110000u + c;                                      // chuỗi bị cắt ngang ký tự
    }
    for (int k = 1; k <= extra; k++) {
        unsigned char t = (unsigned char)s[i + k];
        if ((t & 0xC0) != 0x80) { i++; return 0x110000u + c; }     // byte tiếp theo sai dạng
        cp = (cp << 6) | (t & 0x3F);
    }
    i += extra + 1;
    return cp;
}

// Chữ cái gốc ('a'..'z') của các ký tự Latin mở rộng U+1EA0..U+1EF9 (ạ ả ấ ầ ... ỹ).
// Mỗi chữ chiếm hai ô liên tiếp: chữ hoa ở ô chẵn, chữ thường ở ô lẻ (tính từ U+1EA0).
static const char VN_EXT_BASE[] =
    "aaaaaaaaaaaaaa" "aaaaaaaaaa"   // Ạ-Ậ (7 chữ), Ắ-Ặ (5 chữ)    -> a
    "eeeeee" "eeeeeeeeee"           // Ẹ-Ẽ (3 chữ), Ế-Ệ (5 chữ)    -> e
    "iiii"                          // Ỉ Ị                          -> i
    "oooo" "oooooooooo" "oooooooooo"// Ọ Ỏ, Ố-Ộ, Ớ-Ợ              -> o
    "uuuu" "uuuuuuuuuu"             // Ụ Ủ, Ứ-Ự                    -> u
    "yyyyyyyy";                     // Ỳ-Ỹ                          -> y
static_assert(sizeof(VN_EXT_BASE) - 1 == 0x1EF9 - 0x1EA0 + 1, "bang VN_EXT_BASE phai du 90 o");

// Nếu cp là chữ cái tiếng Việt có dấu thì trả về chữ cái gốc viết thường ('a'..'z') và đặt
// upper = true khi cp là chữ HOA; nếu không thì trả về 0.
static char vietnameseBase(unsigned cp, bool& upper) {
    if (cp >= 0x1EA0 && cp <= 0x1EF9) {
        upper = ((cp - 0x1EA0) % 2 == 0);
        return VN_EXT_BASE[cp - 0x1EA0];
    }
    // Chữ có dấu nằm trong khối Latin-1 (à á â ã è é ê ì í ò ó ô õ ù ú ý) và chữ hoa tương ứng.
    unsigned low = cp;
    upper = false;
    if (cp >= 0xC0 && cp <= 0xDD) { low = cp + 0x20; upper = true; }
    switch (low) {
        case 0xE0: case 0xE1: case 0xE2: case 0xE3: return 'a';
        case 0xE8: case 0xE9: case 0xEA:            return 'e';
        case 0xEC: case 0xED:                       return 'i';
        case 0xF2: case 0xF3: case 0xF4: case 0xF5: return 'o';
        case 0xF9: case 0xFA:                       return 'u';
        case 0xFD:                                  return 'y';
    }
    upper = false;
    // Các chữ riêng của tiếng Việt: Ă ă, Đ đ, Ĩ ĩ, Ũ ũ, Ơ ơ, Ư ư (hoa ở ô chẵn, thường ở ô lẻ).
    switch (cp) {
        case 0x0102: upper = true; return 'a';  case 0x0103: return 'a';
        case 0x0110: upper = true; return 'd';  case 0x0111: return 'd';
        case 0x0128: upper = true; return 'i';  case 0x0129: return 'i';
        case 0x0168: upper = true; return 'u';  case 0x0169: return 'u';
        case 0x01A0: upper = true; return 'o';  case 0x01A1: return 'o';
        case 0x01AF: upper = true; return 'u';  case 0x01B0: return 'u';
    }
    return 0;
}

// nextKeyUnit: lấy "đơn vị so sánh" kế tiếp của s kể từ vị trí i (i được tăng lên), hoặc -1 nếu hết chuỗi.
// Đơn vị là chữ ASCII thường: chữ hoa -> chữ thường, chữ có dấu -> chữ gốc (ễ -> 'e', đ -> 'd').
// Dấu rời (U+0300..U+036F, dạng "e" + dấu mũ khi chữ được lưu kiểu tổ hợp) bị bỏ qua, nên
// "Nguyễn" gõ kiểu dựng sẵn hay kiểu tổ hợp đều ra cùng kết quả. Ký tự lạ giữ nguyên code point.
static int nextKeyUnit(const string& s, size_t& i) {
    while (i < s.length()) {
        unsigned cp = nextCodePoint(s, i);
        if (cp >= 0x0300 && cp <= 0x036F) continue;      // dấu rời: bỏ qua
        if (cp < 0x80) {
            if (cp >= 'A' && cp <= 'Z') return (int)(cp + ('a' - 'A'));
            return (int)cp;
        }
        bool upper;
        char base = vietnameseBase(cp, upper);
        if (base != 0) return base;
        return (int)cp;                                   // ký tự khác: giữ nguyên
    }
    return -1;
}

// removeDiacritics: bỏ dấu tiếng Việt, GIỮ hoa/thường: "Nguyễn Văn Đức" -> "Nguyen Van Duc".
// Ký tự không phải chữ tiếng Việt (ASCII, ký tự lạ) được chép nguyên văn; dấu rời bị bỏ.
string removeDiacritics(const string& s) {
    string result;
    size_t i = 0;
    while (i < s.length()) {
        size_t start = i;
        unsigned cp = nextCodePoint(s, i);
        if (cp >= 0x0300 && cp <= 0x036F) continue;      // dấu rời: bỏ
        bool upper;
        char base = vietnameseBase(cp, upper);
        if (base != 0) {
            result += upper ? (char)(base - ('a' - 'A')) : base;
        } else {
            result += s.substr(start, i - start);        // chép nguyên các byte của ký tự
        }
    }
    return result;
}

// stripBom: nếu s bắt đầu bằng 3 byte BOM của UTF-8 (EF BB BF) thì cắt chúng đi.
void stripBom(string& s) {
    if (s.length() >= 3 &&
        (unsigned char)s[0] == 0xEF && (unsigned char)s[1] == 0xBB && (unsigned char)s[2] == 0xBF) {
        s = s.substr(3);
    }
}


// startsWith: true nếu s bắt đầu bằng prefix, KHÔNG phân biệt hoa/thường và KHÔNG phân biệt dấu
// (so từng đơn vị qua nextKeyUnit). prefix rỗng thì luôn true; prefix dài hơn s thì false.
// Dùng cho tìm theo tiền tố tên: "nguyen" khớp "Nguyễn Văn An". Độ phức tạp O(độ dài prefix).
bool startsWith(const string& s, const string& prefix) {
    size_t i = 0, j = 0;
    while (true) {
        int p = nextKeyUnit(prefix, j);
        if (p == -1) return true;            // hết prefix mà chưa lệch chỗ nào -> khớp
        int c = nextKeyUnit(s, i);
        if (c != p) return false;            // s hết sớm (c = -1) hoặc khác chữ
    }
}


// compareNames: so sánh hai tên theo thứ tự chữ cái, KHÔNG phân biệt hoa/thường và dấu tiếng Việt
// (so từng đơn vị qua nextKeyUnit). Trả về -1 nếu a đứng trước b, 1 nếu a đứng sau b, 0 nếu bằng nhau.
// Đây là hàm quyết định thứ tự trong cây BST: hai tên so ra 0 được coi là TRÙNG, kể cả khi chỉ khác
// dấu ("Nguyễn An" và "Nguyen An"). Nếu một chuỗi là tiền tố của chuỗi kia thì chuỗi ngắn hơn đứng
// trước ("An" < "Anh"). Chữ có dấu xếp ngay theo chữ gốc: "Đức" đứng cùng chỗ với "Duc".
int compareNames(const string& a, const string& b) {
    size_t i = 0, j = 0;
    while (true) {
        int x = nextKeyUnit(a, i);
        int y = nextKeyUnit(b, j);
        if (x == -1 && y == -1) return 0;
        if (x == -1) return -1;
        if (y == -1) return 1;
        if (x < y) return -1;
        if (x > y) return 1;
    }
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