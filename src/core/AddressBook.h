// ============================================================
// File      : src/core/AddressBook.h
// Phụ trách : THÀNH VIÊN A là chủ file  |  Review: thành viên B
//
// >>> ĐÂY LÀ FILE NỐI HAI PHẦN <<<
//   - Dùng HashTable (A) để tra theo SỐ ĐIỆN THOẠI.
//   - Dùng NameTree  (B) để tra theo TÊN, in A-Z, tìm tiền tố.
//   - Menu (B) và FileManager (B) chỉ nói chuyện với AddressBook, không đụng trực tiếp
//     vào HashTable hay NameTree để hai cấu trúc luôn nhất quán.
//
// QUY TẮC NHẤT QUÁN: một Contact phải có mặt ĐỒNG THỜI
//   - trong cây tên (một lần, theo tên), và
//   - trong bảng băm (mỗi số điện thoại một khóa, cùng trỏ về Contact đó).
// ============================================================
#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <string>
#include "core/Contact.h"
#include "core/HashTable.h"
#include "core/NameTree.h"

class AddressBook {
public:
    AddressBook();
    ~AddressBook();   // giải phóng mọi Contact đúng một lần (qua nameIndex.destroyAll())

    // ---- Thêm / xóa / sửa (A viết) ----
    // Thêm c (đã có sẵn tên + các số). Trả về false nếu: dữ liệu sai, TRÙNG SỐ, hoặc TRÙNG TÊN.
    // Khi trùng tên, Menu (B) sẽ hỏi người dùng rồi gọi mergeInto().
    bool addContact(Contact* c);

    // Xóa liên hệ theo tên: gỡ mọi số khỏi bảng băm, gỡ khỏi cây, rồi freeContact().
    bool removeContact(const string& name);

    bool addPhoneTo(const string& name, const string& phone);
    bool removePhoneFrom(const string& name, const string& phone);

    // Gộp liên hệ trùng tên: chuyển các số của extra sang existing, rồi freeContact(extra).
    // Quy ước: extra CHƯA nằm trong bảng băm hay cây.
    bool mergeInto(Contact* existing, Contact* extra);

    // ---- Tra cứu ----
    Contact* findByPhone(const string& phone);   // qua HashTable (A)
    Contact* findByName(const string& name);     // qua NameTree  (B)

    // ---- Hiển thị: chuyển tiếp sang NameTree (B) ----
    void printAll();
    void printByGroup(int group);
    void searchPrefix(const string& prefix);

    // ---- Thống kê (A): hệ số tải, số lần dò trung bình, chiều cao cây ----
    void printStats();

    int size() const { return nameIndex.size(); }

    // FileManager (B) cần duyệt cây để ghi tệp nên được xem cây (chỉ đọc).
    const NameTree& names() const { return nameIndex; }

private:
    HashTable phoneIndex;   // phần của A
    NameTree  nameIndex;    // phần của B

    AddressBook(const AddressBook&) = delete;
    AddressBook& operator=(const AddressBook&) = delete;
};

#endif
