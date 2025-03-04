#ifndef Ref_H
#define Ref_H
#include <string>
#include <stdlib.h>
using namespace std;

string GetNextToken(string& str, const string& delimiters);

class Ref {
private:
   short book, chap, verse;

public:
   Ref();
   Ref(string s);
   Ref(const int, const int, const int);

   int getBook() const; //access the book
   int getChap() const; //access the chapter
   int getVerse() const; //access the verse

   bool operator==(const Ref&) const;
   bool operator<(const Ref&) const;  // Less than operator for map functionality
   void display() const;
};

#endif
