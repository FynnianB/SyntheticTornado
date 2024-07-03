#include "horizontalslicetostreamlinemapper.h"
#include <iostream>

HorizontalSliceToStreamlineMapper::HorizontalSliceToStreamlineMapper()
    : dataSource(nullptr)
{
}

void HorizontalSliceToStreamlineMapper::setDataSource(FlowDataSource *dataSource)
{
    this->dataSource = dataSource;
    this->sizeX = dataSource->getSizeX();
    this->sizeY = dataSource->getSizeY();
    this->sizeZ = dataSource->getSizeZ();
}

QVector<QVector3D> HorizontalSliceToStreamlineMapper::mapSliceToStreamlines(const QVector3D &startPoint, float stepSize, int maxSteps)
{
    QVector<QVector3D> streamline;
    QVector3D currentPoint = startPoint;

    for (int i = 0; i < maxSteps; ++i) {
        streamline.append(currentPoint);
        currentPoint = rungeKuttaIntegration(currentPoint, stepSize);

        // Abbruchbedingung, wenn der Punkt das Datenvolumen verlässt
        if (currentPoint.x() < 0 || currentPoint.y() < 0 || currentPoint.z() < 0 ||
            currentPoint.x() > 1 || currentPoint.y() > 1 || currentPoint.z() > 1) {
            break;
        }
    }

    return streamline;
}

QVector<QVector<QVector3D>> HorizontalSliceToStreamlineMapper::generateEvenlySpacedStreamlines(float stepSize, int maxSteps, float dSep, float dTest)
{
    streamlines.clear();
    QVector3D seedPoint = findNextSeedPoint(dSep);

    while (!seedPoint.isNull()) {
        QVector<QVector3D> streamline;

        // Forward integration
        QVector3D currentPoint = seedPoint;
        for (int i = 0; i < maxSteps; ++i) {
            if (isTooCloseToOtherStreamlines(currentPoint, dTest)) break;
            streamline.append(currentPoint);
            currentPoint = rungeKuttaIntegration(currentPoint, stepSize);

            // Abbruchbedingung, wenn der Punkt das Datenvolumen verlässt
            if (currentPoint.x() < 0 || currentPoint.y() < 0 || currentPoint.z() < 0 ||
                currentPoint.x() > 1 || currentPoint.y() > 1 || currentPoint.z() > 1) {
                break;
            }
        }

        // Backward integration
        currentPoint = seedPoint;
        for (int i = 0; i < maxSteps; ++i) {
            if (isTooCloseToOtherStreamlines(currentPoint, dTest)) break;
            streamline.insert(0, currentPoint);
            currentPoint = rungeKuttaIntegration(currentPoint, -stepSize);

            // Abbruchbedingung, wenn der Punkt das Datenvolumen verlässt
            if (currentPoint.x() < 0 || currentPoint.y() < 0 || currentPoint.z() < 0 ||
                currentPoint.x() > 1 || currentPoint.y() > 1 || currentPoint.z() > 1) {
                break;
            }
        }

        streamlines.append(streamline);
        seedPoint = findNextSeedPoint(dSep);
    }

    return streamlines;
}

QVector3D HorizontalSliceToStreamlineMapper::computeVelocityAtPoint(const QVector3D &point)
{
    int x0 = (int)((point.x() * dataSource->getSizeX())+0.5f);
    int y0 = (int)((point.y() * dataSource->getSizeY())+0.5f);
    int z0 = (int)((point.z() * dataSource->getSizeZ())+0.5f);

    float vx = dataSource->getDataValue(y0, z0, x0, 0);
    float vy = dataSource->getDataValue(y0, z0, x0, 1);
    float vz = dataSource->getDataValue(y0, z0, x0, 2);

    return QVector3D(vy, vz, vx);
}

QVector3D HorizontalSliceToStreamlineMapper::eulerIntegration(const QVector3D &currentPoint, float stepSize)
{
    QVector3D velocity = computeVelocityAtPoint(currentPoint);
    return currentPoint + velocity * stepSize;
}

QVector3D HorizontalSliceToStreamlineMapper::rungeKuttaIntegration(const QVector3D &currentPoint, float stepSize)
{
    QVector3D k1 = computeVelocityAtPoint(currentPoint);
    QVector3D k2 = computeVelocityAtPoint(currentPoint + 0.5f * stepSize * k1);
    QVector3D k3 = computeVelocityAtPoint(currentPoint + 0.5f * stepSize * k2);
    QVector3D k4 = computeVelocityAtPoint(currentPoint + stepSize * k3);

    return currentPoint + (stepSize / 6.0f) * (k1 + 2.0f * k2 + 2.0f * k3 + k4);
}

bool HorizontalSliceToStreamlineMapper::isTooCloseToOtherStreamlines(const QVector3D &point, float dTest)
{
    for (const auto &streamline : qAsConst(streamlines)) {
        for (const auto &p : streamline) {
            if ((point - p).length() < dTest) {
                return true;
            }
        }
    }
    return false;
}

QVector3D HorizontalSliceToStreamlineMapper::findNextSeedPoint(float dSep)
{
    int size = 10;
    int ySize = 50;
    for (int z = 1; z <= size; ++z) {
        for (int y = 1; y <= ySize; ++y) {
            for (int x = 1; x <= size; ++x) {
                QVector3D point(float(z) / size, float(y) / ySize, float(x) / size);
                if (!isTooCloseToOtherStreamlines(point, dSep)) {
                    return point;
                }
            }
        }
    }
    return QVector3D(); // Return a null vector if no suitable seed point is found
}
