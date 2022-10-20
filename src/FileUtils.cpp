#include <string>
#include <fstream>

using namespace std;

namespace FileUtils {
    string readFile(string filename) {
        ifstream file(filename);
        string contents((
            std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
        file.close();
        return contents;
    }
}