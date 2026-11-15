// ============================================================
// File      : src/io/FileManager.cpp
// Phụ trách : THÀNH VIÊN B
// ============================================================
#include <fstream>
#include <iostream>
#include "io/FileManager.h"
#include "core/AddressBook.h"
#include "core/PhoneList.h"
#include "core/StringUtils.h"
#include "core/Validator.h"

using namespace std;

Contact* parseContactLine(const string& line) {
    int p1 = indexOf(line, '|');
    if (p1 == -1) return nullptr;

    int p2 = indexOf(line, '|', p1 + 1);
    if (p2 == -1) return nullptr;

    int p3 = indexOf(line, '|', p2 + 1);
    if (p3 == -1) return nullptr;

    string name = trim(line.substr(0, p1));

    string phonesStr =
        trim(line.substr(p1 + 1, p2 - p1 - 1));

    string email =
        trim(line.substr(p2 + 1, p3 - p2 - 1));

    string groupStr =
        trim(line.substr(p3 + 1));

    if (name.empty())
        return nullptr;

    int group = 0;

    if (groupStr.empty())
        return nullptr;

    for (size_t i = 0; i < groupStr.length(); i++) {
        if (groupStr[i] < '0' || groupStr[i] > '9')
            return nullptr;

        group = group * 10 + (groupStr[i] - '0');
    }

    Contact* c = createContact(name, email, group);

    int start = 0;

    while (true) {
        int comma = indexOf(phonesStr, ',', start);

        if (comma == -1) {
            string phone =
                trim(phonesStr.substr(start));

            if (!phone.empty())
                addPhone(c, phone);

            break;
        }

        string phone =
            trim(phonesStr.substr(start,
                                comma - start));

        if (!phone.empty())
            addPhone(c, phone);

        start = comma + 1;
    }

    return c;
   
}

bool loadContacts(AddressBook& book, const string& path) {
    ifstream in(path);

    if (!in.is_open())
        return false;

    string line;
    int lineNo = 0;

    while (getline(in, line)) {
        lineNo++;

        string s = trim(line);

        if (s.empty())
            continue;

        if (s[0] == '#')
            continue;

        Contact* c = parseContactLine(s);

        if (c == nullptr) {
            cout << "Canh bao: dong "
                 << lineNo
                 << " sai dinh dang.\n";
            continue;
        }

        if (!book.addContact(c)) {
            cout << "Canh bao: khong the them lien he o dong "
                 << lineNo
                 << ".\n";

            freeContact(c);
        }
    }

    return true;
    
}

void writeContactLine(ostream& out, const Contact* c) {
    if (c == nullptr)
        return;

    out << c->name << "|";

    PhoneNode* p = c->phones;

    bool first = true;

    while (p != nullptr) {
        if (!first)
            out << ",";

        out << p->number;

        first = false;
        p = p->next;
    }

    out << "|"
        << c->email
        << "|"
        << c->group
        << '\n';
   
}

bool saveContacts(const AddressBook& book, const string& path) {
     ofstream out(path);

    if (!out.is_open())
        return false;

    book.names().writeAll(out);

    return true;
    
}
