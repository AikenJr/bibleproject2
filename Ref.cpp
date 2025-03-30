#include "Ref.h"
#include <iostream>
#include <string>
using namespace std;

string getBookName(int book) {
    static const string bookNames[] = {
        "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy",
        "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings",
        "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra", "Nehemiah",
        "Esther", "Job", "Psalms", "Proverbs", "Ecclesiastes",
        "Song of Solomon", "Isaiah", "Jeremiah", "Lamentations",
        "Ezekiel", "Daniel", "Hosea", "Joel", "Amos", "Obadiah",
        "Jonah", "Micah", "Nahum", "Habakkuk", "Zephaniah", "Haggai",
        "Zechariah", "Malachi", "Matthew", "Mark", "Luke", "John",
        "Acts", "Romans", "1 Corinthians", "2 Corinthians", "Galatians",
        "Ephesians", "Philippians", "Colossians", "1 Thessalonians",
        "2 Thessalonians", "1 Timothy", "2 Timothy", "Titus", "Philemon",
        "Hebrews", "James", "1 Peter", "2 Peter", "1 John", "2 John",
        "3 John", "Jude", "Revelation"
    };
    if (book > 0 && book <= 66) {
        return bookNames[book - 1];
    }
    return "Unknown Book";
}

string GetNextToken(string& str, const string& delimiters = " ") {
    string::size_type startPos = str.find_first_not_of(delimiters, 0);
    string::size_type endPos = str.find_first_of(delimiters, startPos);
    string next = str.substr(startPos, endPos - startPos);
    string rest = str.substr(endPos - startPos + 1, string::npos);
    str = rest;
    return(next);
}

Ref::Ref() : book(0), chap(0), verse(0) {}

Ref::Ref(const string s) {
    string rtext = s;
    string strbook = GetNextToken(rtext,":");
    book = atoi(strbook.c_str());
    string strchap = GetNextToken(rtext,":");
    chap = atoi(strchap.c_str());
    string strverse = GetNextToken(rtext," ");
    verse = atoi(strverse.c_str());
}

Ref::Ref(const int b, const int c, const int v) : book(b), chap(c), verse(v) {}

int Ref::getBook() const { return book; }
int Ref::getChap() const { return chap; }
int Ref::getVerse() const { return verse; }

bool Ref::operator==(const Ref& r) const {
    return (book == r.book && chap == r.chap && verse == r.verse);
}

// Less than operator
bool Ref::operator<(const Ref& r) const {

//compares book numbers first
    if (book < r.book) return true;
    if (book > r.book) return false;

//compares rge chapter numbers in the same book
    if (chap < r.chap) return true;
    if (chap > r.chap) return false;

    return verse < r.verse;
}

void Ref::display() const {
    cout << getBookName(book) << " " << chap << ":" << verse;
}
