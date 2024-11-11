#ifndef SHPINFO_H
#define SHPINFO_H

class ShpInfo
{
public:
    int numEntities {-1};
    int shpType {-1};
    double padfMinBound[4];
    double padfMaxBound[4];
    ShpInfo() = default;
    void printInfo();
};


#endif