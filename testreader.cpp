// testreader.cpp
// Test program for Bible verse retrieval with indexing.
#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"

using namespace std;

// Function to display usage information
void displayUsage(const string &programName) {
    cout << "Usage: " << programName << " <book> <chapter> <verse> [numVerses]" << endl;
    cout << "  book     - Book number (1-66)" << endl;
    cout << "  chapter  - Chapter number" << endl;
    cout << "  verse    - Verse number" << endl;
    cout << "  numVerses - Number of verses to display (optional)" << endl;
}

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc < 4 || argc > 5) {
        displayUsage(argv[0]);
        return 1;
    }
    try {
        // Create Bible object and measure the time it takes to build the index
        auto startTime = chrono::high_resolution_clock::now();

        Bible bible;

        auto endTime = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> indexTime = endTime - startTime;

        // Display index statistics
        cout << "Bible index created with " << bible.getIndexSize() << " references" << endl;
        cout << "Index creation time: " << indexTime.count() << " ms" << endl;
        cout << "Last verse position: " << bible.getLastIndexPosition() << " bytes" << endl;

        // Test specific references to verify index
        Ref testRef1(1, 1, 1);  // Genesis 1:1
        Ref testRef2(43, 3, 16); // John 3:16
        Ref testRef3(66, 22, 21); // Revelation 22:21 (last verse)
        cout << "\nIndex verification:" << endl;
        cout << "Genesis 1:1 position: " << bible.getRefPosition(testRef1) << endl;
        cout << "John 3:16 position: " << bible.getRefPosition(testRef2) << endl;
        cout << "Revelation 22:21 position: " << bible.getRefPosition(testRef3) << endl;
        // Parse command line arguments
        int book = atoi(argv[1]);
        int chap = atoi(argv[2]);
        int verse = atoi(argv[3]);
        int numVerses = 1; // Default to 1 verse
        if (argc == 5) {
            numVerses = atoi(argv[4]);
            if (numVerses <= 0) {
                cout << "Error: Number of verses must be positive" << endl;
                return 1;
            }
        }
        // Display book name and chapter
        cout << "\n" << bible.getBookName(book) << " " << chap << endl;
        // Time the lookup process
        startTime = chrono::high_resolution_clock::now();
        // Look up requested verse(s)
        Ref ref(book, chap, verse);
        LookupResult status;
        if (numVerses == 1) {
            // Look up a single verse
            Verse v = bible.lookup(ref, status);
            if (status == SUCCESS) {
                v.display();
            } else {
                cout << "Error: " << bible.error(status) << endl;
                return 1;
            }
        } else {
            // Look up multiple verses
            for (int i = 0; i < numVerses; i++) {
                Ref currentRef = ref;
                if (i > 0) {
                    // Get next verse reference
                    currentRef = bible.next(currentRef, status);
                    if (status != SUCCESS) {
                        cout << "Error after " << i << " verses: " << bible.error(status) << endl;
                        break;
                    }
                }
                Verse v = bible.lookup(currentRef, status);
                if (status == SUCCESS) {
                    v.display();
                } else {
                    cout << "Error: " << bible.error(status) << endl;
                    break;
                }
                // Update reference for next iteration
                ref = currentRef;
            }
        }
        endTime = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> lookupTime = endTime - startTime;
        cout << "\nLookup time: " << lookupTime.count() << " ms" << endl;
        return 0;
    } catch (const exception &e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}
