#ifndef HORIZONTALSLICERENDERER_H
#define HORIZONTALSLICERENDERER_H

#include "../mapper/horizontalslicetoimagemapper.h"
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
    void updateImage();
    void initImageGeometry(int currentSlice);

private:
    void initOpenGLShaders();

    HorizontalSliceToImageMapper *sliceMapper;

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLTexture *texture;

};

#endif // HORIZONTALSLICERENDERER_H
