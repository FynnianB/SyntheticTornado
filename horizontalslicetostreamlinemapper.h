#ifndef HORIZONTALSLICETOSTREAMLINEMAPPER_H
#define HORIZONTALSLICETOSTREAMLINEMAPPER_H

#include "cartesiangridtohorizontalslicefilter.h"
#include "flowdatasource.h"
#include <QVector>
#include <QVector3D>
#include <vector>

class HorizontalSliceToStreamlineMapper
{
public:
    HorizontalSliceToStreamlineMapper();

    void setDataSource(FlowDataSource *dataSource);
    void setSliceFilter(CartesianGridToHorizontalSliceFilter *filter);
    QVector<QVector3D> mapSliceToStreamlines(const QVector3D &startPoint, float stepSize, int maxSteps);

private:
    CartesianGridToHorizontalSliceFilter *sliceFilter;
    FlowDataSource *dataSource;
    int sizeX, sizeY;

    QVector3D computeVelocityAtPoint(const QVector3D &point);
    QVector3D eulerIntegration(const QVector3D &currentPoint, float stepSize);
    QVector3D rungeKuttaIntegration(const QVector3D &currentPoint, float stepSize);
    float interpolate(float value1, float value2, float alpha);
};

#endif // HORIZONTALSLICETOSTREAMLINEMAPPER_H
