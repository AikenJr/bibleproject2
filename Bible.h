#ifndef BIBLE_H
#define BIBLE_H

#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;

class Ref;
class Verse;

//The lookup result status codes
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

    // Index-related functions
    int getIndexSize() const;                     // gets the number of entries in the index
    long getLastIndexPosition() const;            // Getst the position of the last verse in the index
    long getRefPosition(const Ref &ref) const;    // Gets the position of a reference in the file
private:
    string infile;
    map<Ref, long> indexMap;                      // Map of references to file positions
    // Helper functions
    void buildIndex();                            // Build the reference index
    Verse retrieveVerse(ifstream &infile, const Ref &ref, long position, LookupResult& status); // Get verse from position
};

#endif // BIBLE_H
