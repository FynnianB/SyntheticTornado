#ifndef HORIZONTALSLICETOIMAGEMAPPER_H
#define HORIZONTALSLICETOIMAGEMAPPER_H

#include <QOpenGLWidget>
#include <vector>
#include "cartesiangridtohorizontalslicefilter.h"

class HorizontalSliceToImageMapper
{
public:
    HorizontalSliceToImageMapper();

    void setSliceFilter(CartesianGridToHorizontalSliceFilter *filter);
    QImage mapSliceToImage();

private:
    CartesianGridToHorizontalSliceFilter *sliceFilter;

};

#endif // HORIZONTALSLICETOIMAGEMAPPER_H
