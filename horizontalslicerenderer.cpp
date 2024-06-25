#include "horizontalslicerenderer.h"

HorizontalSliceRenderer::HorizontalSliceRenderer()
    : sliceMapper(nullptr),
    vertexBuffer(QOpenGLBuffer::VertexBuffer),
    texture(nullptr)
{
    initOpenGLShaders();
    initImageGeometry();
}

HorizontalSliceRenderer::~HorizontalSliceRenderer()
{
    vertexArrayObject.destroy();
    vertexBuffer.destroy();
    if (texture) {
        texture->destroy();
        delete texture;
    }
}

void HorizontalSliceRenderer::setMapper(HorizontalSliceToImageMapper *mapper)
{
    sliceMapper = mapper;

    QImage image = sliceMapper->mapSliceToImage();

    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->create();
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->setData(image);
}

void HorizontalSliceRenderer::initOpenGLShaders()
{
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               ":/texture_vshader.glsl"))
    {
        std::cout << "Vertex shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               ":/texture_fshader.glsl"))
    {
        std::cout << "Fragment shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.link())
    {
        std::cout << "Shader link error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }
}

void HorizontalSliceRenderer::initImageGeometry()
{
    // Vertices of a unit cube that represents the bounding box.
    const float vertices[] = {
        // Positionen          // Texturkoordinaten
        0.0f, 0.0f, 0.0f,       0.0f, 0.0f, // links unten
        1.0f, 0.0f, 0.0f,       1.0f, 0.0f, // rechts unten
        1.0f, 0.0f, 1.0f,       1.0f, 1.0f, // rechts oben
        0.0f, 0.0f, 1.0f,       0.0f, 1.0f,  // links oben
    };

    // Create vertex buffer and upload vertex data to buffer.
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertices, 4 * 5 * sizeof(float)); // 4 Vertices, 5 Werte pro Vertex (3 für Position, 2 für Texturkoordinaten)
    vertexBuffer.release();


    // Store the information OpenGL needs for rendering the vertex buffer
    // in a "vertex array object". This can easily be bound to the OpenGL
    // pipeline during rendering.
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated())
    {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3, 5 * sizeof(float));
        shaderProgram.enableAttributeArray("vertexPosition");

        // Texturkoordinaten als Attribute setzen
        shaderProgram.setAttributeBuffer("textureCoord", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
        shaderProgram.enableAttributeArray("textureCoord");

        vertexBuffer.release();
    }
}

void HorizontalSliceRenderer::drawImage(QMatrix4x4 mvpMatrix)
{
    if (!sliceMapper) return;

    shaderProgram.bind();
    vertexArrayObject.bind();

    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    const int textureUnit = 0; // select a texture unit
    texture->bind(textureUnit);
    shaderProgram.setUniformValue("colorMappingTexture", textureUnit);

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

    vertexArrayObject.release();
    shaderProgram.release();
}
