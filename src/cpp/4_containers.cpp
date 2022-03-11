#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

struct PhoneEntry {
    std::string name;
    std::string number;
};

// We can define our own iostream output operator
std::ostream& operator<<(std::ostream& os, const PhoneEntry& e) {
    return os << "(name: " << e.name << ", number: " << e.number << ")";
}

std::istream& operator>>(std::istream& is, PhoneEntry& e) {
    std::string name, number;
    is >> name;
    is >> number;
    e = {name, number};
    
    return is;
}

// Template for outputting iterators
template<typename C> std::ostream& operator<<(std::ostream& os, __wrap_iter<C*>& itt) {
    os << *itt;
    return os;
}

// Template for outputting vectors by outputting their values
template<typename T> std::ostream& operator<<(std::ostream& os, vector<T>& vector) {
    for (auto& value : vector) {
        os << value << endl;
    }
    return os;
}

// Too much effort to write begin and end, so use this to redefine a shorter sort
template<typename Container> void sort(Container& container) {
    sort(container.begin(), container.end());
}

// This makes it easier to write below
template <typename T> using Iterator = typename T::iterator;

// Find all of a value in a container and return a vector of iterator references
// Iterator<C> = typename C::iterator
template <typename C, typename V> vector<Iterator<C>> findAll(C& container, V value) {
    vector<typename C::iterator> response;
    for (auto p = container.begin(); p != container.end(); ++p) {
        if (*p == value) {
            response.push_back(p);
        }
    }
    return response;
}

void cpp4_main() {

    std::string callum = "Callum";
    // Callum Macmillan
    callum += " Macmillan";

    // Something Macmillan. Param 2 = length
    callum.replace(0, 6, "Something");

    // Copy of string, not value. "Something"
    std::string other = callum.substr(0, 9);

    // Callum is not mutated
    other[0] = toupper('w');

    // Easy string comparison
    if (other == "Womething") {
        other += " Else";
    }

    std::cout << callum << " " << other << std::endl;

    // int i;
    // std::cout << "Please input number and press enter" << std::endl;
    // std::cin >> i;
    // std::cout << "You pressed " << i << std::endl;

    PhoneEntry entry { "Callum Macmillan", "07213728" };
    std::cout << entry << std::endl;
    // std::cout << "Input two new things" << std::endl;
    // std::cin >> entry;
    // std::cout << entry << std::endl;

    // vector<PhoneEntry> phoneEntries;
    // vector<PhoneEntry> phoneEntries = {...};
    // vector<PhoneEntry> phoneEntries(8);
    vector<PhoneEntry> phoneEntries(8, { "Blank", "Blank" });
    phoneEntries.push_back({ "End", "End" });
    cout << phoneEntries.size() << endl;
    for (const auto& ent : phoneEntries) {
        cout << ent << endl;
    }


    // List is a doubly linked list useful for deleting, adding elements. Slow for accessing
    list<PhoneEntry> phoneBook = {{ "X", "Y" }, { "A", "B" }};
    
    // Get the iterator of the begining of the phonebook and move to next entry every loop
    for (auto p = phoneBook.begin(); p != phoneBook.end(); ++p) {
        if (p->name == "X") {
            // Add new entry before P?
            phoneBook.insert(p, { "New", "Entry" });
        }
    }

    // Erase the next entry after the first
    phoneBook.erase(++phoneBook.begin());

      for (const auto& ent : phoneBook) {
        cout << ent << endl;
    }

    // Map is implemented as a binary tree
    // Access is O(log(n))
    // This is ordered by ordering function <
    map<string, int> phoneMap {
        { "A", 3},
        { "B", 2},
        { "C", 1}
    };
    // Nice way of overriding / setting values
    phoneMap["B"] = 5;
    cout << phoneMap["B"] << endl;

    // Unordered map uses a hash function so is better at access. Same operations as phone map
    // unordered_map<string, int> phone_book;

    // Standard library provides some algorithms on some types
    // sort sorts in place
    vector<int> values = { 12, 43, 63, 3, 5, 1, 90, 1, 2, 4, 1, 4 };
    sort(values);

    cout << " " << endl;
    cout << values << endl;

    // unique copy removes adjacent duplicates
    vector<int> uniqueValues;
    unique_copy(values.begin(), values.end(), back_inserter(uniqueValues));

    cout << " " << endl;
    cout << uniqueValues << endl;

    auto foundValue = find(uniqueValues.begin(), uniqueValues.end(), 5);
    cout << *foundValue << endl;

    cout << "" << endl;
    auto allFours = findAll(values, 4);
    cout << allFours << endl;

    // We can define the find completion by lambda func
    // Find the first instance of it greater than 5
    auto newFound = find_if(
        uniqueValues.begin(), 
        uniqueValues.end(), 
        [](int value) { return value > 5; }
    );
    cout << newFound << endl;

    // We could also use a function object:
    struct Greater_than {
        int val;
        Greater_than(int v): val{v} {}
        bool operator()(int other){ 
            return other > val;
        }
    };

    auto newFoundStruct = find_if(
        uniqueValues.begin(), 
        uniqueValues.end(), 
        Greater_than{5}
    );

    cout << *newFoundStruct << " " << Greater_than{5}(6) << endl;


}
