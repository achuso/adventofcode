#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    Solution(string filename) {
        file.open(filename);
        if (!file.is_open()) {
            throw runtime_error("Failed to open file: " + filename);
        }
    }

    ~Solution() {
        file.close();
    }

    int Solve_1() {
        resetFile();
        int sum = 0;
        string line;
        while(getline(file, line)) {
            vector<int> keys = extractDigits(line);
            sum += 10*keys.at(0) + keys.back();
        }
        return sum;
    }

    int Solve_2() {
        resetFile();
        int sum = 0;
        string line;
        while(getline(file, line)) {
            vector<int> keys = extractTrueValues(line);
            sum += 10*keys.at(0) + keys.back();
        }
        return sum;
    }

    vector<int> extractDigits(const string& line) {
        vector<int> values;

        for(const char& c: line)
            if(std::isdigit(c))
                values.push_back(c - '0');

        if (values.empty()) return {0};
        return values;
    }

    vector<int> extractTrueValues(const string& line) {
        vector<int> values;
        const map<string, int> literals = { 
            {"zero", 0},
            {"one", 1},
            {"two", 2},
            {"three", 3},
            {"four", 4},
            {"five", 5},
            {"six", 6},
            {"seven", 7},
            {"eight", 8},
            {"nine", 9}
        };

        for(int i = 0; i < line.length(); i++) {
            if(std::isdigit(line[i]))
                values.push_back(line[i] - '0');
            else {
                for(const auto& pair: literals) {
                    if(line.substr(i, pair.first.length()).compare(pair.first) == 0 ) // match between folling substr and number literal
                            values.push_back(pair.second);
                }
            }
        }

        if(values.empty()) return {0};
        return values;
    }

    void resetFile() {
        file.clear();
        file.seekg(0, std::ios::beg);
    }

private:
    ifstream file;
};

int main() {
    Solution sol("day1_input.txt");
    cout << "part1 ans: " << sol.Solve_1() << endl;
    cout << "part2 ans: " << sol.Solve_2() << endl;
    return 0;
}