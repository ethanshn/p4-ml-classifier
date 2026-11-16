#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;
#include "csvstream.hpp"

class Classifier {
    private:
    int num_posts = 0;
    map<string, int> posts_with_word;
    map<string, int> posts_with_label;
    map<pair <string, string>, int> posts_with_word_and_label;
    
    
    // E: Return a set of unique whitespace delimited words
    set<string> unique_words(const string &str) const {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
                words.insert(word);
        }
        return words;
    }

        
        double log_prior(string label) const {
            double lnpc = log(static_cast<double>(posts_with_label.at(label) / num_posts));
            return lnpc;
        }

        double log_likelihood(string label, string word) const {
            double lnpwc;
            if (posts_with_word_and_label.find({label, word}) != posts_with_word_and_label.end()) {
                lnpwc = log(static_cast<double>(posts_with_word_and_label.at({label, word})) / posts_with_label.at(label));
            }

            else if (posts_with_word.find(word) != posts_with_word.end()) {
                lnpwc = log(static_cast<double>(posts_with_word.at(word)) / num_posts);
            }

            else {
                lnpwc = log(1.0 / num_posts);
            }
            return lnpwc;
    } 
    public:
    // M: this
    // E: updates all the counts for the training system
        void train(const string &label, const string &content) {
            ++num_posts;
            ++posts_with_label[label];

            set<string> words = unique_words(content);

            for (string w : words) {
                ++posts_with_word[w];
                ++posts_with_word_and_label[make_pair(label, w)];
            }
        }
        // E: returns the number of posts trained
        int get_num_posts() const {
            return num_posts;
        }

        // E: returns the number of unique words
        int get_vocab_size() const {
            return static_cast<int>(posts_with_word.size());
        }

        // R: the classifer already trained on atleast once
        // E: returns the label with the highest probability score from the logs for the content
        // with any ties going to the first alphabetical label and stores that score in score.
        string predict (const string &content, double &score) const {
            set<string> words = unique_words(content);

            vector<string> labels;
            vector<double> scores;
            for (const auto &entry : posts_with_label) {
                labels.push_back(entry.first);
                double total = log_prior(entry.first);
                for (const string &w : words) {
                    total += log_likelihood(entry.first, w);
                }
                scores.push_back(total);
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

        // E: Prints the classes utilized as well as the prior logs.
        void print_classes() const {
            cout << "classes: " << endl;
            for (auto entry : posts_with_label) {
                cout << " " << entry.first << ", " << entry.second <<
                " examples, log-prior = " << log_prior(entry.first) << endl;
            }
        }
        // E: prints the count and the likelihood of log for each x = label, y = word
        void print_parameters() const {
            cout << "classifier parameters:" << endl;
            for (const auto &entry : posts_with_word_and_label) {
                const string &label = entry.first.first;
                const string &word = entry.first.second;
                cout << " " << label << ":" << word << ", count = " << entry.second <<
                ", log-likelihood = " << log_likelihood(label, word) << endl;
            }
        }

    
};

// M: classifer, cout
// E: trains the classifier on every post in the csv file
void train_from_file(const string & filename, Classifier & classifier, bool print_posts) {
    csvstream csvin(filename);
    map<string, string> row;

    vector<map<string, string>> rows;
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

void test_from_file(const string &filename, const Classifier &classifier) {
    csvstream csvin(filename);
    map<string, string> row;

    vector<map<string, string>> rows;
    while (csvin >> row) {
        rows.push_back(row);
    }

    int correct = 0;
    int total = 0;
    cout << "test data: " << endl;
    for (size_t i = 0; i < rows.size(); i++) {
        const string label = rows[i]["tag"];
        const string content = rows[i]["content"];
        double score = 0;
        const string predicted = classifier.predict(content, score);
        cout << " correct = " << label << ", predicted = " << predicted << ", log-probability score = "
        << score << endl;
        cout << " content = " << content << endl << endl;
        total++;
        if (predicted == label) {
            correct++;
        }
    }
    cout << "performance: " << correct << " / " << total << " posts predicted correctly" << endl;
}

// M: cout
// E: runs training and also testing if asked. Throws a csvstream_exception if the file won't open
void run(const string &train_file, const string *test_file) {
    const bool train_only = (test_file == nullptr);
    const string train_name = train_file;
    Classifier classifier;
    train_from_file(train_name, classifier, train_only);
    cout << "trained on " << classifier.get_num_posts() << " examples" << endl;
    if (train_only) {
        cout << "vocabulary size = " << classifier.get_vocab_size() << endl;
    }
    cout << endl;
    if (train_only) {
        classifier.print_classes();
        classifier.print_parameters();
        cout << endl;
    } else {
        const string test_name = *test_file;
        test_from_file(test_name, classifier);
    }
}


int main(int argc, char *argv[]) {
    cout.precision(3);
    if (argc != 2 && argc != 3) {
        cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
        return 1;
    }

    const string train_file = argv[1];

    try {
        if (argc == 2) {
            run(train_file, nullptr);
        } 
        else {
            string test_file = argv[2];
            run(train_file, &test_file);
        }
    }
    catch (const csvstream_exception &e) {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}
