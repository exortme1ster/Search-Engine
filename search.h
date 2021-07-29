// Search.h
// Author: Nikita Mashchenko, U. of Illinois Chicago, Spring 2021

#pragma once

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iterator>

using namespace std;
// check if every index isalpha
bool hasAlpha(string s) {
        for (size_t k = 0; k < s.size(); k++) {
        if (isalpha(s[k])) {
            return true;
        }
    }
    return false;
}
// cleanToken function that is used to clean
// inputs from file and delete all of the punctuation
// e.g ..,,,Nikita's..., => Nikita
string cleanToken(string s) {
    string temp;
    int size = s.size();
        if (s == "") {
        return s;
    }
    while (true) {
        // if first index is punct => substr
        if (ispunct(s[0])) {
            s = s.substr(1, size);
            size = s.size();
        }
        // if last index is punct => substr
        if (ispunct(s[size-1])) {
            s = s.substr(0, size-1);
            size = s.size();
        }
        // if both first and last indexes are clear => break;
        if (!ispunct(s[0]) && !ispunct(s[size-1])) {
            break;
        }
    }
    if (!hasAlpha(s)) {
        return "";
    }
    // transform everything to tolower
    // e.g. NIKITA => nikita
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
// The helper function gatherTokens extracts the set of
// unique tokens from the body text.
// The single argument to gatherTokens is a string containing
// the body text from a single web page.
// The function returns a set of the unique cleaned tokens that
// appear in that body text.
set<string> gatherTokens(string text) {
    set<string> tokens;
    istringstream ss(text);  // separate sentence on words
    string temp;
    while (ss >> temp) {
        temp = cleanToken(temp);
        if (!temp.empty()) {  // if string is not empty
        tokens.insert(temp);
        }
    }
    return tokens;
}
// The function buildIndex reads the content from
// the database file and processes
// it into the form of an inverted index.
int buildIndex(string filename, map<string, set<string>>& index) {
        ifstream infile(filename);
        string linkname;
        string keywords;
        set<string> temporary;
        int counter = 0;
            if (!infile.good()) {
        cout << "**file not found" << endl;
        return 0;
    }
     while (!infile.eof()) {  // while not end-of-file
        getline(infile, linkname, '\n');
        if (!linkname.empty())  // count how many files were processed
            counter++;
        getline(infile, keywords, '\n');
        temporary = gatherTokens(keywords);
        if (!infile.fail()) {
            for (auto elem : temporary) {
               index[elem].insert(linkname);
            }
        }
    }
    return counter;
}
// The function plusFinder to perform operations
// with sets when we need to intersect them
void plusFinder(string word, set<string> &operatingSet, set<string> &currentSet,
set<string> &resultSet, set<string> &temporarySet,
map<string, set<string>>& index) {
                            int pos = word.find("+");
                            string sub = word.substr(pos + 1);
                            sub = cleanToken(sub);
                            operatingSet = index[sub];
                            currentSet = resultSet;
                            set_intersection(currentSet.begin(),
                            currentSet.end(),
                            operatingSet.begin(), operatingSet.end(),
                            inserter(temporarySet, temporarySet.begin()));
                            resultSet = temporarySet;
                            temporarySet.clear();
}
// The function minusFinder to perform operations
// with sets when we need to make a difference of them
void minusFinder(string word, set<string> &operatingSet, 
set<string> &currentSet, set<string> &resultSet, 
set<string> &temporarySet, map<string, set<string>>& index) {
                            int pos = word.find("-");
                            string sub = word.substr(pos + 1);
                            sub = cleanToken(sub);
                            operatingSet = index[sub];
                            currentSet = resultSet;
                            set_difference(currentSet.begin(),
                            currentSet.end(), operatingSet.begin(), 
                            operatingSet.end(),
                            inserter(temporarySet, temporarySet.begin()));
                            resultSet = temporarySet;
                            temporarySet.clear();
}
// The function defaultWord to perform union
// of two sets
void defaultWord(string word, set<string> &operatingSet, set<string> &currentSet,
set<string> &resultSet, set<string> &temporarySet, 
map<string, set<string>>& index) {
                            word = cleanToken(word);
                            operatingSet = index[word];
                            currentSet = resultSet;
                            set_union(currentSet.begin(), 
                            currentSet.end(),
                            operatingSet.begin(), operatingSet.end(),
                            inserter(temporarySet, 
                            temporarySet.begin()));
                            resultSet = temporarySet;
                            temporarySet.clear();
}
// The function findQueryMatches is used to find matches with the
// input sentence and words in database and then output all links
// that are matching
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> result;
    string cleaned;
    string oneWord;
              stringstream  stream(sentence);  // separate words
              stream >> oneWord;
                    if (index.count(oneWord) >= 0) {  // if one word
                        oneWord = cleanToken(oneWord);
                        result = index[oneWord];  // show results for than word
                    }else {
                             cout << "**not found" << endl;
                        }
                    while (stream >> oneWord) {  // if more than 1 word
                    set<string> workingSet;
                    set<string> curSet;
                    set<string> temp;
                    if (oneWord[0] == '+') {
                        plusFinder(oneWord, workingSet, curSet,
                        result, temp, index);
                    } else if (oneWord[0] == '-') {
                        minusFinder(oneWord, workingSet, curSet,
                        result, temp, index);
                    } else {
                        defaultWord(oneWord, workingSet, curSet,
                        result, temp, index);
                    }
                }
    return result;
}
// This function implements a console program that
// works as follows:
// *It first constructs an inverted index from
// the contents of the database file.
// *It prints how many web pages were processed to build the index and
// how many distinct words were found across all pages.
// *It then enters a loop that prompts the user to enter a query
// *For each query entered by the user, it find the
// matching pages and prints the URLs.
// *The user presses enter (empty string) to indicate they are done
// *and the program finishes executing.
void searchEngine(string filename) {
    string input;
    map<string, set<string>> temp;
    int numPages = buildIndex(filename, temp);
    int size = temp.size();
    set<string> matches;
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << numPages << " pages containing "
    << size << " unique terms" << endl;
    cout << endl;
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, input);
        while (input != "") {  // while not empty string
        cout << "Found " << findQueryMatches(temp, input).size()
        << " matching pages" << endl;
        matches = findQueryMatches(temp, input);
                for (auto it = matches.begin(); it !=
                        matches.end(); ++it)
                  cout << *it << endl;
        cout << endl;
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, input);
        }
        cout << "Thank you for searching!" << endl;
    }
