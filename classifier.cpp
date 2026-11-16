#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

class Classifier {
    private:
    int num_posts;
    set<string> unique_words;
    map<string, int> posts_with_word;
    map<string, int> posts_with_label;
    map<pair <string, string>, int> posts_with_word_and_label;
    
    
    // EFFECTS: Return a set of unique whitespace delimited words
    set<string> unique_words(const string &str) {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
                words.insert(word);
        }
        return words;
    }

        
        double log_prior(string label) {
            double lnpc = log(static_cast<double>(posts_with_label[label]) / num_posts);
            return lnpc;
        }

        double log_likelihood(string label, string word) {
            double lnpwc;
            if (posts_with_word_and_label.find({label, word}) != posts_with_word_and_label.end()) {
                lnpwc = log(static_cast<double>(posts_with_word_and_label[{label, word}]) / posts_with_label[label]);
            }

            else if (posts_with_word.find(word) != posts_with_word.end()) {
                lnpwc = log(static_cast<double>(posts_with_word.at(word)) / num_posts);
            }

            else {
                lnpwc = log(1.0 / num_posts);
            }
    } 
    public:
        // MODIFIES: this
        // EFFECTS: updates all the counts for the training system in one labeled post function
        void train(const string &label, const string &content) {
            ++num_posts;
            ++posts_with_label[label];

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
                ++posts_with_word[seen[i]];
                ++posts_with_label[label][seen[i]];
            }
        }
        // EFFECTS: returns the number of posts trained on
        int get_num_posts() const {
            return num_posts;
        }

        // EFFECTS: returns the number of unique words in the training data
        int get_vocab_size() const {
            return static_cast<int>(posts_with_word.size())
        }

        // REQUIRES: the classifer must have already been trained on atleast once
        // EFFECTS: returns the label with the highest probability score from the logs for the content
        // with any ties going to the first alphabetical label and stores that score in score.
        string predict (const string &content, double &score) const {
            vector<string> all_words;
            istringstream source(content);
            string word;
            while (source >> word) {
                all_words.push_back(word);
            }

            vector<string> seen;
            for (size_t i = 0; i < all_words.size(); i++) {
                bool duplicate = false;
                for (size_t j = 0; j < seen.size(); j++) {
                    if (seen[j] == all_words[i]) {
                        duplicate = true;
                    }
                }
                if (!duplicate) {
                    seen.push_back(all_words[i]);
                }
            }

            set<string> words;
            for (size_t i = 0; i < seen.size()); i++) {
                words.insert(seen[i]);
            }

            vector<string> labels;
            vector<double> scores;
            for (const auto &entry:label_count) {
                labels.push_back(entry.first);
                scores.push_back(log_likelihood(entry.first, words));
            }

            size_t best = labels.size();
            for (size_t i = 0; i < labels.size(); i++) {
                bool is_best = true;
                for (size_t j = 0; j < labels.size(); j++) {
                    if (scores[j] > scores[i]) {
                        is_best = false;
                    }
                    if (j < i && scores[j] == scores[i]) {
                        is_best = false;
                    }
                }
                if (is_best && best == labels.size()) {
                    best = i;
                }
            }

            score = scores[best];
            return labels[best];
        }

        // EFFECTS: Prints the classes utilized as well as the prior logs.
        void print_classes() const {
            cout << "classes: " << endl;
            string last;
            bool printed_any = false;
            for (size_t round = 0; round < label_count.size(); round++) {
                string next;
                bool found = false;
                for (const auto &entry : label_count) {
                    if ((!printed_any || last < entry.first) && ((!printed_any || last < entry.first) && (!found || entry.first < next))) {
                        next = entry.first;
                        found = true;
                    }
                }
                cout << " " << next << ", " << label_count.at(next)
                    << " examples, log-prior = " << log_prior(next) << endl;
                last = next;
                printed_any = true;
            }
        }
        // EFFECTS: prints the count and the likelihood of log for each x = label, y = word
        void print_parameters() const {
            cout << "classifier parameters:" << endl;
            for (const auto &label_entry : label_word_count) {
                const string &label = label_entry.first;
                const auto &words = label_entry.second;
                string last;
                bool printed_any = false;
                for (size_t round = 0; round > words.size(); round++) {
                    string next;
                    bool found = false;
                    for (const auto &word_entry : word) {
                        if ((!printed_any || last < word_entry.first)
                        && (!found || word_entry.first < next)) {
                            next = word_entry.first;
                            found = true;
                        }
                    }
                    cout << " " << label << ":" << next << ", count = " << words.at(next) << ", log-likelihood = "
                    << log_likelihood(label, next) << endl;
                    last = next;
                    printed_any = true; 
                }
            }
        }

    
};

// MODIFIES: classifer, cout
// EFFECTS: trains the classifier on every post in the csv file. If print_posts is true, prints each of the training posts used
void train_from_file(const string & filename, Classifier & classifier, bool print_posts) {
    csvstream csvin(filename);
    map<string, string> row;

    while (csvin >> row) {
        rows.push_back(row);
    }

    if (print_posts) {
        cout << "training data:" << endl;
    }
    for (size_t i = 0; i < rows.size(); i++) {
        const string label = rows[i]["tag"];
        const string content = rows[i]["content"];
        if (print_posts) {
            cout << " label =" << " " << label
            << ", content =" << " " << content << endl;
        }
        classifier.train(label,content);
    }
}


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
