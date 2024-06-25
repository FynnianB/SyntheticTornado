#ifndef HORIZONTALCONTOURLINESRENDERER_H
#define HORIZONTALCONTOURLINESRENDERER_H

#include "horizontalslicetocontourlinemapper.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <iostream>

class HorizontalContourLinesRenderer
{
public:
    HorizontalContourLinesRenderer();
    ~HorizontalContourLinesRenderer();

    void setMapper(HorizontalSliceToContourLineMapper *mapper);
    void drawContourLines(QMatrix4x4 mvpMatrix);
    void updateLines();

private:
    void initOpenGLShaders();
    void initGeometry();

    HorizontalSliceToContourLineMapper *sliceMapper;

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QVector<QVector3D> lines;
};

#endif // HORIZONTALCONTOURLINESRENDERER_H
