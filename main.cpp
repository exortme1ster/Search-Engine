// Project 2
// Search Engine
// Author: Nikita Mashchenko, U. of Illinois Chicago, Spring 2021


#include <iostream>
#include "search.h"
using namespace std;

int main() {
    
//   cout << cleanToken("....hell's....") << endl;
   
//       cout << cleanToken("+(uic)") << endl;
//       cout << cleanToken("1") << endl;
//       cout << cleanToken("123") << endl;
//       cout << cleanToken("**hi**") << endl;
//     //   cout << cleanToken("*""dd"*") << endl;
//       cout << cleanToken("--1--") << endl;
//       cout << cleanToken("--123--") << endl;
//       cout << cleanToken("a") << endl;
//       cout << cleanToken("--a--") << endl;
//       cout << cleanToken("123a123") << endl;
//       cout << cleanToken("--123a123--") << endl;
//       cout << cleanToken("--a123a--") << endl;
//       cout << cleanToken("a123a") << endl;
      
  
//     set<string> tokens = gatherTokens("to be or not to be");

//     set<string> tokensANS;

//     tokensANS.insert("to");

//     tokensANS.insert("be");

//     tokensANS.insert("or");

//     tokensANS.insert("not");

//     cout << "1. " << (tokens.size() == 4) << endl;

//     cout << "2. " << (tokens == tokensANS) << endl;
    
    map<string, set<string>> nikita;
    
    // buildIndex("tiny.txt", nikita);
    
    
    // findQueryMatches(nikita, "fish -red not");
    
    searchEngine("cplusplus.txt");
    
    // printMap(map<string, set<string>> &index);
    
    // Use this function to call and test the functions you write inside of
    // search.h.  If you do not call the functions here, nothing will run.

    return 0;
}




