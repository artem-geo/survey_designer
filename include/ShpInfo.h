#pragma once

class ShpInfo
{
public:
    int n_entities {-1};
    int shp_type {-1};
    double padf_min_bound[4];
    double padf_max_bound[4];
    ShpInfo() = default;
    void printInfo();
};