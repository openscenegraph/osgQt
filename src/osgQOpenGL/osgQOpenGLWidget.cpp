#include <osgQOpenGL/osgQOpenGLWidget>
#include <osgQOpenGL/OSGRenderer>

#include <osgViewer/Viewer>
#include <osg/GL>

#include <QApplication>
#include <QKeyEvent>
#include <QInputDialog>
#include <QLayout>
#include <QMainWindow>
#include <QScreen>
#include <QWindow>

osgQOpenGLWidget::osgQOpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
}

osgQOpenGLWidget::osgQOpenGLWidget(osg::ArgumentParser* arguments,
                                   QWidget* parent) :
    QOpenGLWidget(parent),
    _arguments(arguments)
{

}

osgQOpenGLWidget::~osgQOpenGLWidget()
{
}

osgViewer::Viewer* osgQOpenGLWidget::getOsgViewer()
{
    return m_renderer;
}

OpenThreads::ReadWriteMutex* osgQOpenGLWidget::mutex()
{
    return &_osgMutex;
}


void osgQOpenGLWidget::initializeGL()
{
    // Initializes OpenGL function resolution for the current context.
    initializeOpenGLFunctions();
    createRenderer();
    emit initialized();
}

void osgQOpenGLWidget::resizeGL(int w, int h)
{
    Q_ASSERT(m_renderer);
    QScreen* screen = windowHandle()
                      && windowHandle()->screen() ? windowHandle()->screen() :
                      qApp->screens().front();
    m_renderer->resize(w, h, screen->devicePixelRatio());
}

void osgQOpenGLWidget::paintGL()
{
    OpenThreads::ScopedReadLock locker(_osgMutex);
	if (_isFirstFrame) {
		_isFirstFrame = false;
		m_renderer->getCamera()->getGraphicsContext()->setDefaultFboId(defaultFramebufferObject());
	}
	m_renderer->frame();
}

void osgQOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    Q_ASSERT(m_renderer);

    // forward event to renderer
    m_renderer->keyPressEvent(event);
}

void osgQOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->keyReleaseEvent(event);
}

void osgQOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mousePressEvent(event);
}

void osgQOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mouseReleaseEvent(event);
}

void osgQOpenGLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mouseDoubleClickEvent(event);
}

void osgQOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mouseMoveEvent(event);
}

void osgQOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->wheelEvent(event);
}

void osgQOpenGLWidget::setDefaultDisplaySettings()
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    ds->setNvOptimusEnablement(1);
    ds->setStereo(false);
}

void osgQOpenGLWidget::createRenderer()
{
    // call this before creating a View...
    setDefaultDisplaySettings();
	if (!_arguments) {
		m_renderer = new OSGRenderer(this);
	} else {
		m_renderer = new OSGRenderer(_arguments, this);
	}
	QScreen* screen = windowHandle()
                      && windowHandle()->screen() ? windowHandle()->screen() :
                      qApp->screens().front();
    m_renderer->setupOSG(width(), height(), screen->devicePixelRatio());
}
