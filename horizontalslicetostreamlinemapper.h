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
    QVector<QVector3D> mapSliceToStreamlines(const QVector3D &startPoint, float stepSize, int maxSteps);
    QVector<QVector<QVector3D>> generateEvenlySpacedStreamlines(float stepSize, int maxSteps, float dSep, float dTest);

private:
    FlowDataSource *dataSource;
    int sizeX, sizeY, sizeZ;
    QVector<QVector<QVector3D>> streamlines;

    QVector3D computeVelocityAtPoint(const QVector3D &point);
    QVector3D eulerIntegration(const QVector3D &currentPoint, float stepSize);
    QVector3D rungeKuttaIntegration(const QVector3D &currentPoint, float stepSize);
    float interpolate(float value1, float value2, float alpha);
    bool isTooCloseToOtherStreamlines(const QVector3D &point, float dTest);
    QVector3D findNextSeedPoint(float dSep);
};

#endif // HORIZONTALSLICETOSTREAMLINEMAPPER_H
