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

SegmentMapper::SegmentMapper () {
}

SegmentMapper::SegmentMapper (string inputFilepat) {
    std::ifstream t;
    t.open(inputFilepat.c_str());
    std::string line;
    while (std::getline(t, line)) {
        this->rawFile += line + "\n";
        this->fileLines.push_back(line);
    }
    this->rawFile = this->rawFile.substr(0, this->rawFile.length() - 1);
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

void SegmentMapper::storePossibleSegmentValue(const string &key,
                                        const string &possibleSegmentValue) {

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
    int currentSegmentPossibility;

    for (int i = 0; i < this->fileLines.size(); i++) {
        //  Step 1: Get New Line to Operate On
        string newLine = this->fileLines[i];

        //  Step 2: Determine What Action to Take
        //  Step 3: Take Action
        //  Step 4: Determine Next Action
        switch (nextExpectedOperation) {
            case SEGMENT_DEFINITION:
                currentlyParsingSegment = this->getFirstSegmentName(
                                                        this->fileLines[i]);
                nextExpectedOperation = SEGMENT_LENGTH;
                break;
            case SEGMENT_LENGTH:
                currentlyParsingSegmentLength = this->getSegmentLength(
                                                        this->fileLines[i]);
                nextExpectedOperation = SEGMENT_VALUE_POSSIBILITY;
                break;
            case SEGMENT_VALUE_POSSIBILITY:
                string possibleSegmentValue = this->fileLines[i];
                this->storePossibleSegmentValue(currentlyParsingSegment,
                                                possibleSegmentValue);
                currentSegmentPossibility++;
                if (currentSegmentPossibility >=
                    currentlyParsingSegmentLength) {
                    nextExpectedOperation = SEGMENT_DELIMETER;
                }
                break;
            case SEGMENT_DELIMETER:
                nextExpectedOperation = SEGMENT_DEFINITION;
                break;
        }
    }
}

string SegmentMapper::getRawFile() {
    return this->rawFile;
}

vector<string> SegmentMapper::getFileLines() {
    return this->fileLines;
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
