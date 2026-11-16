#include <iostream>
#include <fstream>
using namespace std;






int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
        return 1;
    }

    ifstream fin(argv[1]);
    if (!fin.is_open()) {
        cout << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    ifstream fin2;
    if (argc == 3) {
        fin2.open(argv[2]);
        if (!fin2.is_open()) {
            cout << "Error opening file: " << argv[2] << endl;
            return 1;
        }
    }


    int num_posts;
    int unique_words;
    int num_posts_with_words;
    int num_posts_with_label;
    int num_posts_with_words_and_label;


}
