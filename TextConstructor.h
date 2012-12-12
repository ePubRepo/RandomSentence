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
    explicit TextConstructor(SegmentMapper sMapper);
    TextConstructor(SegmentMapper sMapper, string startingSegment);
    void constructOutputString();
    string getOutputString();

 private:
    SegmentMapper segMapper;
    string startingSegment = "start";
    string outputString = "";
};

#endif /* defined(__random_sentence_generator__TextConstructor__) */
