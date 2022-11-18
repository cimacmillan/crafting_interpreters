#include <fstream>
#include <string>

using namespace std;

namespace FileUtils {
string readFile(string filename) {
  ifstream file(filename);
  string contents((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());
  file.close();
  return contents;
}
} // namespace FileUtils