#ifndef HORIZONTALSLICETOIMAGEMAPPER_H
#define HORIZONTALSLICETOIMAGEMAPPER_H

#include <QOpenGLWidget>
#include <vector>
#include "../filter/cartesiangridtohorizontalslicefilter.h"

class HorizontalSliceToImageMapper
{
public:
    HorizontalSliceToImageMapper();

    void setSliceFilter(CartesianGridToHorizontalSliceFilter *filter);
    QImage mapSliceToImage();

private:
    CartesianGridToHorizontalSliceFilter *sliceFilter;

    QColor viridisColourMap(float value);

};

#endif // HORIZONTALSLICETOIMAGEMAPPER_H
