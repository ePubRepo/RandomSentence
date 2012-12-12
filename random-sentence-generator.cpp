/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */
// Copyright 2012 <EB/Stanford>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim
#include "SegmentMapper.h"
#include "TextConstructor.h"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

static const string kGrammarsDirectory = "grammars/";
static const string kGrammarFileExtension = ".g";
static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? "
                                       "[<return> to quit]: "));
        if (filename.empty() ||
            isValidGrammarFilename(filename)) return filename;
        cout << "Failed to open the grammar file named \""
            << filename << "\". Please try again...." << endl;
    }
}

int main() {
    // CATCH UNIT TESTS
    // Create a default config object
    Catch::Config config;

    // Configure CATCH to run all tests starting with "mytests"
    config.addTestSpec("RandomSentence/*");

    // Forward on to CATCH's main using our custom config.
    // This overload doesn't take command line arguments
    // So the config object must be fully set up
    Catch::Main(config);
    
    while (true) {
        string filename = getFileName();
        if (filename.empty()) break;

        //  Step 1: Read and Parse File Into Segment Map
        SegmentMapper segMapper(getNormalizedFilename(filename));
        segMapper.parseFile();

        //  Step 2: Construct Text Segment by Segment
        TextConstructor tConstructor(segMapper);

        //  Step 3: Print Out the Constructed Text
        cout << tConstructor.getOutputString();
    }

    cout << "Thanks for playing!" << endl;
    return 0;
}
