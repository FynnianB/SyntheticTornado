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
    // Grid Streamlines
    lines = QVector<QVector<QVector3D>>();
    float size = 10.0f;
    float ySize = 50.0f;
    for (int iz = 0; iz <= size; ++iz) {
        for (int iy = 0; iy <= ySize; ++iy) {
            for (int ix = 0; ix <= size; ++ix) {
                lines.append(sliceMapper->mapSliceToStreamlines(QVector3D(iz/size, iy/ySize, ix/size), 0.1f, 48));
            }
        }
    }

    // Even-Spaced-Streamlines (too-slow for animation)
    // lines = sliceMapper->generateEvenlySpacedStreamlines(0.3f, 64, 0.1f, 0.1f);

    initGeometry();
}

QVector<QVector3D> HorizontalStreamlineRenderer::calculateNormals(const QVector<QVector3D>& streamline)
{
    QVector<QVector3D> normals;
    for (int i = 1; i < streamline.size() - 1; ++i) {
        QVector3D p0 = streamline[i - 1];
        QVector3D p1 = streamline[i];
        QVector3D p2 = streamline[i + 1];

        QVector3D tangent1 = p1 - p0;
        QVector3D tangent2 = p2 - p1;

        QVector3D normal = QVector3D::crossProduct(tangent1, tangent2).normalized();
        normals.append(normal);
    }
    return normals;
}

void HorizontalStreamlineRenderer::initGeometry()
{

    QVector<float> vertexData;
    for (const auto& line : qAsConst(lines)) {
        QVector<QVector3D> normals = calculateNormals(line);
        for (int i = 0; i < line.size(); ++i) {
            vertexData.push_back(line[i].x());
            vertexData.push_back(line[i].y());
            vertexData.push_back(line[i].z());

            if (i < normals.size()) {
                vertexData.push_back(normals[i].x());
                vertexData.push_back(normals[i].y());
                vertexData.push_back(normals[i].z());
            } else {
                vertexData.push_back(0.0f);
                vertexData.push_back(0.0f);
                vertexData.push_back(0.0f);
            }
        }
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexData.data(), vertexData.size() * sizeof(float));
    vertexBuffer.release();

    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated())
    {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3, 6 * sizeof(float));
        shaderProgram.enableAttributeArray("vertexPosition");
        shaderProgram.setAttributeBuffer("vertexNormal", GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
        shaderProgram.enableAttributeArray("vertexNormal");
        vertexBuffer.release();
    }
}

void HorizontalStreamlineRenderer::drawStreamlines(QMatrix4x4 mvpMatrix)
{
    if (!sliceMapper) return;

    shaderProgram.bind();
    vertexArrayObject.bind();

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity(); // Assuming no model transformations

    shaderProgram.setUniformValue("model", modelMatrix);

    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);
    shaderProgram.setUniformValue("lightPos", QVector3D(-1.0f, -2.0f, 2.0f));
    shaderProgram.setUniformValue("objectColor", QVector3D(1.0f, 1.0f, 1.0f));  // Farbe der Stromlinie
    shaderProgram.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));

    int offset = 0;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    for (const auto& line : qAsConst(lines)) {
        f->glDrawArrays(GL_LINE_STRIP, offset, line.size());
        offset += line.size();
    }

    vertexArrayObject.release();
    shaderProgram.release();
}

void HorizontalStreamlineRenderer::initOpenGLShaders()
{
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/streamlines_vshader.glsl"))
    {
        std::cout << "Vertex shader error:\n" << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/streamlines_fshader.glsl"))
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
