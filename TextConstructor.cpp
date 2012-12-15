//
//  TextConstructor.cpp
//  random-sentence-generator
//
//  Copyright 2012 <EB>
//
//

#include "TextConstructor.h"
#include "SegmentMapper.h"
#include "catch.hpp"

// default constructor to be used for testing
TextConstructor::TextConstructor() {
    this->segMapper = SegmentMapper("grammar/test-poem.g");
}

TextConstructor::TextConstructor(SegmentMapper sMapper) {
    this->segMapper = sMapper;
}

string TextConstructor::getOutputString() {
    return this->outputString;
}

void TextConstructor::constructOutputString() {
    //  Step 1: Choose Start Segment
    this->outputString = this->segMapper.populateNonTerminal("start");

    //  Step 2: While No Segment in Current Line, Loop Through Line
    //    and Substitute Value for Segment
    while (this->segmentPresent(this->outputString)) {
        string nextSegmentToPopulate =
            SegmentMapper::getFirstSegmentName(this->outputString);
        int nextSegmentStartingPos = this->outputString.find_first_of('<');
        string populatedSegment =
            this->segMapper.populateNonTerminal(nextSegmentToPopulate);

        // replace nextSegmentToPopulate with populatedSegment
        this->outputString.replace(nextSegmentStartingPos,
                                   nextSegmentToPopulate.length() + 2,
                                   populatedSegment);
    }
}

bool TextConstructor::segmentPresent(const string &outputTxt) {
    int posFirstSegmentStart = outputTxt.find_first_of('<');
    return (posFirstSegmentStart != string::npos);
}

TEST_CASE("RandomSentence/TextConstructor::segmentPresent()", "") {
    TextConstructor tConstructor;
    REQUIRE(tConstructor.segmentPresent("dsjdks") == false);
    REQUIRE(tConstructor.segmentPresent("dsd <hello> djksd") == true);
}
