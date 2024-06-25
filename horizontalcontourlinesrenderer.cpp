#include "horizontalcontourlinesrenderer.h"

HorizontalContourLinesRenderer::HorizontalContourLinesRenderer()
    : sliceMapper(nullptr),
    vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    initOpenGLShaders();
}

HorizontalContourLinesRenderer::~HorizontalContourLinesRenderer()
{
    vertexArrayObject.destroy();
    vertexBuffer.destroy();
}

void HorizontalContourLinesRenderer::setMapper(HorizontalSliceToContourLineMapper *mapper)
{
    sliceMapper = mapper;
    updateLines();
}

void HorizontalContourLinesRenderer::updateLines()
{
    lines = sliceMapper->mapSliceToContourLineSegments();
    initGeometry();
}

void HorizontalContourLinesRenderer::initGeometry()
{
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(lines.data(), lines.size() * sizeof(QVector3D));
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

void HorizontalContourLinesRenderer::drawContourLines(QMatrix4x4 mvpMatrix)
{
    if (!sliceMapper) return;

    shaderProgram.bind();
    vertexArrayObject.bind();

    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDrawArrays(GL_LINES, 0, lines.size());

    vertexArrayObject.release();
    shaderProgram.release();
}

void HorizontalContourLinesRenderer::initOpenGLShaders()
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
