# ============================================================
# Makefile - biên dịch toàn dự án (dùng được trên macOS và Linux)
# Cách dùng:  make          -> build/danhba
#             make run       -> biên dịch và chạy với data/contacts_sample.txt
#             make test_hash / test_tree  -> chạy test riêng của A / B
#             make gen_data  -> sinh dữ liệu lớn (A)
#             make exp_hash / exp_tree    -> thực nghiệm đo thời gian (A / B)
#             make leaks     -> kiểm tra rò rỉ bộ nhớ trên macOS
#             make clean
# ============================================================
CXX      ?= g++
# -Wno-unused-parameter: tạm tắt cảnh báo "tham số chưa dùng" khi code còn là khung.
# Khi code xong có thể xóa cờ này để trình biên dịch nhắc nhở.
CXXFLAGS  = -std=c++17 -Wall -Wextra -Wno-unused-parameter -g -Isrc

CORE = src/core/Contact.cpp src/core/PhoneList.cpp src/core/HashTable.cpp \
       src/core/NameTree.cpp src/core/StringUtils.cpp src/core/Validator.cpp \
       src/core/AddressBook.cpp src/io/FileManager.cpp src/ui/Menu.cpp

.PHONY: all run test_hash test_tree gen_data exp_hash exp_tree leaks clean

all: build/danhba

build:
	mkdir -p build

build/danhba: $(CORE) src/main.cpp | build
	$(CXX) $(CXXFLAGS) $^ -o $@

run: build/danhba
	./build/danhba data/contacts_sample.txt

build/test_hash: tests/test_hash.cpp $(CORE) | build
	$(CXX) $(CXXFLAGS) $^ -o $@
test_hash: build/test_hash
	./build/test_hash

build/test_tree: tests/test_tree.cpp $(CORE) | build
	$(CXX) $(CXXFLAGS) $^ -o $@
test_tree: build/test_tree
	./build/test_tree

build/gen_data: tools/gen_data.cpp | build
	$(CXX) $(CXXFLAGS) $^ -o $@
gen_data: build/gen_data
	./build/gen_data 10000 data/contacts_large.txt

build/exp_hash: tools/exp_hash.cpp $(CORE) | build
	$(CXX) $(CXXFLAGS) $^ -o $@
exp_hash: build/exp_hash
	./build/exp_hash

build/exp_tree: tools/exp_tree.cpp $(CORE) | build
	$(CXX) $(CXXFLAGS) $^ -o $@
exp_tree: build/exp_tree
	./build/exp_tree

# macOS: Valgrind không chạy trên chip M1, dùng lệnh leaks
leaks: build/danhba
	leaks --atExit -- ./build/danhba data/contacts_sample.txt

clean:
	rm -rf build
