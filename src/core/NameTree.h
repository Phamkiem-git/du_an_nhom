
#ifndef NAMETREE_H
#define NAMETREE_H

#include <iosfwd>
#include <string>
#include "core/Contact.h"

struct TreeNode {
    Contact*  contact;    // không sở hữu theo nghĩa thông thường: xem quy ước ở trên
    TreeNode* left;
    TreeNode* right;
    TreeNode(Contact* c) : contact(c), left(nullptr), right(nullptr) {}
};

class NameTree {
public:
    NameTree();
    ~NameTree();

    // Thêm c theo c->name. Trả về false nếu tên đã có (so sánh bằng compareNames).
    // hoa/thường và dấu tiếng Việt, nên "Nguyễn An" và "nguyen an" là cùng một tên).
    bool insert(Contact* c);

    // Tìm theo tên đầy đủ. Trả về nullptr nếu không có.
    Contact* find(const string& name) const;

    // Gỡ nút có tên name khỏi cây (delete TreeNode) nhưng KHÔNG delete Contact.
    // Phải xử lý đủ 3 trường hợp: nút lá, nút có một con, nút có hai con.
    bool remove(const string& name);

    // In theo thứ tự chữ cái (duyệt LNR).
    void printInOrder() const;

    // In các liên hệ thuộc một nhóm, theo thứ tự chữ cái.
    void printByGroup(int group) const;

    // In các liên hệ có tên bắt đầu bằng prefix. Phải CẮT NHÁNH, không duyệt cả cây một cách vô ích.
    void searchPrefix(const string& prefix) const;

    // Ghi mọi liên hệ ra out theo thứ tự chữ cái, mỗi liên hệ một dòng (gọi writeContactLine trong FileManager).
    void writeAll(std::ostream& out) const;

    // [NÂNG CAO] Ghi mọi liên hệ ra out dạng CSV theo thứ tự chữ cái (gọi writeCsvLine trong FileManager).
    void writeAllCsv(std::ostream& out) const;

    int size() const { return count; }
    int height() const;               // chiều cao cây (cây rỗng = 0)

    // Giải phóng toàn bộ nút VÀ toàn bộ Contact (gọi freeContact). An toàn khi gọi nhiều lần.
    void destroyAll();

private:
    TreeNode* root;
    int count;

    // Các hàm đệ quy trợ giúp (B có thể đổi tên/tham số tùy ý)
    TreeNode* insertRec(TreeNode* node, Contact* c, bool& inserted);
    TreeNode* removeRec(TreeNode* node, const string& name, bool& removed);
    void inOrderRec(const TreeNode* node) const;
    void groupRec(const TreeNode* node, int group) const;
    void prefixRec(const TreeNode* node, const string& prefix) const;
    void writeRec(const TreeNode* node, std::ostream& out) const;
    void writeCsvRec(const TreeNode* node, std::ostream& out) const;
    int  heightRec(const TreeNode* node) const;
    void destroyRec(TreeNode* node);

    NameTree(const NameTree&) = delete;
    NameTree& operator=(const NameTree&) = delete;
};

#endif
