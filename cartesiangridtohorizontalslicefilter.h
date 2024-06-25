#ifndef CARTESIANGRIDTOHORIZONTALSLICEFILTER_H
#define CARTESIANGRIDTOHORIZONTALSLICEFILTER_H

#include "flowdatasource.h"

class CartesianGridToHorizontalSliceFilter
{
public:
    CartesianGridToHorizontalSliceFilter();

    void setDataSource(FlowDataSource *dataSource);
    void setSlice(int iz);
    void setWindComponent(int ic);
    std::vector<float> getFilteredSlice() const;

    int getSizeX() const { return sizeX; }
    int getSizeY() const { return sizeY; }

private:
    FlowDataSource *dataSource;
    int sliceIndex;
    int windComponent;
    int sizeX, sizeY;
};

#endif // CARTESIANGRIDTOHORIZONTALSLICEFILTER_H
