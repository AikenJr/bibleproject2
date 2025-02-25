/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, February 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matchi>
 * field name, and returning a "form_iterator" object, which contains the actual
 * string the user entered into the corresponding field. The can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.

/* Required libraries for AJAX/CGI functionality */
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

/* Required libraries for AJAX/CGI functionality */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"
using namespace cgicc;

int main() {
    // Set the content type to HTML for better formatting
    cout << "Content-Type: text/html\n\n";

    Cgicc cgi;  // Create object to access CGI request data from form

    // Retrieve input parameters from the web form
    form_iterator st = cgi.getElement("search_type");
    form_iterator book = cgi.getElement("book");
    form_iterator chapter = cgi.getElement("chapter");
    form_iterator verse = cgi.getElement("verse");
    form_iterator nv = cgi.getElement("num_verse");
    
    // Make sure we have all the required form fields
    if (book == cgi.getElements().end() || 
        chapter == cgi.getElements().end() || 
        verse == cgi.getElements().end()) {
        cout << "<div style=\"color: red; margin: 20px 0;\">";
        cout << "<h3>Error: Missing required fields</h3>";
        cout << "<p>Please make sure all fields are filled in and try again.</p>";
        cout << "</div>";
        return 0;
    }

    // Validate user input parameters
    bool validInput = false;
    int bookNum = 0;
    int chapterNum = 0;
    int verseNum = 0;
    int numVerses = 1; // Default to 1 if not specified

    // Convert string inputs to integers
    try {
        bookNum = book->getIntegerValue();
        chapterNum = chapter->getIntegerValue();
        verseNum = verse->getIntegerValue();
        
        if (nv != cgi.getElements().end()) {
            numVerses = nv->getIntegerValue();
        }
        
        // Basic validation of input range
        if (bookNum <= 0 || bookNum > 66) {
            cout << "<div style=\"color: red; margin: 20px 0;\">";
            cout << "<h3>Invalid Book Number</h3>";
            cout << "<p>Book number must be between 1 and 66.</p>";
            cout << "</div>";
        } else if (chapterNum <= 0) {
            cout << "<div style=\"color: red; margin: 20px 0;\">";
            cout << "<h3>Invalid Chapter Number</h3>";
            cout << "<p>Chapter number must be positive.</p>";
            cout << "</div>";
        } else if (verseNum <= 0) {
            cout << "<div style=\"color: red; margin: 20px 0;\">";
            cout << "<h3>Invalid Verse Number</h3>";
            cout << "<p>Verse number must be positive.</p>";
            cout << "</div>";
        } else if (numVerses <= 0) {
            cout << "<div style=\"color: red; margin: 20px 0;\">";
            cout << "<h3>Invalid Number of Verses</h3>";
            cout << "<p>Number of verses must be positive.</p>";
            cout << "</div>";
        } else {
            validInput = true;
        }
    } catch (...) {
        cout << "<div style=\"color: red; margin: 20px 0;\">";
        cout << "<h3>Invalid Input</h3>";
        cout << "<p>Please enter valid numbers for chapter, verse, and number of verses.</p>";
        cout << "</div>";
        return 0;
    }

    if (validInput) {
        // Create a Bible object with the path to the WEB Bible text file
        Bible bible("/home/class/csc3004/Bibles/web-complete");

        // Create a Reference object for the requested verse
        Ref ref(bookNum, chapterNum, verseNum);

        // Lookup the verse
        LookupResult status;
        Verse v = bible.lookup(ref, status);
        
        if (status == SUCCESS) {
            // Format a clean result similar to the example
            
            // First display the reference in bold
            cout << "<h2>" << bible.getBookName(bookNum) << " " << chapterNum << ":" << verseNum;
            
            // If displaying multiple verses, show the range
            if (numVerses > 1) {
                cout << "-" << (verseNum + numVerses - 1);
            }
            cout << "</h2>";
            
            // Then display the verse text
            cout << "<p>" << v.getVerse() << "</p>";
            
            // If multiple verses requested, show the rest
            if (numVerses > 1) {
                for (int i = 1; i < numVerses; i++) {
                    Ref nextRef(bookNum, chapterNum, verseNum + i);
                    LookupResult nextStatus;
                    Verse nextVerse = bible.lookup(nextRef, nextStatus);
                    
                    if (nextStatus == SUCCESS) {
                        cout << "<p>" << nextVerse.getVerse() << "</p>";
                    } else {
                        // We've reached the end of available verses
                        break;
                    }
                }
            }
        } else {
            // Display error message
            cout << "<div style=\"color: red; margin: 20px 0;\">";
            cout << "<h3>Error</h3>";
            cout << "<p>" << bible.error(status) << "</p>";
            cout << "</div>";
        }
    }

    return 0;
}
