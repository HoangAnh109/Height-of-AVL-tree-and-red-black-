#include <iostream>
#include <fstream>
#include <random>

using namespace std;

int main() {
    const int numFiles = 10;
    const int numIntegers = 1000000;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1'000'000'000);

    for (int i = 1; i <= numFiles; ++i) {
        string filename = "test_file_" + to_string(i) + ".txt";
        ofstream file(filename);

        if (!file) {
            cout << "Failed to create file: " << filename << endl;
            continue;
        }

        for (int j = 0; j < numIntegers; ++j) {
            int randomNum = dis(gen);
            file << randomNum << endl;
        }

        file.close();
        cout << "File created: " << filename << endl;
    }

    return 0;
}
