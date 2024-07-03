#include "cartesiangridtohorizontalslicefilter.h"

CartesianGridToHorizontalSliceFilter::CartesianGridToHorizontalSliceFilter()
    : dataSource(nullptr), sliceIndex(0), windComponent(0)
{
}

void CartesianGridToHorizontalSliceFilter::setDataSource(FlowDataSource *dataSource)
{
    this->dataSource = dataSource;
    this->sizeX = dataSource->getSizeX();
    this->sizeY = dataSource->getSizeY();
}

void CartesianGridToHorizontalSliceFilter::setSlice(int iz)
{
    this->sliceIndex = iz;
}

void CartesianGridToHorizontalSliceFilter::setWindComponent(int ic)
{
    this->windComponent = ic;
}

std::vector<float> CartesianGridToHorizontalSliceFilter::getFilteredSlice() const
{
    std::vector<float> sliceData;
    if (!dataSource) return sliceData;

    for (int iy = 0; iy < sizeY; ++iy) {
        for (int ix = 0; ix < sizeX; ++ix) {
            if (windComponent == 3) {
                sliceData.push_back(dataSource->calculateMagnitude(sliceIndex, iy, ix));
            } else {
                sliceData.push_back(dataSource->getDataValue(sliceIndex, iy, ix, windComponent));
            }
        }
    }
    return sliceData;
}
