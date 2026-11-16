#include <iostream>
#include <fstream>
#include <set>
#include <map>
using namespace std;

class Classifier {
    public:
    private:
    int num_posts;
    set<string> unique_words;
    map<string, int> posts_with_word;
    map<string, int> posts_with_label;
    map<string, int> posts_with_word_and_label;

    set<string> unique_words_helper_function(const string &str)
    
};




int main(int argc, char *argv[]) {
    cout.precision(3);
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


}
