#include "ShpInfo.h"
#include <iostream>

/**
 * @brief Prints attributes of the ShpInfo object (info from the SHP-file header)
 */
void ShpInfo::printInfo() 
{
    std::cout << "Number of entities: " << n_entities << "\n";
    std::cout << "SHP type: " << shp_type << "\n";
    std::cout << "Padding values: " << "\n";
    for (size_t i{0}; i < 4; ++i)
        std::cout << "Min " << i << ": " << padf_min_bound[i] << " Max " << ": " << padf_max_bound[i] << "\n";
}