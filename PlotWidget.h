#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWindow>
#include <QWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_0>
#include <QTimer>
#include <QGLWidget>
#include <QResizeEvent>
#include <QDebug>
#include <cmath>
#include <iostream>
#include "vanim.h"
#include "common.h"

class PlotWidget :
    public QWidget
{
    Q_OBJECT

public:
    PlotWidget(QWidget * plot, QWidget * parent = 0);

    void resizeEvent(QResizeEvent * event);

    QSize sizeHint() const {return QSize(600, 600);}

    QSize minimumSizeHint() const {return QSize(600, 600);}

private:
    QWidget * m_Plot;
};

class PlotWindow :
    public QWindow
{
    Q_OBJECT

    static const int handleDistP = 40;

public:

    struct Handle
    {
        float x, y;
        QColor color;
        bool isPressed;
    };

public:
    PlotWindow(std::vector<std::pair<Point, vanim::comp> >& vec, QWindow * parent = 0);

    ~PlotWindow();



public slots:
    void draw();

    void setTime(float);

    void setEditMode(EDITOR_MODE mode);

    void setKeyFrameMode(KEYFRAME_MODE mode)
    {
        m_KeyframeMode = mode;
    }

    void setKeyframeSelection(int k)
    {
        m_KeyframeSelection = k;
    }

signals:
    void selectionChanged();

    void setSelection(int);

    void propKeyframeSelection(int k);

private:
    void mousePressEvent(QMouseEvent *);

    void mouseDoubleClickEvent(QMouseEvent *);

    void mouseMoveEvent(QMouseEvent*);

    void mouseReleaseEvent(QMouseEvent *);

    void keyPressEvent(QKeyEvent*);

    void resizeEvent(QResizeEvent*);

    void updateHandles();

private:
    float m_CurrentTime;
    std::vector<std::pair<Point, vanim::comp> > & m_Points;
    int m_Selection;

    Handle m_Rot;
    QPoint m_PosBefore;
    Handle m_ScaleX;
    Handle m_ScaleY;
    Handle m_Offset;

    bool m_SelectionPressed;
    QOpenGLContext * m_Context;
    QOpenGLFunctions_3_0 * gl;
    QTimer * m_Timer;
    uint m_PointTexture;
    EDITOR_MODE m_EditMode;
    KEYFRAME_MODE m_KeyframeMode;
    int m_KeyframeSelection;
};

#endif // PLOTWIDGET_H
