// bibleclient.cpp
// CGI client program for Bible verse retrieval
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"
#include "fifo.h"
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace std;
using namespace cgicc;

// Define logging for client
#define logging
#define LOG_FILENAME "/home/class/csc3004/tmp/aikbarahira-bibleclient.log"
#include "logfile.h"

// Pipe names
const string REQUEST_PIPE = "request_pipe";
const string REPLY_PIPE = "reply_pipe";

int main() {
    // Initialize CGI
    Cgicc cgi;

    // Open the log file
    #ifdef logging
    logFile.open(LOG_FILENAME, ios::app);
    #endif
    log("Bible Client started");
    // Send HTTP header
    cout << HTTPHTMLHeader() << endl;

    try {
        // Get form data
        form_iterator book = cgi.getElement("book");
        form_iterator chapter = cgi.getElement("chapter");
        form_iterator verse = cgi.getElement("verse");
        form_iterator count = cgi.getElement("count");

        // Check if required parameters are provided
        if (book == cgi.getElements().end() || 
            chapter == cgi.getElements().end() || 
            verse == cgi.getElements().end()) {
            cout << "<p>Error: Missing required parameters (book, chapter, verse)</p>" << endl;
            log("Error: Missing required parameters");
            return 1;
        }

        // Parse input parameters
        string bookStr = **book;
        string chapterStr = **chapter;
        string verseStr = **verse;
        string countStr = "1"; // Default to 1 verse

        if (count != cgi.getElements().end()) {
            countStr = **count;
        }

        // Format the request string
        string request = bookStr + ":" + chapterStr + ":" + verseStr;
        if (countStr != "1") {
            request += ":" + countStr;
        }

        log("Sending request: " + request);

        // Create the pipes
        Fifo requestPipe(REQUEST_PIPE);
        Fifo replyPipe(REPLY_PIPE);

        // This opens the pipes
        requestPipe.openwrite();
        log("Request pipe opened for writing");

        replyPipe.openread();
        log("Reply pipe opened for reading");

        // Send the request
        requestPipe.send(request);
        log("Request sent to server");

        // Get the response
        string response = replyPipe.recv();
        log("Received response: " + response);

        // Close the pipes
        requestPipe.fifoclose();
        replyPipe.fifoclose();

        // Parse the response
        istringstream iss(response);
        string statusStr, verseText;

        getline(iss, statusStr, '|');
        getline(iss, verseText);

        int status = stoi(statusStr);

        // Generate the HTML response
        if (status == 0) {
            // Success
            Bible bible;
            string bookName = bible.getBookName(stoi(bookStr));

            cout << "<h2>" << bookName << " " << chapterStr << "</h2>" << endl;
            cout << "<p><strong>" << verseStr << "</strong> " << verseText << "</p>" << endl;
        } else {
            // Error
            cout << "<p>Error: " << verseText << "</p>" << endl;
        }
    } catch (exception &e) {
        cout << "<p>Error: " << e.what() << "</p>" << endl;
        log("Exception: " + string(e.what()));
    }
    
    log("Bible Client finished");
    return 0;
}
