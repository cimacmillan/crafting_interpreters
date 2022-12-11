#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <stack>

using namespace std;

vector<string> split(const string &file, const string &delimit) {
    vector<string> lines;
    string contents = file;
    while(contents.size() > 0) {
        auto found = contents.find(delimit);
        if (found == string::npos) {
            lines.push_back(contents);
            break;
        }
        auto sub = contents.substr(0, found);
        contents = contents.substr(found + 1, contents.size());
        lines.push_back(std::move(sub));
    }
    return lines;
}

string readFile(const string &filename) {
    ifstream file(filename);
    string contents((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    file.close();
    return contents;
}

struct File {
    uint size;
    unordered_map<string, File*> children;
    File *parent;
};


uint get_total_size(File *file, vector<uint> &sizes) {
    uint result = file->size;
    bool is_dir = false;
    for (auto entry : file->children) {
        is_dir = true;
        result += get_total_size(entry.second, sizes);
    }
    if (is_dir) {
        sizes.push_back(result);
    }
    return result;
}

int get_index(int x, int y, int grid_width) {
    return x + (y * grid_width);
}

void set_height(int *grid, int x, int y, int grid_width, int height) {
    grid[get_index(x, y, grid_width)] = height;
}

int get_height(int *grid, int x, int y, int grid_width) {
    return grid[get_index(x, y, grid_width)];
}

void fire_ray(int *grid, bool *vis_grid, int grid_width, int grid_height, int start_x, int start_y, int d_x, int d_y) {
    int pos_x = start_x;
    int pos_y = start_y;
    int tree_height = -1;

    while (true) {
        int index = get_index(pos_x, pos_y, grid_width);
        int test_height = grid[index];

        if (test_height > tree_height) {
            vis_grid[index] = true;
            tree_height = test_height;
        }

        if (pos_x == 0 && d_x < 0) break;
        if (pos_y == 0 && d_y < 0) break;
        if (pos_x == grid_width - 1 && d_x > 0) break;
        if (pos_y == grid_height - 1 && d_y > 0) break;

        pos_x += d_x;
        pos_y += d_y;
    }
}

// Fires a ray from the start height
// Return the number of trees travelled before being blocked
int fire_distance_ray(int *grid, int grid_width, int grid_height, int start_x, int start_y, int d_x, int d_y, int start_height) {
    int pos_x = start_x + d_x;
    int pos_y = start_y + d_y;
    int distance = 0;

    if (pos_x < 0 || pos_y < 0 || pos_x >= grid_width || pos_y >= grid_height) 
        return 0;

    while (true) {
        int index = get_index(pos_x, pos_y, grid_width);
        int test_height = grid[index];

        distance++;

        if (test_height >= start_height) break;
        if (pos_x == 0 && d_x < 0) break;
        if (pos_y == 0 && d_y < 0) break;
        if (pos_x == grid_width - 1 && d_x > 0) break;
        if (pos_y == grid_height - 1 && d_y > 0) break;

        pos_x += d_x;
        pos_y += d_y;
    }

    return distance;
}

int main() {

    auto input = readFile("src/advent/8.txt");
    vector<string> lines = split(std::move(input), "\n");
    int width = lines[0].size();
    int height = lines.size();
    int *tree_grid = new int[width * height];
    bool *vis_grid = new bool[width * height];

    for (int y = 0; y < lines.size(); y++) {
        auto line = lines[y];
        for (int x = 0; x < line.size(); x++) {
            int height = line[x] - '0';
            set_height(tree_grid, x, y, width, height);
            vis_grid[get_index(x, y, width)] = false;
        }
    }


    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%d ,", get_height(tree_grid, x, y, width));
        }
        printf("\n");
    }

    for (int i = 0; i < width; i++) {
        fire_ray(tree_grid, vis_grid, width, height, i, 0, 0, 1);
        fire_ray(tree_grid, vis_grid, width, height, i, height - 1, 0, -1);
    }
     for (int i = 0; i < height; i++) {
        fire_ray(tree_grid, vis_grid, width, height, 0, i, 1, 0);
        fire_ray(tree_grid, vis_grid, width, height, width - 1, i, -1, 0);
    }

    int count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = get_index(x, y, width);
            if (vis_grid[index]) {
                count++;
            }
        }
    }

    cout << count << endl;

    int max = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int index = get_index(x, y, width);
            int test_height = tree_grid[index];
            int score = 1;
            score *= fire_distance_ray(tree_grid, width, height, x, y, 0, 1, test_height);
            score *= fire_distance_ray(tree_grid, width, height, x, y, 0, -1, test_height);
            score *= fire_distance_ray(tree_grid, width, height, x, y, 1, 0, test_height);
            score *= fire_distance_ray(tree_grid, width, height, x, y, -1, 0, test_height);
            max = score > max ? score : max;
        }
    }

    cout << max << endl;
}
