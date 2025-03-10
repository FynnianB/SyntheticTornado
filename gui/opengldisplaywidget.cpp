#include "opengldisplaywidget.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <iostream>
#include "math.h"


OpenGLDisplayWidget::OpenGLDisplayWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      distanceToCamera(-8.0),
      currentSlice(0),
      timestamp(0),
      animationDirection(1),
      showStreamlines(true),
      evenlySpacedStreamlines(false)
{
    setFocusPolicy(Qt::StrongFocus);
}


OpenGLDisplayWidget::~OpenGLDisplayWidget()
{
    // Clean up visualization pipeline.
    makeCurrent();

    delete bboxRenderer;
    delete dataSource;
    delete sliceFilter;
    delete sliceMapper;
    delete sliceRenderer;
    delete contourMapper;
    delete contourRenderer;
    delete streamlineMapper;
    delete streamlineRenderer;

    doneCurrent();
}


QString OpenGLDisplayWidget::openGLString()
{
    QString profileStr;
    switch (format().profile())
    {
    case QSurfaceFormat::NoProfile:
        profileStr = "no profile";
        break;
    case QSurfaceFormat::CompatibilityProfile:
        profileStr = "compatibility profile";
        break;
    case QSurfaceFormat::CoreProfile:
        profileStr = "core profile";
        break;
    }

    return QString("%1.%2 (%3)").arg(format().majorVersion())
            .arg(format().minorVersion()).arg(profileStr);
}


void OpenGLDisplayWidget::initializeGL()
{
    // Query and display some information about the used OpenGL context.
    std::cout << "Initializing OpenGLDisplayWidget with OpenGL version "
              <<  openGLString().toStdString() << ".\n" << std::flush;

    // Set the backgound color of the OpenGL display enable the depth buffer.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0, 0, 0, 1);
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Our own initialization of the visualization pipeline.
    initVisualizationPipeline();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OpenGLDisplayWidget::animate);
    timer->start(50);
}


void OpenGLDisplayWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio of the current viewport.
    float aspectRatio = float(w) / std::max(1, h);

    // Reset projection and set new perspective projection.
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, aspectRatio, 0.05f, 25.0f);

    // Update model-view-projection matrix with new projection.
    updateMVPMatrix();
}


void OpenGLDisplayWidget::paintGL()
{
    // Clear color and depth buffer.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call renderer modules.
    bboxRenderer->drawBoundingBox(mvpMatrix);
    sliceRenderer->drawImage(mvpMatrix);
    contourRenderer->drawContourLines(mvpMatrix);
    if (showStreamlines) {
        streamlineRenderer->drawStreamlines(mvpMatrix);
    }
}


void OpenGLDisplayWidget::mousePressEvent(QMouseEvent *e)
{
    // Save the current position of the mouse pointer for subsequent use
    // in mouseMoveEvent().
    lastMousePosition = QVector2D(e->localPos());
}


void OpenGLDisplayWidget::mouseMoveEvent(QMouseEvent *e)
{
    // If the user holds the left mouse button while moving the mouse, update
    // the rotation angles that specify from which side the grid visualization
    // is viewed.
    if (e->buttons() & Qt::LeftButton)
    {
        // Vector that points from the last stored position of the mouse
        // pointer to the current position.
        QVector2D mousePosDifference = QVector2D(e->localPos()) - lastMousePosition;

        // Update rotation angles in x and y direction. The factor "10" is an
        // arbitrary scaling constant that controls the sensitivity of the
        // mouse.
        rotationAngles.setX(
                    fmod(rotationAngles.x() + mousePosDifference.x()/10.,
                         360.));
        rotationAngles.setY(
                    fmod(rotationAngles.y() + mousePosDifference.y()/10.,
                         360.));

        // Store current position of mouse pointer for next call to this method.
        lastMousePosition = QVector2D(e->localPos());

        // Update model-view-projection matrix with new rotation angles.
        updateMVPMatrix();

        // Redraw OpenGL.
        update();
    }
}


void OpenGLDisplayWidget::wheelEvent(QWheelEvent *e)
{
    // Update distance of the camera to the rendered visualization. The factor
    // "500" is arbitrary and controls that sensitivity of the mouse.
    distanceToCamera += e->delta() / 500.;

    // Update model-view-projection matrix with new distance to camera.
    updateMVPMatrix();

    // Redraw OpenGL.
    update();
}


void OpenGLDisplayWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Up)
    {
        moveSlice(1);
    }
    else if (e->key() == Qt::Key_Down)
    {
        moveSlice(-1);
    }
    else if (e->key() == Qt::Key_X)
    {
        changeWindComponent(0);
    }
    else if (e->key() == Qt::Key_Y)
    {
        changeWindComponent(1);
    }
    else if (e->key() == Qt::Key_Z)
    {
        changeWindComponent(2);
    }
    else if (e->key() == Qt::Key_M)
    {
        changeWindComponent(3);
    }
    else if (e->key() == Qt::Key_C)
    {
        animationDirection = -1;
    }
    else if (e->key() == Qt::Key_V)
    {
        animationDirection = 1;
    }
    else if (e->key() == Qt::Key_Space)
    {
        animationDirection = 0;
    }
    else if (e->key() == Qt::Key_S)
    {
        showStreamlines = !showStreamlines;
    }
    else if (e->key() == Qt::Key_E)
    {
        evenlySpacedStreamlines = !evenlySpacedStreamlines;
        streamlineRenderer->updateLines(evenlySpacedStreamlines);
    }
    else
    {
        return;
    }

    // Redraw OpenGL.
    update();
}


void OpenGLDisplayWidget::updateMVPMatrix()
{
    // Calculate a simple model view transformation from rotation angles
    // and distance to camera.
    // NOTE: Read from bottom to top.

    QMatrix4x4 mvMatrix;
    mvMatrix.translate(0.0, 0.0, distanceToCamera);
    mvMatrix.rotate(rotationAngles.y(), QVector3D(1.0, 0.0, 0.0));
    mvMatrix.rotate(rotationAngles.x(), QVector3D(0.0, 1.0, 0.0));
    mvMatrix.translate(-1.0, -1.0, -1.0);
    mvMatrix.scale(2.0);

    mvpMatrix = projectionMatrix * mvMatrix;
}

void OpenGLDisplayWidget::moveSlice(int steps)
{
    int newSlice = currentSlice + steps;
    if (newSlice >= 0 && newSlice <= 10) {
        currentSlice = newSlice;
        sliceFilter->setSlice(currentSlice);
        sliceRenderer->updateImage();
        sliceRenderer->initImageGeometry(currentSlice);
        contourRenderer->updateLines();
        update();
    }
}

void OpenGLDisplayWidget::changeWindComponent(int ic)
{
    if (ic >= 0 && ic <= 3) {
        currentWindComponent = ic;
        sliceFilter->setWindComponent(currentWindComponent);
        sliceRenderer->updateImage();
        contourRenderer->updateLines();
        update();
    }
}

void OpenGLDisplayWidget::updateTimestamp(int time)
{
    timestamp = time;
    dataSource->createData(timestamp);
    sliceRenderer->updateImage();
    contourRenderer->updateLines();
    streamlineRenderer->updateLines(evenlySpacedStreamlines);
    update();
}

void OpenGLDisplayWidget::animate()
{
    if (animationDirection == 0) return;
    if (evenlySpacedStreamlines) return;

    updateTimestamp(timestamp + animationDirection);
}


void OpenGLDisplayWidget::initVisualizationPipeline()
{
    // Initialize the visualization pipeline:

    // Initialize data source(s).
    dataSource = new FlowDataSource(100, 100, 100);
    dataSource->createData(timestamp);

    // Initialize filter modules.
    sliceFilter = new CartesianGridToHorizontalSliceFilter();
    sliceFilter->setDataSource(dataSource);
    sliceFilter->setSlice(0);
    sliceFilter->setWindComponent(0);

    // Initialize mapper modules.
    sliceMapper = new HorizontalSliceToImageMapper();
    sliceMapper->setSliceFilter(sliceFilter);
    contourMapper = new HorizontalSliceToContourLineMapper();
    contourMapper->setSliceFilter(sliceFilter);
    streamlineMapper = new HorizontalSliceToStreamlineMapper();
    streamlineMapper->setDataSource(dataSource);

    // Initialize rendering modules.
    sliceRenderer = new HorizontalSliceRenderer();
    sliceRenderer->setMapper(sliceMapper);
    contourRenderer = new HorizontalContourLinesRenderer();
    contourRenderer->setMapper(contourMapper);
    bboxRenderer = new DataVolumeBoundingBoxRenderer();
    streamlineRenderer = new HorizontalStreamlineRenderer();
    streamlineRenderer->setMapper(streamlineMapper);
}
