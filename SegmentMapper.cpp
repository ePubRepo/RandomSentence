//
//  SegmentMapper.cpp
//  random-sentence-generator
//
//  Copyright 2012 <EB>
//
//

#include <iostream>
#include <fstream>
#include "SegmentMapper.h"
#include "catch.hpp"
#include "random.h"

SegmentMapper::SegmentMapper () {
}

SegmentMapper::SegmentMapper (const string &inputFilepat) {
    std::ifstream t(inputFilepat.c_str());
    std::string line;
    while (std::getline(t, line)) {
        this->fileLines.push_back(line);
        this->rawFile += line + "\n";
    }    this->rawFile = this->rawFile.substr(0, this->rawFile.length() - 1);
    t.close();
}

string SegmentMapper::getFirstSegmentName(const string &line) {
    int posFirstSegmentStart = line.find_first_of('<');
    string refinedLine = line.substr(posFirstSegmentStart + 1);

    int posFirstSegmentEnd = refinedLine.find_first_of('>');
    string segmentName = refinedLine.substr(0, posFirstSegmentEnd);
    return segmentName;
}

int SegmentMapper::getSegmentLength(const string &line) {
    int result;
    istringstream convert(line);
    if (!(convert >> result))
        return 0;  // error

    return result;
}

int SegmentMapper::getNumLinesInFile() {
    return this->fileLines.size();
}

void SegmentMapper::storePossibleSegmentValue(const string &key,
                                        const string &possibleSegmentValue) {
    //  Step 1: Determine if Key Exists
    if (this->segmentMapping.find(key) == this->segmentMapping.end()) {
        this->segmentMapping[key] = vector<string>();
    }

    //  Step 2: Append Value
    this->segmentMapping[key].push_back(possibleSegmentValue);
}

void SegmentMapper::parseFile() {
    enum ParseOperation {
        SEGMENT_DEFINITION,
        SEGMENT_LENGTH,
        SEGMENT_VALUE_POSSIBILITY,
        SEGMENT_DELIMETER
    };

    ParseOperation nextExpectedOperation = SEGMENT_DEFINITION;
    string currentlyParsingSegment;
    int currentlyParsingSegmentLength;
    int currentSegmentPossibility = 0;

    int n = 0;
    int numLines = this->fileLines.size();
    while (n < numLines) {
        //  Step 1: Get New Line to Operate On
        //  Step 2: Determine What Action to Take
        //  Step 3: Take Action
        //  Step 4: Determine Next Action
        switch (nextExpectedOperation) {
            case SEGMENT_DEFINITION: {
                currentlyParsingSegment = this->getFirstSegmentName(
                                                        this->fileLines[n]);
                nextExpectedOperation = SEGMENT_LENGTH;
            }
                break;
            case SEGMENT_LENGTH: {
                currentlyParsingSegmentLength = this->getSegmentLength(
                                                        this->fileLines[n]);
                nextExpectedOperation = SEGMENT_VALUE_POSSIBILITY;
                currentSegmentPossibility = 0;
            }
                break;
            case SEGMENT_VALUE_POSSIBILITY: {
                string possibleSegmentValue = this->fileLines[n];
                this->storePossibleSegmentValue(currentlyParsingSegment,
                                                possibleSegmentValue);
                currentSegmentPossibility++;
                if (currentSegmentPossibility >=
                    currentlyParsingSegmentLength) {
                    nextExpectedOperation = SEGMENT_DELIMETER;
                }
            }
                break;
            case SEGMENT_DELIMETER:
                nextExpectedOperation = SEGMENT_DEFINITION;
                break;
        }
        n++;
    }
}

string SegmentMapper::getRawFile() {
    return this->rawFile;
}

vector<string> SegmentMapper::getFileLines() {
    return this->fileLines;
}

string SegmentMapper::populateNonTerminal(const string &terminalName) {
    if (this->segmentMapping.find(terminalName) == this->segmentMapping.end()) {
        return "ERROR";
    }

    vector<string> possibleValues = this->segmentMapping[terminalName];
    int index = randomInteger(0, possibleValues.size() - 1);
    return possibleValues[index];
}

TEST_CASE("RandomSentence/SegmentMapper::getRawFile()", "") {
    string tFileName = "grammars/test.g";
    SegmentMapper tMapper(tFileName);

    REQUIRE(tMapper.getRawFile() == "<start>\n1\nThe <object> tonight.");
}

TEST_CASE("RandomSentence/SegmentMapper::getFileLines()", "") {
    string tFileName = "grammars/test.g";
    SegmentMapper tMapper2(tFileName);

    vector<string> fileLines = tMapper2.getFileLines();
    REQUIRE(fileLines.size() == 3);
    REQUIRE(fileLines[0] == "<start>");
}

TEST_CASE("RandomSentence/SegmentMapper::getFirstSegmentName()", "") {
    SegmentMapper tMapper3;

    REQUIRE(tMapper3.getFirstSegmentName("<start>Now is the time") ==
            "start");
    REQUIRE(tMapper3.getFirstSegmentName("Now is the time to <start>") ==
            "start");
    REQUIRE(tMapper3.getFirstSegmentName("Now is <game> time") ==
                  "game");
    REQUIRE_FALSE(tMapper3.getFirstSegmentName("Now is <sdjsds> time") ==
            "game");
}

TEST_CASE("RandomSentence/SegmentMapper::getSegmentLength()", "") {
    SegmentMapper tMapper4;
    REQUIRE(tMapper4.getSegmentLength("2") == 2);
    REQUIRE(tMapper4.getSegmentLength("12") == 12);
}

TEST_CASE("RandomSentence/SegmentMapper::getNumLinesInFile()", "") {
    string tFileName = "grammars/test-poem.g";
    SegmentMapper tMapper5(tFileName);
    REQUIRE(tMapper5.getNumLinesInFile() == 21);
}

TEST_CASE("RandomSentence/SegmentMapper::storePossibleSegmentValue()", "") {
    string tFileName = "grammars/test2.g";
    SegmentMapper tMapper6(tFileName);
    tMapper6.parseFile();

    REQUIRE(tMapper6.populateNonTerminal("object") == "chicken");
}
