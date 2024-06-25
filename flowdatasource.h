#ifndef FLOWDATASOURCE_H
#define FLOWDATASOURCE_H

#include <vector>
#include <iostream>
#include <cmath>
#include <limits>

class FlowDataSource
{
public:
    FlowDataSource(int xs, int ys, int zs);

    void createData(int time);
    float getDataValue(int iz, int iy, int ix, int ic);
    void printValuesOfHorizontalSlice(int iz, int ic);
    float calculateMagnitude(int iz, int iy, int ix);
    void calculateMinMaxMagnitudes(float &minMagnitude, float &maxMagnitude);

    int getSizeX() const { return xs; }
    int getSizeY() const { return ys; }
    int getSizeZ() const { return zs; }

private:
    std::vector<float> cartesianDataGrid;
    int xs, ys, zs;

    int index(int iz, int iy, int ix, int ic) const {
        return ((iz * ys + iy) * xs + ix) * 3 + ic;
    }
};

#endif // FLOWDATASOURCE_H
