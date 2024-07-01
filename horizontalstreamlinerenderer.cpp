#include "horizontalstreamlinerenderer.h"

HorizontalStreamlineRenderer::HorizontalStreamlineRenderer()
    : sliceMapper(nullptr),
    vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    initOpenGLShaders();
}

HorizontalStreamlineRenderer::~HorizontalStreamlineRenderer()
{
    vertexArrayObject.destroy();
    vertexBuffer.destroy();
}

void HorizontalStreamlineRenderer::setMapper(HorizontalSliceToStreamlineMapper *mapper)
{
    sliceMapper = mapper;
    updateLines();
}

void HorizontalStreamlineRenderer::updateLines()
{
    lines = QVector<QVector<QVector3D>>();
    int size = 10;
    float step = 1.0f / size;
    for (int iz = 0; iz <= size; ++iz) {
        for (int iy = 0; iy <= size; ++iy) {
            for (int ix = 0; ix <= size; ++ix) {
                lines.append(sliceMapper->mapSliceToStreamlines(QVector3D(iz*step, iy*step, ix*step), 0.1f, 64));
            }
        }
    }

    initGeometry();
}

void HorizontalStreamlineRenderer::initGeometry()
{
    QVector<QVector3D> flattened;
    for (const auto& line : lines) {
        flattened += line;
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(flattened.data(), flattened.size() * sizeof(QVector3D));
    vertexBuffer.release();

    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated())
    {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3, sizeof(QVector3D));
        shaderProgram.enableAttributeArray("vertexPosition");
        vertexBuffer.release();
    }
}

void HorizontalStreamlineRenderer::drawStreamlines(QMatrix4x4 mvpMatrix)
{
    if (!sliceMapper) return;

    shaderProgram.bind();
    vertexArrayObject.bind();

    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    int offset = 0;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    for (const auto& line : lines) {
        f->glDrawArrays(GL_LINE_STRIP, offset, line.size());
        offset += line.size();
    }

    vertexArrayObject.release();
    shaderProgram.release();
}

void HorizontalStreamlineRenderer::initOpenGLShaders()
{
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/simple_vshader.glsl"))
    {
        std::cout << "Vertex shader error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/simple_fshader.glsl"))
    {
        std::cout << "Fragment shader error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.link())
    {
        std::cout << "Shader link error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }
}
