#include <iostream>
#include <string>
#include <vector>

using namespace std;

class complex {
    double re, im; // representation: two doubles
public:
    // Multiple function signatures with same name but different args
    complex(double r, double i) :re{r}, im{i} {} 
    complex(double r) :re{r}, im{0} {} 
    complex() :re{0}, im{0} {}
    double real() const { return re; } 
    void real(double d) { re=d; } 
    double imag() const { return im; } 
    void imag(double d) { im=d; }
    complex& operator+=(complex z) { 
        re+=z.re, im+=z.im; return *this; 
    }
    complex& operator==(complex z) { re-=z.re, im-=z.im; return *this; }
    complex& operator*=(complex); // defined out-of-class somewhere
    complex& operator/=(complex); // defined out-of-class somewhere };
    
    string toString() {
        return to_string(re) + " " + to_string(im);
    }
};


// Abstract classes contain virtual functions
class Container {
    public:
        // virtual = 0 means PURE virtual
        virtual double& operator[](int) = 0; 
        virtual int size() const = 0;
        virtual ~Container() {}
};

void use(Container& container) {
    cout << container.size() << endl;
}


class CrapVector: public Container { 
private:
    double* elem; // elem points to an array of sz doubles
    int sz; 
public:
    CrapVector(int s) :elem{new double[s]}, sz{s} {
        for (int i=0; i!=s; ++i) elem[i]=0;
    }

    // The delete operator deallocates memory and calls the destructor for a single object created with new.
    // The delete [] operator deallocates memory and calls destructors for an array of objects created with new [].
    // Using delete on a pointer returned by new [] or delete [] on a pointer returned by new results in undefined behavior.
    ~CrapVector() { delete[] elem; }
    double& operator[](int i) { return elem[i]; }
    int size() const { return sz; }
};

class Printable {
public:
    virtual void print() = 0;
};

class RandomObject: public Printable { 
private:
    int* val;
public:
    RandomObject(int s) :val(new int(s)) {}
    ~RandomObject() { delete val; }

    void print() {
        std::cout << "Value is: " << *val << std::endl;
    }
};

complex& complex::operator*=(complex z) {
    this->re *= z.re;
    this->im *= z.im;
    return *this;
}

std::vector<string> readLines() {
    std::vector<string> v;
    string line;
    cin >> line;
    v.push_back(line);
    return v;
}

Printable* createPrintable() {
    RandomObject* randomObject = new RandomObject(10);
    return randomObject;
}

std::unique_ptr<Printable> createBetterPrintable() {
    return unique_ptr<Printable>(new RandomObject(10));
} 

void print(std::unique_ptr<Printable>& p) {
    p->print();
}



void cpp2_main() {
    complex c1(1, 2);
    complex c2(3, 4);

    c1 *= c2;

    cout << c1.toString() << endl; 

    // cout << "Input a line and press enter" << endl;
    // auto v = readLines();

    // for (auto line : v) {
    //     cout << "You inputted" << endl;
    //     cout << line << endl;
    // }

    CrapVector crap(10);
    use(crap);

    // This is a problem because people need to remember to deallocate the printable
    // Printable* printable = createPrintable();

    // This is better because the printable is destroyed when uniqueptr goes out of scope
    std::unique_ptr<Printable> printable = createBetterPrintable();
    print(printable);

}
