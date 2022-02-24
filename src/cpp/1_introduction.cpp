#include <iostream>

using namespace std;

struct Vector {
    int size;
    double* values;
};

// constexpr = this can be evaluated at compile time
constexpr int add(int a, int b) {
    return a + b;
}

// new is like malloc but in c
void vector_init(Vector& v, int s) {
    v.size = s;
    v.values = new double[s];
}

void vector_destroy(Vector& v) {
    v.size = 0;
    delete v.values;
    v.values = nullptr;
}

class ClassVector {
public:
    // Initialising elem{..} sets the private value shortly
    ClassVector(int s):elem{new double[s]}, sz{s} {}
    // This uses subscript [], returns reference to allow mutating
    double& operator[](int i) { return elem[i]; }
    int size() { return sz; }
private:
    double* elem;
    int sz;
};

// enum class is different from enum. 
// Enum maps to number so Color::red == Traffic::red
// Enum class is completely unique Color::red != Traffic::red
enum class Color { red, blue, green };
enum class Traffic_light { green, yellow, red };



void cpp1_main() {
    cout << "Hello world" << endl;    

    auto x = 1;

    // Prints 1, increments after assignment
    cout << (x++) << endl;
    // Prints 3, increments then assigns
    cout << (++x) << endl;

    // Values can be assigned like this
    auto val {3.4};

    cout << val << endl;

    // const = I promise not to change this value
    const int dmv = 17;

    // constexpr = Evaluated at compile time
    constexpr int dmvAdded = add(dmv, 3);

    cout << dmvAdded << endl;


    // Simpler for statement for traversing arrays
    int v1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (auto x : v1) {
        cout << x << endl;
    }   
    for (auto x : {1, 4, 7}) {
        cout << x << endl;
    }  

    // & means "reference to" where mutating changes the value without pointer dereferencing
    for (auto& x : v1) {
        x = x * 2;
    }
    for (auto x : v1) {
        cout << x << endl;
    }  

    // reference mutates what it's assigned to
    int& reference = v1[0];
    cout << reference << endl;
    reference *= 2;
    cout << v1[0] << endl;

    // nullptr type is shared between all pointer types
    double* pd = nullptr;
    if (pd == nullptr) {
        cout << "pd is a nullptr" << endl;
    }

    Vector vec;
    vector_init(vec, 10);

    for (auto x = 0; x < 10; x++) {
        cout << vec.values[x] << endl;
    }

    vector_destroy(vec);
    if (vec.values == nullptr) {
        cout << "Vector was destroyed" << endl;
    }

    ClassVector classVec(6);
    cout << "Created vector of size: " << classVec.size() << endl;

    // Static assert finds compile time errors
    static_assert(5 > 3, "5 is not greater than 3!");
    // static_assert(5 > 6, "5 is not greater than 6!");


    // This fails because enum class cannot be outputted
    // Color col = Color::red;
    // cout << "Colour is " << col << endl;
}
