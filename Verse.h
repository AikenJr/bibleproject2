#ifndef Verse_H
#define Verse_H
#include <string>
#include <stdlib.h>
#include "Ref.h"
using namespace std;

class Verse {
private:
  Ref verseRef;
  string verseText;

public:
  Verse();

  Verse(const string s);

  string getVerse();
  Ref getRef();

  string getText() { return verseText; }

  void display();
};

#endif //Verse_H
