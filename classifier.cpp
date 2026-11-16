#include <iostream>
#include <fstream>
#include <set>
#include <map>
using namespace std;

// EFFECTS: returns a set of unique words with no blank whitespaces before or after
set<string> unique_words(const string &str) {
    istringstream source(str);
    set<string> words;
    string word;
    while (source >> word) {
        words.insert(word);
    }
    return words;
}

class Classifier {
    public:
        // MODIFIES: this
        // EFFECTS: updates all the counts for the training system in one labeled post function
        void train(const string &label, const string &content) {
            ++num_posts;
            ++label_count[label];

            vector<string> words;
            istringstream source(content);
            string word;
            while (source >> word) {
                words.push_back(word);
            }

            vector<string> seen;
            for (size_t i = 0; i < words.size(); i++) {
                bool duplicate = false;
                for (size_t j = 0; j < seen.size(); j++) {
                    if (seen[j] == words[i]) {
                        duplicate = true;
                    }
                }
                if (!duplicate) {
                    seen.push_back(words[i]);
                }
            }

            for (size_t i = 0; i < seen.size(); i++) {
                ++word_count[seen[i]];
                ++label_word_count[label][seen[i]];
            }
        }
        // EFFECTS: returns the number of posts trained on
        int get_num_posts() const {
            return num_posts;
        }

        // EFFECTS: returns the number of unique words in the training data
        int get_vocab_size() const {
            return static_cast<int>(word_count.size())
        }

        // REQUIRES: the classifer must have already been trained on atleast once
        // EFFECTS: returns the label with the highest probability score from the logs for the content
        // with any ties going to the first alphabetical label and stores that score in score.

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
