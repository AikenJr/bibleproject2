#include "Verse.h"
#include <iostream>
using namespace std;

Verse::Verse() {
    verseText = "Uninitialized Verse!";
    verseRef = Ref();
}

Verse::Verse(const string s) {
    size_t spacePos = s.find(' ');
    if (spacePos != string::npos) {
        string refStr = s.substr(0, spacePos);
        verseRef = Ref(refStr);
        verseText = s.substr(spacePos + 1);
    } else {
        verseRef = Ref(s);
        verseText = "";
    }
}

string Verse::getVerse() {
    return verseText;
}

Ref Verse::getRef() {
    return verseRef;
}

void Verse::display() {
    verseRef.display();
    cout << " " << verseText;
}
