// ============================================================
// File      : src/core/NameTree.cpp
// Phụ trách : THÀNH VIÊN B
// ============================================================
#include <iostream>
#include "core/NameTree.h"
#include "core/StringUtils.h"
#include "io/FileManager.h"

using namespace std;

NameTree::NameTree() : root(nullptr), count(0) {}

NameTree::~NameTree() {
    destroyAll();
}

bool NameTree::insert(Contact* c) {
    // TODO (B): root = insertRec(root, c, inserted); nếu inserted thì count++.
    return false;
}

TreeNode* NameTree::insertRec(TreeNode* node, Contact* c, bool& inserted) {
    // TODO (B): node == nullptr -> tạo nút mới, inserted = true.
    //   So sánh compareNames(c->name, node->contact->name): <0 đi trái, >0 đi phải, ==0 trùng (inserted = false).
    inserted = false;
    return node;
}

Contact* NameTree::find(const string& name) const {
    // TODO (B): đi từ root: bằng thì trả về, nhỏ hơn đi trái, lớn hơn đi phải. (Dùng vòng lặp hoặc đệ quy.)
    return nullptr;
}

bool NameTree::remove(const string& name) {
    // TODO (B): root = removeRec(root, name, removed); nếu removed thì count--.
    return false;
}

TreeNode* NameTree::removeRec(TreeNode* node, const string& name, bool& removed) {
    // TODO (B): tìm nút cần xóa rồi xử lý 3 trường hợp:
    //   (1) nút lá            -> delete nút, trả về nullptr
    //   (2) một con           -> delete nút, trả về con còn lại
    //   (3) hai con           -> tìm nút nhỏ nhất bên cây phải (kế thừa), chép con trỏ contact của nó
    //                            lên nút hiện tại, rồi xóa nút kế thừa khỏi cây phải.
    //   Nhớ: chỉ delete TreeNode, KHÔNG delete Contact.
    removed = false;
    return node;
}

void NameTree::printInOrder() const {
    // TODO (B): inOrderRec(root); nếu cây rỗng thì in thông báo "Danh bạ trống".
}

void NameTree::inOrderRec(const TreeNode* node) const {
    // TODO (B): LNR: trái -> in nút -> phải. In: tên, các số điện thoại, email, nhóm.
}

void NameTree::printByGroup(int group) const {
    // TODO (B): groupRec(root, group).
}

void NameTree::groupRec(const TreeNode* node, int group) const {
    // TODO (B): duyệt LNR, chỉ in các nút có contact->group == group.
}

void NameTree::searchPrefix(const string& prefix) const {
    // TODO (B): prefixRec(root, prefix).
}

void NameTree::prefixRec(const TreeNode* node, const string& prefix) const {
    // TODO (B): cắt nhánh. Gợi ý: nếu tên của nút "lớn hơn" mọi chuỗi bắt đầu bằng prefix thì chỉ đi trái;
    //   nếu "nhỏ hơn" thì chỉ đi phải; nếu startsWith(tên nút, prefix) thì in và đi cả hai phía (theo thứ tự LNR).
}

void NameTree::writeAll(ostream& out) const {
    // TODO (B): writeRec(root, out).
}

void NameTree::writeRec(const TreeNode* node, ostream& out) const {
    // TODO (B): LNR, mỗi nút gọi writeContactLine(out, node->contact) (khai báo trong io/FileManager.h).
}

int NameTree::height() const {
    // TODO (B): return heightRec(root).
    return 0;
}

int NameTree::heightRec(const TreeNode* node) const {
    // TODO (B): 0 nếu node == nullptr; ngược lại 1 + max(chiều cao trái, chiều cao phải) (tự viết phép so sánh max).
    return 0;
}

void NameTree::destroyAll() {
    // TODO (B): destroyRec(root); root = nullptr; count = 0.   (gọi nhiều lần vẫn an toàn)
}

void NameTree::destroyRec(TreeNode* node) {
    // TODO (B): duyệt hậu thứ tự (LRN): giải phóng con trái, con phải, rồi freeContact(node->contact), rồi delete node.
}
