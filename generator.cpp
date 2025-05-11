#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <ctime>
#include <filesystem>
#include <sys/stat.h>

const int SMALL_SIZE = 100000;
const int MEDIUM_SIZE = 1000000;
const int LARGE_SIZE = 10000000;

const int SMALL_VALUE = 1000;
const int MEDIUM_VALUE = 1000000;
const int LARGE_VALUE = 1000000000;

const int INSERT_OP = 1;
const int DELETE_OP = 2;
const int SEARCH_OP = 3;

void ensure_directory_exists(const std::string& dirPath) {
    #ifdef _WIN32
    mkdir(dirPath.c_str());
    #else
    mkdir(dirPath.c_str(), 0777);
    #endif
}

std::vector<int> generate_increasing_sequence(int size, int max_value) {
    std::vector<int> result;
    result.reserve(size);

    int step = std::max(1, max_value / size);

    for (int i = 1; i <= size; i++) {
        result.push_back(i * step);
    }

    return result;
}

std::vector<int> generate_decreasing_sequence(int size, int max_value) {
    std::vector<int> result;
    result.reserve(size);

    int step = std::max(1, max_value / size);

    for (int i = 0; i < size; i++) {
        result.push_back((size - i) * step);
    }

    return result;
}

std::vector<int> generate_random_sequence(int size, int max_value) {
    std::vector<int> result;
    result.reserve(size);

    static std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(1, max_value);

    for (int i = 0; i < size; i++) {
        result.push_back(dist(gen));
    }

    return result;
}

std::vector<int> generate_many_equal_sequence(int size, int max_value) {
    static std::mt19937 gen(42);

    std::vector<int> distinct_values(10);
    std::uniform_int_distribution<int> dist(1, max_value);

    for (int i = 0; i < 10; i++) {
        distinct_values[i] = dist(gen);
    }

    std::vector<int> result;
    result.reserve(size);

    std::uniform_int_distribution<int> index_dist(0, 9);

    for (int i = 0; i < size; i++) {
        result.push_back(distinct_values[index_dist(gen)]);
    }

    return result;
}

std::vector<std::pair<int, int>> generate_balanced_operations(const std::vector<int>& data) {
    static std::mt19937 gen(42);

    int size = data.size() / 3;

    std::vector<std::pair<int, int>> operations;
    operations.reserve(size * 3);

    std::vector<int> data_copy = data;

    if (data_copy.size() > size * 3) {
        data_copy.resize(size * 3);
    }

    std::shuffle(data_copy.begin(), data_copy.end(), gen);

    for (int i = 0; i < size; i++) {
        operations.push_back({INSERT_OP, data_copy[i]});
    }

    std::uniform_real_distribution<double> prob_dist(0, 1);
    std::uniform_int_distribution<int> index_dist(0, size - 1);

    int max_val = *std::max_element(data_copy.begin(), data_copy.end());
    std::uniform_int_distribution<int> val_dist(1, 2 * max_val);

    for (int i = 0; i < size; i++) {
        int value;
        if (prob_dist(gen) < 0.7) {
            value = data_copy[index_dist(gen)];
        } else {
            value = val_dist(gen);
        }
        operations.push_back({SEARCH_OP, value});
    }

    for (int i = size; i < 2 * size; i++) {
        operations.push_back({DELETE_OP, data_copy[i]});
    }

    return operations;
}

int main() {
    std::cout << "Test Generator for skipList and treaps data structures" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;

    ensure_directory_exists("teste");

    struct TestCase {
        std::string name;
        int size;
        int max_value;
        std::vector<int> (*generator)(int, int);
    };

    std::vector<TestCase> test_cases = {
        {"small_increasing", SMALL_SIZE, SMALL_VALUE, generate_increasing_sequence},
        {"small_decreasing", SMALL_SIZE, SMALL_VALUE, generate_decreasing_sequence},
        {"small_random", SMALL_SIZE, SMALL_VALUE, generate_random_sequence},
        {"small_equal", SMALL_SIZE, SMALL_VALUE, generate_many_equal_sequence},

        {"medium_increasing", MEDIUM_SIZE, MEDIUM_VALUE, generate_increasing_sequence},
        {"medium_decreasing", MEDIUM_SIZE, MEDIUM_VALUE, generate_decreasing_sequence},
        {"medium_random", MEDIUM_SIZE, MEDIUM_VALUE, generate_random_sequence},
        {"medium_equal", MEDIUM_SIZE, MEDIUM_VALUE, generate_many_equal_sequence},

        {"large_increasing", LARGE_SIZE, LARGE_VALUE, generate_increasing_sequence},
        {"large_decreasing", LARGE_SIZE, LARGE_VALUE, generate_decreasing_sequence},
        {"large_random", LARGE_SIZE, LARGE_VALUE, generate_random_sequence},
        {"large_equal", LARGE_SIZE, LARGE_VALUE, generate_many_equal_sequence}
    };

    for (const auto& test_case : test_cases) {
        std::cout << "Generating test: " << test_case.name << std::endl;

        std::vector<int> data = test_case.generator(test_case.size * 3, test_case.max_value);

        auto operations = generate_balanced_operations(data);

        std::string filename = "teste/" + test_case.name + ".txt";
        std::ofstream file(filename, std::ios::out | std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
            continue;
        }

        file << operations.size() << "\n";

        for (const auto& op : operations) {
            file << op.first << " " << op.second << "\n";
        }

        file.close();
    }

    std::cout << "----------------------------------------------------" << std::endl;
    return 0;
}