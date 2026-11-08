# Quản lý danh bạ điện thoại (Đề tài 7 - Cấu trúc dữ liệu và giải thuật)

Chương trình quản lý danh bạ: tra cứu nhanh theo **số điện thoại** (bảng băm địa chỉ mở)
và theo **tên** (cây tìm kiếm nhị phân). Toàn bộ cấu trúc dữ liệu tự cài đặt, không dùng
`vector`, `list`, `stack`, `queue`, `map`, `set`, `unordered_map`, `priority_queue`, `sort`, `find` của STL.

## Phân công

| Phần | File | Phụ trách |
|---|---|---|
| Struct dùng chung | `src/core/Contact.h/.cpp` | **CHUNG** |
| Danh sách số điện thoại | `src/core/PhoneList.h/.cpp` | **A** |
| Bảng băm | `src/core/HashTable.h/.cpp` | **A** |
| Kiểm tra số điện thoại, nhóm | `src/core/Validator.h/.cpp` | **A** |
| Đo thời gian | `src/core/Timer.h` | **A** |
| **Nối A và B** | `src/core/AddressBook.h/.cpp` | **A** viết, **B** review |
| Xử lý chuỗi | `src/core/StringUtils.h/.cpp` | **B** |
| Cây tên (BST) | `src/core/NameTree.h/.cpp` | **B** |
| Đọc/ghi tệp | `src/io/FileManager.h/.cpp` | **B** |
| Menu console | `src/ui/Menu.h/.cpp` | **B** |
| Điểm vào chương trình | `src/main.cpp` | **B** |
| Sinh dữ liệu lớn | `tools/gen_data.cpp` | **A** |
| Test + thực nghiệm bảng băm | `tests/test_hash.cpp`, `tools/exp_hash.cpp` | **A** |
| Test + thực nghiệm cây | `tests/test_tree.cpp`, `tools/exp_tree.cpp` | **B** |
| Dữ liệu mẫu (30+ liên hệ) | `data/contacts_sample.txt` | **B** |

Mỗi file có ghi chú đầu file cho biết ai phụ trách; mỗi chỗ cần code có `TODO (A)` hoặc `TODO (B)`.

## Cách hai phần nối nhau

```
Menu (B) ──► AddressBook (A viết, B review) ──┬──► HashTable (A)  : tra theo số điện thoại
FileManager (B) ──► AddressBook               └──► NameTree  (B)  : tra theo tên, in A-Z, tiền tố
                         Contact (chung): mỗi liên hệ được cấp phát một lần, hai cấu trúc chỉ giữ con trỏ
```

## Biên dịch và chạy

```
make            # biên dịch -> build/danhba
make run        # chạy với data/contacts_sample.txt
make test_hash  # test riêng của A
make test_tree  # test riêng của B
make leaks      # kiểm tra rò rỉ bộ nhớ trên macOS
make clean
```

## Quy tắc làm việc với Git

- Không push thẳng vào `main`. A làm trên nhánh `feature/phone-hash`, B làm trên `feature/name-tree`.
- Chỉ sửa file của mình. Muốn sửa file của người kia thì nhắn họ hoặc mở Pull Request.
- Mỗi Pull Request phải được người kia đọc và giải thích lại được trước khi merge.
