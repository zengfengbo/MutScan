#ifndef MATCH_H
#define MATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "read.h"

using namespace std;

class Match{
public:
    Match(Read* r, int pos, int distance, bool reversed = false);
    ~Match();
    void print();
    void setReversed(bool flag);

private:
    Read* mRead;
    int mDistance;
    bool mReversed;
    int mPos;
};


#endif