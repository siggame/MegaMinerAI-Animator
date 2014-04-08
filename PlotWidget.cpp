#include "PlotWidget.h"

const float PI = 3.141592f;

PlotWidget::PlotWidget(QWidget *plot, QWidget *parent) :
    QWidget(parent),
    m_Plot(plot)
{
    plot->setParent(this);
    plot->setMouseTracking(false);
    QSizePolicy pol;
    pol.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    pol.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    setSizePolicy(pol);
}

void PlotWidget::resizeEvent(QResizeEvent *)
{
    if(width() > height())
        resize(height(), height());
    else
        resize(width(), width());

    m_Plot->resize(width(), height());
}


PlotWindow::PlotWindow(std::vector<std::pair<Point, vanim::comp> >& vec, QWindow *parent) :
    QWindow(parent),
    m_Points(vec),
    m_Selection(-1),
    m_EditMode(COMPONENT_EDITOR),
    m_KeyframeSelection(-1)
{
    this->setSurfaceType(OpenGLSurface);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setMajorVersion(3);
    format.setMinorVersion(0);
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    this->setFormat(format);
    this->create();

    m_Context = new QOpenGLContext;
    m_Context->setFormat(format);
    m_Context->create();

    m_Context->makeCurrent(this);

    gl = m_Context->versionFunctions<QOpenGLFunctions_3_0>();
    if(!gl)
    {
        exit(1);
    }
    gl->initializeOpenGLFunctions();

    gl->glShadeModel(GL_SMOOTH);
    gl->glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    m_Timer = new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(draw()));
    m_Timer->start(16);

    QImage img;
    if(!img.load("point.png"))
        exit(1);

    QImage gltex = QGLWidget::convertToGLFormat(img);

    if(gltex.isNull())
        exit(1);

    gl->glGenTextures(1, &m_PointTexture);
    gl->glBindTexture(GL_TEXTURE_2D, m_PointTexture);

    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gltex.width(), gltex.height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, gltex.bits());

    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

PlotWindow::~PlotWindow()
{}

void PlotWindow::draw()
{
    const int pointWidthP = 3;
    const int selectWidthP = 4;
    float pointWidth = pointWidthP / static_cast<float>(width());
    float selectWidth = selectWidthP/ static_cast<float>(width());

    const int handleWidthP = 5;
    float handleWidth = handleWidthP / static_cast<float>(width());

    QColor handleColors[4];

    handleColors[0].setRgbF(1.0, 0.0, 0.0);
    handleColors[1].setRgbF(0.0, 0.0, 1.0);
    handleColors[2].setRgbF(0.0, 1.0, 0.0);
    handleColors[3].setRgbF(1.0, 1.0, 0.0);

    m_Context->makeCurrent(this);

    gl->glMatrixMode(GL_PROJECTION);
    gl->glLoadIdentity();
    gl->glOrtho(0.0, 1.0, 1.0, 0.0, 0.0, 1.0);
    gl->glMatrixMode(GL_MODELVIEW);
    gl->glDisable(GL_DEPTH_TEST);

    gl->glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);


    gl->glColor4f(1.0, 1.0f, 1.0f, 1.0f);
    float s;
    float c;
    float px;
    float py;
    float cx;
    float cy;

    for(uint i = 0; i < m_Points.size(); i++)
    {
        s = sin(m_Points[i].first.rot);
        c = cos(m_Points[i].first.rot);
        cx = m_Points[i].first.x;
        cy = m_Points[i].first.y;

        gl->glBegin(GL_QUADS);
            gl->glTexCoord2f(m_Points[i].first.texSX, m_Points[i].first.texSY);
            px = (m_Points[i].first.x - m_Points[i].first.width) - cx;
            py = (m_Points[i].first.y - m_Points[i].first.length) - cy;
            gl->glVertex2f(((px * c) - (py * s)) + cx, ((px * s) + (py * c)) + cy);

            gl->glTexCoord2f(m_Points[i].first.texEX, m_Points[i].first.texSY);
            px = (m_Points[i].first.x + m_Points[i].first.width) - cx;
            py = (m_Points[i].first.y - m_Points[i].first.length) - cy;
            gl->glVertex2f(((px * c) - (py * s)) + cx, ((px * s) + (py * c)) + cy);

            gl->glTexCoord2f(m_Points[i].first.texEX, m_Points[i].first.texEY);
            px = (m_Points[i].first.x + m_Points[i].first.width) - cx;
            py = (m_Points[i].first.y + m_Points[i].first.length) - cy;
            gl->glVertex2f(((px * c) - (py * s)) + cx, ((px * s) + (py * c)) + cy);

            gl->glTexCoord2f(m_Points[i].first.texSX, m_Points[i].first.texEY);
            px = (m_Points[i].first.x - m_Points[i].first.width) - cx;
            py = (m_Points[i].first.y + m_Points[i].first.length) - cy;
            gl->glVertex2f(((px * c) - (py * s)) + cx, ((px * s) + (py * c)) + cy);
        gl->glEnd();
    }

    gl->glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
    if(m_Selection != -1)
    {
        gl->glColor4f(m_Rot.color.redF(), m_Rot.color.greenF(),
                      m_Rot.color.blueF(), m_Rot.color.alphaF());
        gl->glBegin(GL_QUADS);
                gl->glTexCoord2f(0.0f, 0.0f);
                gl->glVertex2f(m_Rot.x - handleWidth, m_Rot.y - handleWidth);
                gl->glTexCoord2f(1.0f, 0.0f);
                gl->glVertex2f(m_Rot.x + handleWidth, m_Rot.y - handleWidth);
                gl->glTexCoord2f(1.0f, 1.0f);
                gl->glVertex2f(m_Rot.x + handleWidth, m_Rot.y + handleWidth);
                gl->glTexCoord2f(0.0f, 1.0f);
                gl->glVertex2f(m_Rot.x - handleWidth, m_Rot.y + handleWidth);
        gl->glEnd();

        gl->glColor4f(m_ScaleX.color.redF(), m_ScaleX.color.greenF(),
                      m_ScaleX.color.blueF(), m_ScaleX.color.alphaF());
        gl->glBegin(GL_QUADS);
                gl->glTexCoord2f(0.0f, 0.0f);
                gl->glVertex2f(m_ScaleX.x - handleWidth, m_ScaleX.y - handleWidth);
                gl->glTexCoord2f(1.0f, 0.0f);
                gl->glVertex2f(m_ScaleX.x + handleWidth, m_ScaleX.y - handleWidth);
                gl->glTexCoord2f(1.0f, 1.0f);
                gl->glVertex2f(m_ScaleX.x + handleWidth, m_ScaleX.y + handleWidth);
                gl->glTexCoord2f(0.0f, 1.0f);
                gl->glVertex2f(m_ScaleX.x - handleWidth, m_ScaleX.y + handleWidth);
        gl->glEnd();

        gl->glColor4f(m_ScaleY.color.redF(), m_ScaleY.color.greenF(),
                      m_ScaleY.color.blueF(), m_ScaleY.color.alphaF());
        gl->glBegin(GL_QUADS);
                gl->glTexCoord2f(0.0f, 0.0f);
                gl->glVertex2f(m_ScaleY.x - handleWidth, m_ScaleY.y - handleWidth);
                gl->glTexCoord2f(1.0f, 0.0f);
                gl->glVertex2f(m_ScaleY.x + handleWidth, m_ScaleY.y - handleWidth);
                gl->glTexCoord2f(1.0f, 1.0f);
                gl->glVertex2f(m_ScaleY.x + handleWidth, m_ScaleY.y + handleWidth);
                gl->glTexCoord2f(0.0f, 1.0f);
                gl->glVertex2f(m_ScaleY.x - handleWidth, m_ScaleY.y + handleWidth);
        gl->glEnd();

        if(m_EditMode == COMPONENT_EDITOR)
        {
            gl->glColor4f(m_Offset.color.redF(), m_Offset.color.greenF(),
                          m_Offset.color.blueF(), m_Offset.color.alphaF());
            gl->glBegin(GL_QUADS);
                    gl->glTexCoord2f(0.0f, 0.0f);
                    gl->glVertex2f(m_Offset.x - handleWidth, m_Offset.y - handleWidth);
                    gl->glTexCoord2f(1.0f, 0.0f);
                    gl->glVertex2f(m_Offset.x + handleWidth, m_Offset.y - handleWidth);
                    gl->glTexCoord2f(1.0f, 1.0f);
                    gl->glVertex2f(m_Offset.x + handleWidth, m_Offset.y + handleWidth);
                    gl->glTexCoord2f(0.0f, 1.0f);
                    gl->glVertex2f(m_Offset.x - handleWidth, m_Offset.y + handleWidth);
            gl->glEnd();
        }

        gl->glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        gl->glBegin(GL_QUADS);
            gl->glTexCoord2f(0.0f, 0.0f);
            gl->glVertex2f(m_Points[m_Selection].first.x - selectWidth, m_Points[m_Selection].first.y - selectWidth);
            gl->glTexCoord2f(1.0f, 0.0f);
            gl->glVertex2f(m_Points[m_Selection].first.x + selectWidth, m_Points[m_Selection].first.y - selectWidth);
            gl->glTexCoord2f(1.0f, 1.0f);
            gl->glVertex2f(m_Points[m_Selection].first.x + selectWidth, m_Points[m_Selection].first.y + selectWidth);
            gl->glTexCoord2f(0.0f, 1.0f);
            gl->glVertex2f(m_Points[m_Selection].first.x - selectWidth, m_Points[m_Selection].first.y + selectWidth);
        gl->glEnd();
    }

    gl->glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    for(uint i = 0; i < m_Points.size(); i++)
    {
        gl->glBegin(GL_QUADS);
            gl->glTexCoord2f(0.0f, 0.0f);
            gl->glVertex2f(m_Points[i].first.x - pointWidth, m_Points[i].first.y - pointWidth);
            gl->glTexCoord2f(1.0f, 0.0f);
            gl->glVertex2f(m_Points[i].first.x + pointWidth, m_Points[i].first.y - pointWidth);
            gl->glTexCoord2f(1.0f, 1.0f);
            gl->glVertex2f(m_Points[i].first.x + pointWidth, m_Points[i].first.y + pointWidth);
            gl->glTexCoord2f(0.0f, 1.0f);
            gl->glVertex2f(m_Points[i].first.x - pointWidth, m_Points[i].first.y + pointWidth);
        gl->glEnd();
    }

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    m_Context->swapBuffers(this);
}

void PlotWindow::resizeEvent(QResizeEvent* event)
{
    QSize newSize = event->size();
    gl->glViewport(0, 0, newSize.width(), newSize.height());
}

void PlotWindow::mouseDoubleClickEvent(QMouseEvent * event)
{
    Point add;
    vanim::comp newComp;

    if(m_EditMode == COMPONENT_EDITOR)
    {
        add.x = static_cast<float>(event->x()) / this->width();
        add.y = static_cast<float>(event->y()) / this->height();
        add.texSX = 0;
        add.texSY = 0;
        add.texEX = 0;
        add.texEY = 0;
        add.rot = 0;
        add.length = 0;
        add.width = 0;
        add.offx = 0;
        add.offy = 0;

        newComp.x = add.x;
        newComp.y = add.y;
        newComp.texSX = add.texSX;
        newComp.texSY = add.texSY;
        newComp.texEX = add.texEX;
        newComp.texEY = add.texEY;
        newComp.rot = add.rot;
        newComp.length = add.length;
        newComp.width = add.width;
        newComp.offx = add.offx;
        newComp.offy = add.offy;

        m_Points.push_back({add, newComp});
    }
}

void PlotWindow::mousePressEvent(QMouseEvent * event)
{
    const int pointWidthP = 3;
    float pointWidth = pointWidthP / static_cast<float>(width());

    float handleDist = handleDistP / static_cast<float>(width());
    const int handleWidthP = 5;
    float handleWidth = handleWidthP / static_cast<float>(width());

    m_PosBefore = event->pos();

    for(uint i = 0; i < m_Points.size(); i++)
    {
        float d = sqrt(pow((static_cast<float>(event->x())/ this->width()) - m_Points[i].first.x, 2) +
                       pow((static_cast<float>(event->y())/ this->height()) - m_Points[i].first.y, 2));

        float dRotHandle = sqrt(pow((static_cast<float>(event->x())/ this->width()) - m_Rot.x, 2) +
                                pow((static_cast<float>(event->y())/ this->height()) - m_Rot.y, 2));

        float dScaleXHandle = sqrt(pow((static_cast<float>(event->x())/ this->width()) - m_ScaleX.x, 2) +
                                   pow((static_cast<float>(event->y())/ this->height()) - m_ScaleX.y, 2));

        float dScaleYHandle = sqrt(pow((static_cast<float>(event->x())/ this->width()) - m_ScaleY.x, 2) +
                                   pow((static_cast<float>(event->y())/ this->height()) - m_ScaleY.y, 2));

        float dOffsetHandle = sqrt(pow((static_cast<float>(event->x())/ this->width()) - m_Offset.x, 2) +
                                   pow((static_cast<float>(event->y())/ this->height()) - m_Offset.y, 2));


        if(dRotHandle < handleWidth)
        {
            m_Rot.isPressed = true;
            m_Rot.color.setRgbF(0.8, 0.0f, 0.0f);
            return;
        }
        else if(dScaleXHandle < handleWidth)
        {
            m_ScaleX.isPressed = true;
            m_ScaleX.color.setRgbF(0.0f, 0.0f, 0.8f);
            return;
        }
        else if(dScaleYHandle < handleWidth)
        {
            m_ScaleY.isPressed = true;
            m_ScaleY.color.setRgbF(0.0f, 0.8f, 0.0f);
            return;
        }
        else if(dOffsetHandle < handleWidth)
        {
            if(m_EditMode == COMPONENT_EDITOR)
            {
                m_Offset.isPressed = true;
                m_Offset.color.setRgbF(0.8f, 0.8f, 0.0f);
                return;
            }
        }

        if(d < pointWidth)
        {
            m_Selection = i;
            m_SelectionPressed = true;

            m_Rot.isPressed = false;
            m_Rot.color.setRgbF(1.0f, 0.0f, 0.0f);
            m_Rot.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot + 3*PI/2);
            m_Rot.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot + 3*PI/2);

            m_ScaleX.isPressed = false;
            m_ScaleX.color.setRgbF(0.0f, 0.0f, 1.0f);
            m_ScaleX.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  0);
            m_ScaleX.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  0);

            m_ScaleY.isPressed = false;
            m_ScaleY.color.setRgbF(0.0f, 1.0f, 0.0f);
            m_ScaleY.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  PI/2);
            m_ScaleY.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  PI/2);

            if(m_EditMode == COMPONENT_EDITOR)
            {
                m_Offset.isPressed = false;
                m_Offset.color.setRgbF(1.0f, 1.0f, 0.0f);
                m_Offset.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  PI);
                m_Offset.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  PI);
            }

            emit setSelection(m_Selection);
            return;
        }
    }

    m_Selection = -1;
    m_SelectionPressed = false;

    emit setSelection(-1);
}

void PlotWindow::mouseMoveEvent(QMouseEvent * event)
{
    float handleDist = handleDistP / static_cast<float>(width());

    if(m_Selection != -1)
    {
        if(event->x() > 0 && event->x() < width() &&
           event->y() > 0 && event->y() < height() && event->buttons() == Qt::LeftButton)
        {
            if(m_SelectionPressed)
            {
                m_Points[m_Selection].first.x = event->x() /static_cast<float>(width());
                m_Points[m_Selection].first.y = event->y() /static_cast<float>(height());

                if(m_EditMode == COMPONENT_EDITOR)
                {
                    m_Points[m_Selection].second.x = m_Points[m_Selection].first.x;
                    m_Points[m_Selection].second.y = m_Points[m_Selection].first.y;
                }

                if(m_KeyframeMode == TRANS_KEYS &&
                   m_KeyframeSelection != -1)
                {
                    std::vector<vanim::comp::TranslateDelta>& transKeys = m_Points[m_Selection].second.m_TranslateDeltas;
                    transKeys[m_KeyframeSelection].x = m_Points[m_Selection].first.x;
                    transKeys[m_KeyframeSelection].y = m_Points[m_Selection].first.y;
                }

                updateHandles();
            }
            else if(m_Rot.isPressed)
            {
                m_Rot.x = event->x()/static_cast<float>(width());
                m_Rot.y = event->y()/static_cast<float>(height());

                if(m_Rot.x - m_Points[m_Selection].first.x > 0.0f && m_Rot.y - m_Points[m_Selection].first.y < 0.0f)
                    m_Points[m_Selection].first.rot = -atan((m_Rot.x - m_Points[m_Selection].first.x)/
                                                   (m_Rot.y - m_Points[m_Selection].first.y));
                else if(m_Rot.x - m_Points[m_Selection].first.x > 0.0f && m_Rot.y - m_Points[m_Selection].first.y > 0.0f)
                    m_Points[m_Selection].first.rot = -atan((m_Rot.x - m_Points[m_Selection].first.x)/
                                                   (m_Rot.y - m_Points[m_Selection].first.y)) + PI;
                else if(m_Rot.x - m_Points[m_Selection].first.x < 0.0f && m_Rot.y - m_Points[m_Selection].first.y > 0.0f)
                    m_Points[m_Selection].first.rot = -atan((m_Rot.x - m_Points[m_Selection].first.x)/
                                                   (m_Rot.y - m_Points[m_Selection].first.y)) + PI;
                else if(m_Rot.x - m_Points[m_Selection].first.x < 0.0f && m_Rot.y - m_Points[m_Selection].first.y < 0.0f)
                    m_Points[m_Selection].first.rot = -atan((m_Rot.x - m_Points[m_Selection].first.x)/
                                                   (m_Rot.y - m_Points[m_Selection].first.y)) + 2*PI;

                if(m_EditMode == COMPONENT_EDITOR)
                {
                    m_Points[m_Selection].second.rot = m_Points[m_Selection].first.rot;
                }

                updateHandles();
            }
            else if(m_ScaleX.isPressed)
            {
                float deltaX;
                float deltaY;
                float delta;

                if((m_Points[m_Selection].first.rot > 7*PI/4 || m_Points[m_Selection].first.rot < PI/4) ||
                   (m_Points[m_Selection].first.rot > 3*PI/4 && m_Points[m_Selection].first.rot < 5*PI/4))
                {
                    deltaX = (event->x() - m_PosBefore.x())/ static_cast<float>(width());
                    deltaY = ((event->x() - m_PosBefore.x())/ static_cast<float>(width())) * tan(m_Points[m_Selection].first.rot);
                }
                else
                {
                    deltaX = ((event->y() - m_PosBefore.y())/ static_cast<float>(height())) * 1/tan(m_Points[m_Selection].first.rot);
                    deltaY = (event->y() - m_PosBefore.y())/ static_cast<float>(height());
                }

                float distBefore = sqrt( pow(m_ScaleX.x - m_Points[m_Selection].first.x, 2) + pow(m_ScaleX.y - m_Points[m_Selection].first.y, 2));
                float distAfter = sqrt(pow((m_ScaleX.x + deltaX) - m_Points[m_Selection].first.x, 2) +
                                       pow((m_ScaleX.y + deltaY) - m_Points[m_Selection].first.y, 2));

                delta = sqrt(pow(deltaX,2) + pow(deltaY,2));

                if(distBefore < distAfter)
                    m_Points[m_Selection].first.width += delta;
                else
                    m_Points[m_Selection].first.width -= delta;

                if(m_Points[m_Selection].first.width < 0)
                    m_Points[m_Selection].first.width = 0;
                else
                {
                    m_ScaleX.x += deltaX;
                    m_ScaleX.y += deltaY;
                }

                m_Rot.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot + 3*PI/2);
                m_Rot.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot + 3*PI/2);

                m_ScaleY.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  PI/2);
                m_ScaleY.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  PI/2);

                if(m_EditMode == COMPONENT_EDITOR)
                {
                    m_Offset.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  PI);
                    m_Offset.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  PI);


                    m_Points[m_Selection].second.width = m_Points[m_Selection].first.width;
                }
            }
            else if(m_ScaleY.isPressed)
            {
                float deltaX;
                float deltaY;
                float delta;

                if((m_Points[m_Selection].first.rot > 7*PI/4 || m_Points[m_Selection].first.rot < PI/4) ||
                   (m_Points[m_Selection].first.rot > 3*PI/4 && m_Points[m_Selection].first.rot < 5*PI/4))
                {
                    deltaX = ((event->y() - m_PosBefore.y())/ static_cast<float>(height())) * 1/tan(m_Points[m_Selection].first.rot + PI/2);
                    deltaY = (event->y() - m_PosBefore.y())/ static_cast<float>(height());
                }
                else
                {
                    deltaX = (event->x() - m_PosBefore.x()) / static_cast<float>(width());
                    deltaY = ((event->x() - m_PosBefore.x())/ static_cast<float>(width())) * tan(m_Points[m_Selection].first.rot + PI/2);
                }

                float distBefore = sqrt( pow(m_ScaleY.x - m_Points[m_Selection].first.x, 2) + pow(m_ScaleY.y - m_Points[m_Selection].first.y, 2));
                float distAfter = sqrt(pow((m_ScaleY.x + deltaX) - m_Points[m_Selection].first.x, 2) +
                                       pow((m_ScaleY.y + deltaY) - m_Points[m_Selection].first.y, 2));

                delta = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

                if(distBefore < distAfter)
                    m_Points[m_Selection].first.length += delta;
                else
                    m_Points[m_Selection].first.length -= delta;

                if(m_Points[m_Selection].first.length < 0)
                    m_Points[m_Selection].first.length = 0;
                else
                {
                    m_ScaleY.x += deltaX;
                    m_ScaleY.y += deltaY;
                }

                m_Rot.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot + 3*PI/2);
                m_Rot.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot + 3*PI/2);

                m_ScaleX.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  0);
                m_ScaleX.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  0);

                if(m_EditMode == COMPONENT_EDITOR)
                {
                    m_Offset.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  PI);
                    m_Offset.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  PI);

                    m_Points[m_Selection].second.length = m_Points[m_Selection].first.length;
                }
            }
            else if(m_Offset.isPressed)
            {
                // to be implemented
            }

            m_PosBefore = event->pos();

            emit selectionChanged();
        }
    }
}

void PlotWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_Rot.isPressed = false;
    m_Rot.color.setRgbF(1.0f, 0.0f, 0.0f);
    m_ScaleX.isPressed = false;
    m_ScaleX.color.setRgbF(0.0f, 0.0f, 1.0f);
    m_ScaleY.isPressed = false;
    m_ScaleY.color.setRgbF(0.0f, 1.0f, 0.0f);
    m_Offset.isPressed = false;
    m_Offset.color.setRgbF(1.0f, 1.0f, 0.0f);
    m_SelectionPressed = false;

    updateHandles();
}

void PlotWindow::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Delete)
    {
        if(m_Selection != -1 && m_KeyframeSelection == -1)
        {
            auto it = m_Points.begin() + m_Selection;
            m_Points.erase(it);

            m_Selection = -1;
            emit setSelection(m_Selection);
        }

        updateHandles();
    }
    else if(event->key() == Qt::Key_Return)
    {
        if(m_EditMode == ANIM_EDITOR && m_Selection != -1)
        {
            std::vector<vanim::comp::TranslateDelta> & transDeltas =  m_Points[m_Selection].second.m_TranslateDeltas;
            if(transDeltas.empty())
            {
                if(m_CurrentTime > 0.0f && (m_Points[m_Selection].first.x != m_Points[m_Selection].second.x ||
                                            m_Points[m_Selection].first.y != m_Points[m_Selection].second.y))
                {
                    transDeltas.emplace_back(m_CurrentTime, m_Points[m_Selection].first.x, m_Points[m_Selection].first.y);
                }
            }
            else
            {
                for(auto it = transDeltas.begin(); it != transDeltas.end(); it++)
                {
                    auto next = std::next(it, 1);
                    if(m_CurrentTime < (*it).time)
                    {
                        transDeltas.insert(it, {m_CurrentTime, m_Points[m_Selection].first.x, m_Points[m_Selection].first.y});
                        break;
                    }

                    if(next == transDeltas.end() && m_CurrentTime > (*it).time)
                    {
                        transDeltas.emplace_back(m_CurrentTime, m_Points[m_Selection].first.x, m_Points[m_Selection].first.y);
                        break;
                    }
                }
            }

            std::vector<vanim::comp::RotateDelta> & rotateDeltas = m_Points[m_Selection].second.m_RotateDeltas;
            if(rotateDeltas.empty())
            {
                if(m_CurrentTime > 0.0f && (m_Points[m_Selection].first.rot != m_Points[m_Selection].second.rot))
                    rotateDeltas.emplace_back(m_CurrentTime, m_Points[m_Selection].first.rot);
            }
            else
            {
                for(auto it = rotateDeltas.begin(); it != rotateDeltas.end(); it++)
                {
                    auto next = std::next(it, 1);
                    if(m_CurrentTime < (*it).time)
                    {
                        if(m_Points[m_Selection].first.rot != (*it).rot)
                            rotateDeltas.insert(it, {m_CurrentTime, m_Points[m_Selection].first.rot});
                        break;
                    }

                    if(next == rotateDeltas.end() && m_CurrentTime > (*it).time)
                    {
                        rotateDeltas.emplace_back(m_CurrentTime, m_Points[m_Selection].first.rot);
                        break;
                    }
                }
            }

            std::vector<vanim::comp::ScaleDelta> & scaleDeltas = m_Points[m_Selection].second.m_ScaleDeltas;
            if(scaleDeltas.empty())
            {
                if(m_CurrentTime > 0.0f && (m_Points[m_Selection].first.length != m_Points[m_Selection].second.length ||
                                            m_Points[m_Selection].first.width != m_Points[m_Selection].second.width))
                    scaleDeltas.emplace_back(m_CurrentTime, m_Points[m_Selection].first.width, m_Points[m_Selection].first.length);
            }
            else
            {
                for(auto it = scaleDeltas.begin(); it != scaleDeltas.end(); it++)
                {
                    auto next = std::next(it, 1);
                    if(m_CurrentTime < (*it).time)
                    {
                        if(m_Points[m_Selection].first.length != (*it).length || m_Points[m_Selection].first.width != (*it).width)
                            scaleDeltas.insert(it, {m_CurrentTime, m_Points[m_Selection].first.width, m_Points[m_Selection].first.length});
                        break;
                    }

                    if(next == scaleDeltas.end() && m_CurrentTime > (*it).time)
                    {
                        scaleDeltas.emplace_back(m_CurrentTime, m_Points[m_Selection].first.width, m_Points[m_Selection].first.length);
                        break;
                    }
                }
            }

            emit selectionChanged();
        }
    }\

    QWindow::keyPressEvent(event);
}

void PlotWindow::setTime(float time)
{
    m_CurrentTime = time;
    float handleDist = handleDistP / static_cast<float>(width());
    Q_UNUSED(handleDist);

    if(m_EditMode == ANIM_EDITOR)
    {
        for(uint i = 0; i < m_Points.size(); i++)
        {
            float x, y;
            std::vector<vanim::comp::TranslateDelta>& transDeltas = m_Points[i].second.m_TranslateDeltas;
            if(!transDeltas.empty())
            {
                if(m_CurrentTime < transDeltas[0].time)
                {
                    x = ((transDeltas[0].x - m_Points[i].first.x) * ((m_CurrentTime - 0.0f) / (transDeltas[0].time - 0.0f))) + m_Points[i].first.x;
                    y = ((transDeltas[0].y - m_Points[i].first.y) * ((m_CurrentTime - 0.0f) / (transDeltas[0].time - 0.0f))) + m_Points[i].first.y;
                }
                else
                {
                    for(auto it = transDeltas.begin(); it != transDeltas.end(); it++)
                    {
                        auto next = std::next(it, 1);

                        if(next == transDeltas.end())
                        {
                            x = (*it).x;
                            y = (*it).y;
                            break;
                        }

                        if(m_CurrentTime >= (*it).time && m_CurrentTime < (*next).time)
                        {
                            x = (((*next).x - (*it).x) * ((m_CurrentTime - (*it).time)/ ((*next).time - (*it).time))) + (*it).x;
                            y = (((*next).y - (*it).y) * ((m_CurrentTime - (*it).time)/ ((*next).time - (*it).time))) + (*it).y;
                            break;
                        }
                    }
                }
                m_Points[i].first.x = x;
                m_Points[i].first.y = y;
            }

        }

        updateHandles();
        emit selectionChanged();
    }

}

void PlotWindow::setEditMode(EDITOR_MODE mode)
{
    m_EditMode = mode;

    if(mode == ANIM_EDITOR)
    {
        for(uint i = 0; i < m_Points.size(); i++)
        {
            m_Points[i].first.x = m_Points[i].second.x;
            m_Points[i].first.y = m_Points[i].second.y;
            m_Points[i].first.rot = m_Points[i].second.rot;
            m_Points[i].first.length = m_Points[i].second.length;
            m_Points[i].first.width = m_Points[i].second.width;
            m_Points[i].first.texSX = m_Points[i].second.texSX;
            m_Points[i].first.texSY = m_Points[i].second.texSY;
            m_Points[i].first.texEX = m_Points[i].second.texEX;
            m_Points[i].first.texEY = m_Points[i].second.texEY;


        }

        emit selectionChanged();
        updateHandles();
    }
}

void PlotWindow::updateHandles()
{
    float handleDist = handleDistP / static_cast<float>(width());

    if(m_Selection != -1)
    {
        m_Rot.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot + 3*PI/2);
        m_Rot.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot + 3*PI/2);

        m_ScaleX.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  0);
        m_ScaleX.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  0);

        m_ScaleY.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  PI/2);
        m_ScaleY.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  PI/2);

        if(m_EditMode == COMPONENT_EDITOR)
        {
            m_Offset.x = m_Points[m_Selection].first.x + handleDist * cos(m_Points[m_Selection].first.rot +  PI);
            m_Offset.y = m_Points[m_Selection].first.y + handleDist * sin(m_Points[m_Selection].first.rot +  PI);
        }
    }
    else
    {
        m_Rot.isPressed = false;
        m_Rot.x = 0;
        m_Rot.y = 0;

        m_ScaleX.isPressed = false;
        m_ScaleX.x = 0;
        m_ScaleX.y = 0;

        m_ScaleY.isPressed = false;
        m_ScaleY.x = 0;
        m_ScaleY.y = 0;

        m_Offset.isPressed = false;
        m_Offset.x = 0;
        m_Offset.y = 0;
    }
}
