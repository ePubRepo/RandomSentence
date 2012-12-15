//
//  TextConstructor.h
//  random-sentence-generator
//
//  Copyright 2012 <EB>
//
//

#ifndef __random_sentence_generator__TextConstructor__
#define __random_sentence_generator__TextConstructor__

#include <iostream>
#include "SegmentMapper.h"

class TextConstructor {
 public:
    TextConstructor();
    explicit TextConstructor(SegmentMapper sMapper);
    TextConstructor(SegmentMapper sMapper, string startingSegment);
    void constructOutputString();
    string getOutputString();
    bool segmentPresent(const string &outputTxt);

 private:
    SegmentMapper segMapper;
    string outputString = "";
};

#endif /* defined(__random_sentence_generator__TextConstructor__) */
