#ifndef BIBLE_H
#define BIBLE_H

#include <iostream>
#include <string>
#include <map>

using namespace std;  // Add this line to use string without std:: prefix

// Forward declarations
class Ref;
class Verse;

// Lookup result status codes
enum LookupResult {
    SUCCESS,
    NO_BOOK,
    NO_CHAPTER,
    NO_VERSE,
    FILE_ERROR,
    OTHER_ERROR
};

class Bible {
public:
    Bible();
    Bible(const string s);
    
    // Lookup a verse
    Verse lookup(Ref ref, LookupResult& status);
    
    // Get next verse
    Verse nextVerse(LookupResult& status);
    
    // Get book name from number
    string getBookName(int book);
    
    // Get error message
    string error(LookupResult status);
    
    // Display Bible info
    void display();
    
    // Get next/previous references
    Ref next(const Ref ref, LookupResult& status);
    Ref prev(const Ref ref, LookupResult& status);
    
private:
    string infile;
};

#endif // BIBLE_H
