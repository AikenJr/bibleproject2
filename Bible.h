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
    // Get the next verse
    Verse nextVerse(LookupResult& status);
    // book name from number
    string getBookName(int book);
    // error message
    string error(LookupResult status);
    // Displaythe Bible info
    void display();

    Ref next(const Ref ref, LookupResult& status);
    Ref prev(const Ref ref, LookupResult& status);

    // Index-related functions
    int getIndexSize() const;                     // gets the number of entries in the index
    long getLastIndexPosition() const;            //position of the last verse
    long getRefPosition(const Ref &ref) const;
private:
    string infile;
    map<Ref, long> indexMap;
    void buildIndex();                            // Building the ref index
    Verse retrieveVerse(ifstream &infile, const Ref &ref, long position, LookupResult& status);
};

#endif // BIBLE_H
