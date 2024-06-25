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

    int width = sliceFilter->getSizeX();
    int height = sliceFilter->getSizeY();

    auto sliceData = sliceFilter->getFilteredSlice();
    QImage image(width, height, QImage::Format_RGB32);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float value = sliceData[y * width + x];

            // Skalierung
            int red = qBound(0, static_cast<int>((value > 0 ? value : 0) * 3 * 255), 255);
            int blue = qBound(0, static_cast<int>((value < 0 ? -value : 0) * 3 * 255), 255);

            QColor color(red, 0, blue);
            image.setPixelColor(x, y, color);
        }
    }

    return image;
}
