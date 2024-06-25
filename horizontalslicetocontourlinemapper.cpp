#include "horizontalslicetocontourlinemapper.h"
#include <iostream>

HorizontalSliceToContourLineMapper::HorizontalSliceToContourLineMapper()
    : sliceFilter(nullptr)
{
}

void HorizontalSliceToContourLineMapper::setSliceFilter(CartesianGridToHorizontalSliceFilter *filter)
{
    sliceFilter = filter;
}

QVector<QVector3D> HorizontalSliceToContourLineMapper::mapSliceToContourLineSegments()
{
    int width = sliceFilter->getSizeX();
    int height = sliceFilter->getSizeY();
    int sliceIndex = sliceFilter->getSliceIndex();
    std::vector<float> isoValues = {-0.1f, 0.0f, 0.1f};

    auto slice = sliceFilter->getFilteredSlice();

    QVector<QVector3D> lineSegments;

    for (int y = 0; y < height - 1; ++y) {
        for (int x = 0; x < width - 1; ++x) {
            float valTL = slice[y * width + x];
            float valTR = slice[y * width + x + 1];
            float valBR = slice[(y + 1) * width + x + 1];
            float valBL = slice[(y + 1) * width + x];

            for (float isoValue : isoValues) {
                bool topLeft = valTL > isoValue;
                bool topRight = valTR > isoValue;
                bool bottomRight = valBR > isoValue;
                bool bottomLeft = valBL > isoValue;

                int caseIndex = getCaseIndex(topLeft, topRight, bottomRight, bottomLeft);

                float step = 1.0f / (width-1);
                float fz = sliceIndex / 15.0f + 0.003f;
                QVector3D vTL(x*step, fz, y*step);
                QVector3D vTR((x+1)*step, fz, y*step);
                QVector3D vBR((x+1)*step, fz, (y+1)*step);
                QVector3D vBL(x*step, fz, (y+1)*step);

                switch (caseIndex) {
                case 0:
                case 15:
                    break;
                case 1:
                case 14:
                    lineSegments.append(isoCrossingBetweenTwoVertices(vBL, vTL, isoValue, valBL, valTL));
                    lineSegments.append(isoCrossingBetweenTwoVertices(vBL, vBR, isoValue, valBL, valBR));
                    break;
                case 2:
                case 13:
                    lineSegments.append(isoCrossingBetweenTwoVertices(vBR, vTR, isoValue, valBR, valTR));
                    lineSegments.append(isoCrossingBetweenTwoVertices(vBR, vBL, isoValue, valBR, valBL));
                    break;
                case 3:
                case 12:
                    lineSegments.append(isoCrossingBetweenTwoVertices(vBL, vTL, isoValue, valBL, valTL));
                    lineSegments.append(isoCrossingBetweenTwoVertices(vBR, vTR, isoValue, valBR, valTR));
                    break;
                case 4:
                case 11:
                    lineSegments.append(isoCrossingBetweenTwoVertices(vTR, vTL, isoValue, valTR, valTL));
                    lineSegments.append(isoCrossingBetweenTwoVertices(vTR, vBR, isoValue, valTR, valBR));
                    break;
                case 5:
                    lineSegments.append(isoCrossingBetweenTwoVertices(vTR, vTL, isoValue, valTR, valTL));
                    lineSegments.append(isoCrossingBetweenTwoVertices(vBL, vBR, isoValue, valBL, valBR));
                    break;
                case 6:
                case 9:
                    lineSegments.append(isoCrossingBetweenTwoVertices(vTR, vTL, isoValue, valTR, valTL));
                    lineSegments.append(isoCrossingBetweenTwoVertices(vBR, vBL, isoValue, valBR, valBL));
                    break;
                case 7:
                case 8:
                    lineSegments.append(isoCrossingBetweenTwoVertices(vTL, vTR, isoValue, valTL, valTR));
                    lineSegments.append(isoCrossingBetweenTwoVertices(vTL, vBL, isoValue, valTL, valBL));
                    break;
                case 10:
                    lineSegments.append(isoCrossingBetweenTwoVertices(vTR, vBR, isoValue, valTR, valBR));
                    lineSegments.append(isoCrossingBetweenTwoVertices(vTL, vBL, isoValue, valTL, valBL));
                    break;
                }
            }
        }
    }

    return lineSegments;
}

int HorizontalSliceToContourLineMapper::getCaseIndex(bool topLeft, bool topRight, bool bottomRight, bool bottomLeft)
{
    return (topLeft << 3) | (topRight << 2) | (bottomRight << 1) | bottomLeft;
}

QVector3D HorizontalSliceToContourLineMapper::isoCrossingBetweenTwoVertices(const QVector3D& v1, const QVector3D& v2, float isoValue, float val1, float val2)
{
    float t = (isoValue - val1) / (val2 - val1);
    return v1 + t * (v2 - v1);
}
