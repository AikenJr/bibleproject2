#include "Bible.h"
#include "Ref.h" 
#include "Verse.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

Bible::Bible() {
    infile = "/home/class/csc3004/Bibles/web-complete";
    buildIndex();
}

Bible::Bible(const string s) { 
    infile = s;
    buildIndex();
}

// Build the reference index
void Bible::buildIndex() {
    ifstream in(infile.c_str());
    if (!in) {
        cout << "Error: Unable to open Bible file for indexing: " << infile << endl;
        return;
    }
    string line;
    long position;
    // Process each line in the file
    while (in.good()) {
        // Get the current position in the file before reading the line
        position = in.tellg();
        // Read the next line
        if (!getline(in, line)) {
            break;
        }
        Verse verse(line);
        Ref ref = verse.getRef();
        // Only add to index if it's a valid reference
        if (ref.getBook() > 0) {
            indexMap[ref] = position;
        }
    }
    in.close();
}

Verse Bible::retrieveVerse(ifstream &in, const Ref &ref, long position, LookupResult& status) {
    if (position < 0 || !in) {
        status = FILE_ERROR;
        return Verse();
    }
    
    // Seek to the position in the file
    in.seekg(position);
    
    // Read the verse line
    string line;
    if (getline(in, line)) {
        Verse verse(line);
        
        // Verify this is the correct verse
        if (verse.getRef() == ref) {
            status = SUCCESS;
            return verse;
        }
    }
    
    status = NO_VERSE;
    return Verse();
}

Verse Bible::lookup(Ref ref, LookupResult& status) {
    // Check if the reference exists in the index
    long position = getRefPosition(ref);
    if (position < 0) {
        // Reference not in index, determine why
        if (ref.getBook() <= 0 || ref.getBook() > 66) {
            status = NO_BOOK;
        } else {
//only checks for verse validity
            status = NO_VERSE;
        }
        return Verse();
    }
    // Open the Bible file
    ifstream in(infile.c_str());
    if (!in) {
        status = FILE_ERROR;
        return Verse();
    }
    // Retrieve the verse from the position
    return retrieveVerse(in, ref, position, status);
}

Verse Bible::nextVerse(LookupResult& status) {
    status = NO_VERSE;
    return Verse();
}

Ref Bible::next(const Ref ref, LookupResult& status) {
    // Find the current reference in the index
    auto it = indexMap.find(ref);
    if (it == indexMap.end()) {
        // Reference not found, try to find the next closest one
        it = indexMap.lower_bound(ref);
        if (it == indexMap.end()) {
            // No reference found after the given reference
            status = NO_VERSE;
            return Ref();
        }
    } else {
        // Found the reference, move to the next one
        ++it;
        if (it == indexMap.end()) {
            // This was the last reference
            status = NO_VERSE;
            return Ref();
        }
    }
    // Return the next reference
    status = SUCCESS;
    return it->first;
}

Ref Bible::prev(const Ref ref, LookupResult& status) {
    status = NO_VERSE;
    return ref;
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
    cout << "Index contains " << indexMap.size() << " verses" << endl;
}

// Index-related utility functions
int Bible::getIndexSize() const {
    return indexMap.size();
}

long Bible::getLastIndexPosition() const {
    if (indexMap.empty()) {
        return -1;
    }
    // Get the last element in the map
    auto lastElement = indexMap.rbegin();
    return lastElement->second;
}

long Bible::getRefPosition(const Ref &ref) const {
    auto it = indexMap.find(ref);
    if (it != indexMap.end()) {
        return it->second;
    }
    return -1;  // Reference not found in index
}
