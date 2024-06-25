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

    for (int j = 0; j < sizeY; ++j) {
        for (int i = 0; i < sizeX; ++i) {
            sliceData.push_back(dataSource->getDataValue(sliceIndex, j, i, windComponent));
        }
    }
    return sliceData;
}
