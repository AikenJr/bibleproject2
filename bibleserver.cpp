// bibleserver.cpp
// Server program for Bible verse retrieval with indexing
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"
#include "fifo.h"

using namespace std;

#define logging
#define LOG_FILENAME "/home/class/csc3004/tmp/aikbarahira-bibleserver.log"
#include "logfile.h"

// Pipe names
const string REQUEST_PIPE = "request_pipe";
const string REPLY_PIPE = "reply_pipe";

// Process a verse request and return the result
string processRequest(Bible &bible, const string &request) {
    log("Processing request: " + request);

    istringstream iss(request);
    string bookStr, chapStr, verseStr, countStr;

    	getline(iss, bookStr, ':');
    	getline(iss, chapStr, ':');
   	getline(iss, verseStr, ':');
	getline(iss, countStr);

 int book, chapter, verse, count = 1;
    try {
        book = stoi(bookStr);
        chapter = stoi(chapStr);
        verse = stoi(verseStr);

        if (!countStr.empty()) {
            count = stoi(countStr);
            if (count <= 0) count = 1;
        }
    } catch (const exception &e) {
        log("Error parsing request: " + string(e.what()));
        return "5|Error: Invalid request format - " + string(e.what());
    }

    LookupResult status;
    Ref ref(book, chapter, verse);

    if (count == 1) {
        // Single verse request
        Verse v = bible.lookup(ref, status);

        if (status == SUCCESS) {
            log("Lookup successful: " + v.getText());
            return "0|" + v.getText();
        } else {
            string errorMsg = bible.error(status);
            log("Lookup failed: " + errorMsg);
            return to_string(static_cast<int>(status)) + "|" + errorMsg;
        }
    } else {
        // Multiple verse request
        string result = "";
        bool first = true;

        for (int i = 0; i < count; i++) {
            if (i > 0) {
                ref = bible.next(ref, status);
                if (status != SUCCESS) {
                    if (!first) {
                        break;
                    }
                    // No verses found at all
                    string errorMsg = bible.error(status);
                    log("Lookup failed: " + errorMsg);
                    return to_string(static_cast<int>(status)) + "|" + errorMsg;
                }
            }

            Verse v = bible.lookup(ref, status);
            if (status == SUCCESS) {
                if (!first) {
                    result += " ";
                }
                result += v.getText();
                first = false;
            } else {
                if (!first) {
                    break;
                }
                // No verses found at all
                string errorMsg = bible.error(status);
                log("Lookup failed: " + errorMsg);
                return to_string(static_cast<int>(status)) + "|" + errorMsg;
            }
        }

        log("Multiple lookup successful: " + result);
        return "0|" + result;
    }
}

int main() {
    // build the index
    auto startTime = chrono::high_resolution_clock::now();

    cout << "Bible Server starting..." << endl;
    log("Bible Server starting");

    Bible bible;

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> indexTime = endTime - startTime;

    cout << "Bible index created with " << bible.getIndexSize() << " references" << endl;
    cout << "Index creation time: " << indexTime.count() << " ms" << endl;
    log("Bible index created with " + to_string(bible.getIndexSize()) + " references");
    log("Index creation time: " + to_string(indexTime.count()) + " ms");

    // Create the pipes
    Fifo requestPipe(REQUEST_PIPE);
    Fifo replyPipe(REPLY_PIPE);

    // Open the pipes
    requestPipe.openread();
    cout << "Request pipe opened for reading" << endl;
    log("Request pipe opened for reading");

    replyPipe.openwrite();
    cout << "Reply pipe opened for writing" << endl;
    log("Reply pipe opened for writing");

    while (1) {
        cout << "Waiting for request..." << endl;
        string request = requestPipe.recv();
        if (request == "QUIT") {
            cout << "Received QUIT command, shutting down server" << endl;
            log("Received QUIT command, shutting down server");
            break;
        }
        cout << "Received request: " << request << endl;
        // Process the request
        string response = processRequest(bible, request);
//send rhe response
        replyPipe.send(response);
        cout << "Sent response: " << response << endl;
    }
    // Close the pipes
    requestPipe.fifoclose();
    replyPipe.fifoclose();
    cout << "Bible Server shutdown complete" << endl;
    log("Bible Server shutdown complete");

    return 0;
}
