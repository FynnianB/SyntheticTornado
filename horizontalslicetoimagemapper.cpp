#include "horizontalslicetoimagemapper.h"
#include <iostream>

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper()
    : sliceFilter(nullptr)
{
}

void HorizontalSliceToImageMapper::setSliceFilter(CartesianGridToHorizontalSliceFilter *filter)
{
    sliceFilter = filter;
}

QImage HorizontalSliceToImageMapper::mapSliceToImage()
{
    if (!sliceFilter) return QImage();

    auto sliceData = sliceFilter->getFilteredSlice();
    int i = 0;
    for (const auto &value : sliceData)
    {
        std::cout << value << " ";
        if (++i == sliceFilter->getSizeX()) {
            i = 0;
            std::cout << std::endl;
        }
    }

    return QImage(); // Placeholder
}
