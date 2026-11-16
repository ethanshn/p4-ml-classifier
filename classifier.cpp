#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
        return 1;
    }

    ifstream fin(argv[1]);
    ifstream fin2(argv[2]);
    if (!fin.is_open()) {
        cout << "Error opening file: " << argv[1] << endl;

    }


}
