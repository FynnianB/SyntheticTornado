#include "flowdatasource.h"
#include "tornadoSrc.h"

FlowDataSource::FlowDataSource(int xs, int ys, int zs)
    : xs(xs), ys(ys), zs(zs),
    cartesianDataGrid(xs * ys * zs * 3)
{
}

void FlowDataSource::createData(int time) {
    gen_tornado(xs, ys, zs, time, cartesianDataGrid.data());
}

float FlowDataSource::getDataValue(int iz, int iy, int ix, int ic) {
    if (iz >= 0 && iz < zs && iy >= 0 && iy < ys &&
        ix >= 0 && ix < xs && ic >= 0 && ic < 3) {
        return cartesianDataGrid[index(iz, ix, iy, ic)];
    }
    return 0.0f; // Fehlerwert
}

void FlowDataSource::printValuesOfHorizontalSlice(int iz, int ic) {
    if (iz >= 0 && iz < zs && ic >= 0 && ic < 3) {
        for (int iy = 0; iy < ys; ++iy) {
            for (int ix = 0; ix < xs; ++ix) {
                std::cout << getDataValue(iz, iy, ix, ic) << " ";
            }
            std::cout << std::endl;
        }
    }
}

float FlowDataSource::calculateMagnitude(int iz, int iy, int ix) {
    if (iz >= 0 && iz < zs && iy >= 0 && iy < ys &&
        ix >= 0 && ix < xs) {
        float vx = getDataValue(iz, iy, ix, 0);
        float vy = getDataValue(iz, iy, ix, 1);
        float vz = getDataValue(iz, iy, ix, 2);
        return std::sqrt(vx * vx + vy * vy + vz * vz);
    }
    return 0.0f; // Fehlerwert
}

void FlowDataSource::calculateMinMaxMagnitudes(float &minMagnitude, float &maxMagnitude) {
    minMagnitude = std::numeric_limits<float>::max();
    maxMagnitude = std::numeric_limits<float>::min();

    for (int iz = 0; iz < zs; ++iz) {
        for (int iy = 0; iy < ys; ++iy) {
            for (int ix = 0; ix < xs; ++ix) {
                float magnitude = calculateMagnitude(iz, iy, ix);
                if (magnitude < minMagnitude) {
                    minMagnitude = magnitude;
                }
                if (magnitude > maxMagnitude) {
                    maxMagnitude = magnitude;
                }
            }
        }
    }
}
