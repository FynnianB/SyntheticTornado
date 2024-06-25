#ifndef HORIZONTALSLICETOCONTOURLINEMAPPER_H
#define HORIZONTALSLICETOCONTOURLINEMAPPER_H

#include "cartesiangridtohorizontalslicefilter.h"
#include <QVector>
#include <QVector3D>
#include <vector>

class HorizontalSliceToContourLineMapper
{
public:
    HorizontalSliceToContourLineMapper();

    void setSliceFilter(CartesianGridToHorizontalSliceFilter *filter);
    QVector<QVector3D> mapSliceToContourLineSegments();

private:
    CartesianGridToHorizontalSliceFilter *sliceFilter;

    int getCaseIndex(bool topLeft, bool topRight, bool bottomRight, bool bottomLeft);
    QVector3D isoCrossingBetweenTwoVertices(const QVector3D& v1, const QVector3D& v2, float isoValue, float val1, float val2);
};

#endif // HORIZONTALSLICETOCONTOURLINEMAPPER_H
