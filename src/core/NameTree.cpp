#include <iostream>
#include "core/NameTree.h"
#include "core/StringUtils.h"
#include "io/FileManager.h"

using namespace std;

// printContact (hàm trợ giúp, chỉ dùng trong file này): in một liên hệ trên MỘT dòng:
// tên, các số điện thoại (duyệt danh sách liên kết), email (nếu có) và nhóm. Bỏ qua nếu c rỗng.
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

// Hàm tạo: cây rỗng (root = nullptr, count = 0).
NameTree::NameTree() : root(nullptr), count(0) {}

// Hàm hủy: gọi destroyAll để giải phóng toàn bộ nút VÀ liên hệ khi đối tượng bị hủy.
NameTree::~NameTree() {
    destroyAll();
}


// insert: thêm liên hệ c vào cây theo tên. Trả về false nếu c rỗng hoặc tên đã có
// (so sánh không phân biệt hoa/thường); khi đó cây KHÔNG giữ c nên người gọi phải tự freeContact(c).
// Độ phức tạp: trung bình O(log n), xấu nhất O(n).
bool NameTree::insert(Contact* c) {
    if (c == nullptr) return false;
    bool inserted = false;
    root = insertRec(root, c, inserted);
    if (inserted) {
        count++;
    }
    return inserted;
}


// insertRec: hàm đệ quy của insert. Đi xuống theo compareNames (nhỏ hơn -> trái, lớn hơn -> phải)
// đến chỗ trống thì tạo nút mới (inserted = true); gặp tên trùng thì giữ nguyên cây (inserted = false).
// Trả về gốc của cây con sau khi chèn để nối lại con trỏ của nút cha.
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


// find: tìm liên hệ theo tên đầy đủ (không phân biệt hoa/thường) bằng vòng lặp từ gốc đi xuống.
// Trả về nullptr nếu không có. Trung bình O(log n), xấu nhất O(n).
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


// remove: gỡ nút có tên name khỏi cây. Chỉ delete TreeNode, KHÔNG delete Contact
// (AddressBook lo việc free). Trả về true nếu tìm thấy và gỡ được.
bool NameTree::remove(const string& name) {
    bool removed = false;
    root = removeRec(root, name, removed);
    if (removed) {
        count--;
    }
    return removed;
}


// removeRec: đệ quy tìm nút cần xóa rồi xử lý 3 trường hợp:
//   (1) nút lá -> xóa nút; (2) chỉ có một con -> nối con đó lên thay chỗ;
//   (3) có hai con -> tìm nút nhỏ nhất của cây con phải (nút kế thừa), chép CON TRỎ contact của nó
//       lên nút hiện tại, rồi gỡ nút kế thừa khỏi cây con phải (cũng bằng removeRec).
// Trả về gốc mới của cây con.
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


// printInOrder: in toàn bộ danh bạ theo thứ tự chữ cái A-Z (duyệt LNR); cây rỗng thì báo danh bạ trống.
void NameTree::printInOrder() const {
    if (root == nullptr) {
        cout<<"danh ba trong. \n";
        return;
    }
    inOrderRec(root);
}


// inOrderRec: duyệt trái -> nút -> phải. Vì là BST nên kết quả tự động đã sắp xếp theo tên.
void NameTree::inOrderRec(const TreeNode* node) const {
    if (node == nullptr) return;
    inOrderRec(node->left);
    printContact(node->contact);
    inOrderRec(node->right);
}


// printByGroup: in các liên hệ thuộc nhóm group, theo thứ tự chữ cái.
void NameTree::printByGroup(int group) const {
    groupRec(root, group);
}


// groupRec: duyệt LNR cả cây, chỉ in nút có contact->group == group.
// Nhóm không liên quan đến thứ tự tên nên phải duyệt hết: O(n).
void NameTree::groupRec(const TreeNode* node, int group) const {
    if (node == nullptr) return;
    groupRec(node->left, group);
    if (node->contact->group == group) {
        printContact(node->contact);
    }
    groupRec(node->right, group);
}


// searchPrefix: in các liên hệ có tên bắt đầu bằng prefix (không phân biệt hoa/thường).
// Không có liên hệ nào khớp thì không in gì.
void NameTree::searchPrefix(const string& prefix) const {
    prefixRec(root, prefix);
   
}


// prefixRec: duyệt có CẮT NHÁNH. Nút khớp tiền tố -> in nó và đi cả hai phía (theo thứ tự LNR).
// Nút không khớp: nếu prefix đứng trước tên nút thì mọi tên bắt đầu bằng prefix chỉ có thể
// nằm bên trái, ngược lại chỉ có thể nằm bên phải -> bỏ cả nhánh còn lại, không duyệt vô ích.
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


// writeAll: ghi toàn bộ liên hệ ra luồng out theo thứ tự chữ cái (mỗi liên hệ một dòng), dùng khi lưu tệp.
void NameTree::writeAll(ostream& out) const {
    writeRec(root, out);
    
}


// writeRec: duyệt LNR, mỗi nút gọi writeContactLine để ghi một dòng đúng định dạng tệp.
void NameTree::writeRec(const TreeNode* node, ostream& out) const {
    if (node == nullptr) return;
    writeRec(node->left, out);
    writeContactLine(out, node->contact);
    writeRec(node->right, out);
   
}

// writeAllCsv: giống writeAll nhưng mỗi liên hệ được ghi thành một dòng CSV (nhập/xuất CSV - phần nâng cao).
void NameTree::writeAllCsv(ostream& out) const {
    writeCsvRec(root, out);
}


// writeCsvRec: duyệt LNR, mỗi nút gọi writeCsvLine để ghi một dòng CSV.
void NameTree::writeCsvRec(const TreeNode* node, ostream& out) const {
    if (node == nullptr) return;
    writeCsvRec(node->left, out);
    writeCsvLine(out, node->contact);
    writeCsvRec(node->right, out);
}


// height: chiều cao cây (cây rỗng = 0, cây một nút = 1). Dùng trong thực nghiệm để xem cây có bị lệch không.
int NameTree::height() const {
    return heightRec(root);
    
}


// heightRec: chiều cao = 1 + chiều cao lớn hơn trong hai cây con (cây rỗng = 0).
int NameTree::heightRec(const TreeNode* node) const {
    if (node == nullptr) return 0;
    
    int leftH = heightRec(node->left);
    int rightH = heightRec(node->right);
    
    return 1 + (leftH > rightH ? leftH : rightH);

}


// destroyAll: giải phóng toàn bộ nút và Contact rồi đặt lại cây rỗng; gọi nhiều lần vẫn an toàn.
void NameTree::destroyAll() {
    destroyRec(root);
    root = nullptr;
    count = 0;
  
}


// destroyRec: duyệt hậu thứ tự (trái, phải, rồi mới tới nút): phải giải phóng con trước nút cha.
// Mỗi nút: freeContact rồi delete nút.
void NameTree::destroyRec(TreeNode* node) {
    if (node == nullptr) return;

    destroyRec(node->left);
    destroyRec(node->right);
    freeContact ( node->contact);
    delete node;
    
}