// ============================================================
// File      : src/core/NameTree.cpp
// Phụ trách : THÀNH VIÊN B
// ============================================================
#include <iostream>
#include "core/NameTree.h"
#include "core/StringUtils.h"
#include "io/FileManager.h"

using namespace std;

static void printContact(const Contact* c) {
    if (c == nullptr) return;
    cout << "- " << c->name << " | SDT: ";
    if (c->phones == nullptr) {
        cout << "(khong co)";
    } else {
        for (PhoneNode* p = c->phones; p != nullptr; p = p->next) {
            cout << p->number;
            if (p->next != nullptr) cout << ", ";
        }
    }
    if (!c->email.empty()) {
        cout << " | Email: " << c->email;
    }
    cout << " | Nhom: ";
    if (c->group == GROUP_FAMILY) cout << "Gia dinh";
    else if (c->group == GROUP_FRIEND) cout << "Ban be";
    else if (c->group == GROUP_WORK) cout << "Cong viec";
    else cout << "Khac";
    cout << "\n";
}

NameTree::NameTree() : root(nullptr), count(0) {}

NameTree::~NameTree() {
    destroyAll();
}

bool NameTree::insert(Contact* c) {
    if (c == nullptr) return false;
    bool inserted = false;
    root = insertRec(root, c, inserted);
    if (inserted) {
        count++;
    }
    return inserted;
}

TreeNode* NameTree::insertRec(TreeNode* node, Contact* c, bool& inserted) {
    // 1. Đã tìm thấy vị trí trống -> tạo nút mới
    if (node == nullptr){
        inserted = true;
        return new TreeNode(c);
    }
    // 2. So sánh tên để rẽ nhánh
    int cmp = compareNames(c->name, node->contact->name);
    if (cmp < 0) {
        node->left = insertRec(node->left, c, inserted);
    }else if (cmp > 0) {
        node->right = insertRec(node->right, c, inserted);
    }else {
        // cmp == 0: trung tên ko trèn
        inserted = false;
    }
    return node;
}

Contact* NameTree::find(const string& name) const {
    TreeNode* curr = root;
    while (curr != nullptr) {
        int cmp = compareNames(name, curr->contact->name);
        if (cmp == 0) {
            return curr->contact;
        }else if (cmp < 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return nullptr;
}

bool NameTree::remove(const string& name) {
    bool removed = false;
    root = removeRec(root, name, removed);
    if (removed) {
        count--;
    }
    return removed;
}

TreeNode* NameTree::removeRec(TreeNode* node, const string& name, bool& removed) {

    if (node == nullptr) {
        removed = false;
        return nullptr;
    }

    int cmp = compareNames(name, node->contact->name);
    if (cmp < 0) {
        node->left = removeRec(node->left, name, removed);
    } else if (cmp > 0) {
        node->right = removeRec(node->right, name, removed);
    } else {
        // Đã tìm thấy nút cần xóa
        removed = true;

        // Trường hợp 1: Nút lá (không có con nào)
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }
        // Trường hợp 2a: Chỉ có con phải
        if (node->left == nullptr) {
            TreeNode* temp = node->right;
            delete node;
            return temp;
        }
        // Trường hợp 2b: Chỉ có con trái
        if (node->right == nullptr) {
            TreeNode* temp = node->left;
            delete node;
            return temp;
        }

        // Trường hợp 3: Có đủ 2 con
        // Tìm nút trái nhất của cây con bên phải (phần tử nhỏ nhất lớn hơn node)
        TreeNode* successor = node->right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }

        // Chép dữ liệu từ nút thế mạng lên nút hiện tại
        node->contact = successor->contact;

        // Xóa nút thế mạng ở cây con bên phải (lúc này nó chắc chắn rơi vào TH 1 hoặc 2a)
        bool dummyRemoved = false;
        node->right = removeRec(node->right, successor->contact->name, dummyRemoved);
    }

    return node;
}

void NameTree::printInOrder() const {
    if (root == nullptr) {
        cout<<"danh ba trong. \n";
        return;
    }
    inOrderRec(root);
}

void NameTree::inOrderRec(const TreeNode* node) const {
    if (node == nullptr) return;
    inOrderRec(node->left);
    printContact(node->contact);
    inOrderRec(node->right);
}

void NameTree::printByGroup(int group) const {
    groupRec(root, group);
}

void NameTree::groupRec(const TreeNode* node, int group) const {
    if (node == nullptr) return;
    groupRec(node->left, group);
    if (node->contact->group == group) {
        printContact(node->contact);
    }
    groupRec(node->right, group);
}

void NameTree::searchPrefix(const string& prefix) const {
    prefixRec(root, prefix);
   
}

void NameTree::prefixRec(const TreeNode* node, const string& prefix) const {
    if (node == nullptr) return;

    // th1 : tên tại nút bắt đầu bằng prefix
    if(startsWith(node->contact->name, prefix)) {
        prefixRec(node->left, prefix);
        printContact(node->contact);
        prefixRec(node->right, prefix);
    }
    // Trường hợp 2: Không khớp tiền tố -> dùng compareNames để cắt nhánh
    else if (compareNames(prefix, node->contact->name) < 0) {
        // prefix đứng trước tên nút -> chỉ tìm bên trái
        prefixRec(node->left, prefix);
    } else {
        // prefix đứng sau tên nút -> chỉ tìm bên phải
        prefixRec(node->right, prefix);
    }
}

void NameTree::writeAll(ostream& out) const {
    writeRec(root, out);
    
}

void NameTree::writeRec(const TreeNode* node, ostream& out) const {
    if (node == nullptr) return;
    writeRec(node->left, out);
    writeContactLine(out, node->contact);
    writeRec(node->right, out);
   
}

int NameTree::height() const {
    return heightRec(root);
    
}

int NameTree::heightRec(const TreeNode* node) const {
    if (node == nullptr) return 0;
    
    int leftH = heightRec(node->left);
    int rightH = heightRec(node->right);
    
    return 1 + (leftH > rightH ? leftH : rightH);

}

void NameTree::destroyAll() {
    destroyRec(root);
    root = nullptr;
    count = 0;
  
}

void NameTree::destroyRec(TreeNode* node) {
    if (node == nullptr) return;

    destroyRec(node->left);
    destroyRec(node->right);
    freeContact ( node->contact);
    delete node;
    
}
