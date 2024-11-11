#include "ShpInfo.h"
#include <iostream>

void ShpInfo::printInfo()
{
    std::cout << "Number of entities: " << numEntities << "\n";
    std::cout << "SHP type: " << shpType << "\n";
    std::cout << "Padding values: " << "\n";
    for (size_t i{0}; i < 4; ++i)
        std::cout << "Min " << i << ": " << padfMinBound[i] << " Max " << ": " << padfMaxBound[i] << "\n";
}