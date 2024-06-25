#ifndef HORIZONTALSLICERENDERER_H
#define HORIZONTALSLICERENDERER_H

#include "horizontalslicetoimagemapper.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QImage>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <iostream>

class HorizontalSliceRenderer
{
public:
    HorizontalSliceRenderer();
    ~HorizontalSliceRenderer();

    void setMapper(HorizontalSliceToImageMapper *mapper);
    void drawImage(QMatrix4x4 mvpMatrix);

private:
    void initOpenGLShaders();
    void initImageGeometry();

    HorizontalSliceToImageMapper *sliceMapper;

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLTexture *texture;

};

#endif // HORIZONTALSLICERENDERER_H
