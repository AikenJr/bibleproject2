#include "Bible.h"
#include "Ref.h"  
#include "Verse.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>

Bible::Bible() {
    infile = "/home/class/csc3004/Bibles/web-complete";
}

Bible::Bible(const string s) { infile = s; }

Verse Bible::lookup(Ref ref, LookupResult& status) {
    ifstream in(infile.c_str());
    if (!in) {
        status = FILE_ERROR;
        return Verse();
    }

    string line;
    while (getline(in, line)) {
        Verse verse(line);
        if (verse.getRef() == ref) {
            status = SUCCESS;
            return verse;
        }
    }

    status = NO_VERSE;
    return Verse();
}

Verse Bible::nextVerse(LookupResult& status) {
    status = NO_VERSE;
    return Verse();
}

string Bible::getBookName(int bookNumber) {
    static const map<int, string> bookMap = {
	{1, "Genesis"},
        {2, "Exodus"},
        {3, "Leviticus"},
        {4, "Numbers"},
        {5, "Deuteronomy"},
        {6, "Joshua"},
        {7, "Judges"},
        {8, "Ruth"},
        {9, "1 Samuel"},
        {10, "2 Samuel"},
        {11, "1 Kings"},
        {12, "2 Kings"},
        {13, "1 Chronicles"},
        {14, "2 Chronicles"},
        {15, "Ezra"},
        {16, "Nehemiah"},
        {17, "Esther"},
        {18, "Job"},
        {19, "Psalms"},
        {20, "Proverbs"},
        {21, "Ecclesiastes"},
        {22, "Song of Solomon"},
        {23, "Isaiah"},
        {24, "Jeremiah"},
        {25, "Lamentations"},
        {26, "Ezekiel"},
        {27, "Daniel"},
        {28, "Hosea"},
        {29, "Joel"},
        {30, "Amos"},
        {31, "Obadiah"},
        {32, "Jonah"},
        {33, "Micah"},
        {34, "Nahum"},
        {35, "Habakkuk"},
        {36, "Zephaniah"},
        {37, "Haggai"},
        {38, "Zechariah"},
        {39, "Malachi"},
        {40, "Matthew"},
        {41, "Mark"},
        {42, "Luke"},
        {43, "John"},
        {44, "Acts"},
        {45, "Romans"},
        {46, "1 Corinthians"},
        {47, "2 Corinthians"},
        {48, "Galatians"},
        {49, "Ephesians"},
        {50, "Philippians"},
        {51, "Colossians"},
        {52, "1 Thessalonians"},
        {53, "2 Thessalonians"},
        {54, "1 Timothy"},
        {55, "2 Timothy"},
        {56, "Titus"},
        {57, "Philemon"},
        {58, "Hebrews"},
        {59, "James"},
        {60, "1 Peter"},
        {61, "2 Peter"},
        {62, "1 John"},
        {63, "2 John"},
        {64, "3 John"},
        {65, "Jude"},
        {66, "Revelation"}
    };

    auto it = bookMap.find(bookNumber);
    if (it != bookMap.end()) {
        return it->second;
    } else {
        return "Unknown Book";
    }
}

string Bible::error(LookupResult status) {
    switch(status) {
        case SUCCESS: return "Success";
        case NO_VERSE: return "Verse not found :(";
        case FILE_ERROR: return "Error opening file :(";
        case NO_BOOK: return "Book not found :(";
        case NO_CHAPTER: return "Chapter not found :(";
        default: return "Unknown error :(";
    }
}

void Bible::display() {
    cout << "Bible file: " << infile << endl;
}

Ref Bible::next(const Ref ref, LookupResult& status) {
    status = NO_VERSE;
    return ref;
}

Ref Bible::prev(const Ref ref, LookupResult& status) {
    status = NO_VERSE;
    return ref;
}
