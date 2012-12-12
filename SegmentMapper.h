//
//  SegmentMapper.h
//  random-sentence-generator
//
//  Copyright 2012 <EB>
//
//

#ifndef __random_sentence_generator__SegmentMapper__
#define __random_sentence_generator__SegmentMapper__

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class SegmentMapper {
 public:
    SegmentMapper();
    explicit SegmentMapper(const string &filePath);
    string populateNonTerminal(const string &terminalName);
    string getRawFile();
    string getFirstSegmentName(const string &line);
    int getSegmentLength(const string &line);
    vector<string> getFileLines();
    void parseFile();
    int getNumLinesInFile();

 private:
    void storePossibleSegmentValue(const string &key,
                                    const string &possibleSegmentValue);
    string rawFile;
    vector<string> fileLines;
    map<string, vector<string> > segmentMapping;
};

#endif /* defined(__random_sentence_generator__SegmentMapper__) */
