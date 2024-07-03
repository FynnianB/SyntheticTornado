#ifndef HORIZONTALSTREAMLINERENDERER_H
#define HORIZONTALSTREAMLINERENDERER_H

#include "../mapper/horizontalslicetostreamlinemapper.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <iostream>

class HorizontalStreamlineRenderer
{
public:
    HorizontalStreamlineRenderer();
    ~HorizontalStreamlineRenderer();

    void setMapper(HorizontalSliceToStreamlineMapper *mapper);
    void drawStreamlines(QMatrix4x4 mvpMatrix);
    void updateLines(bool evenlySpacedStreamlines);

private:
    void initOpenGLShaders();
    void initGeometry();
    QVector<QVector3D> calculateNormals(const QVector<QVector3D>& streamline);

    HorizontalSliceToStreamlineMapper *sliceMapper;

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QVector<QVector<QVector3D>> lines;
};

#endif // HORIZONTALSTREAMLINERENDERER_H
