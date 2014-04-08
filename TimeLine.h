#ifndef TIMELINE_H
#define TIMELINE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTextStream>
#include <QTimer>
#include <QElapsedTimer>
#include "PlotWidget.h"
#include "AnimationWidget.h"
#include "common.h"

class TimeLine :
    public QWidget
{
    Q_OBJECT

public:
    TimeLine(std::vector<std::pair<Point, vanim::comp> >&,QWidget* parent = 0);
    ~TimeLine() {}

    void paintEvent(QPaintEvent *);

    void mouseMoveEvent(QMouseEvent *);

    void mousePressEvent(QMouseEvent * event);

    void mouseReleaseEvent(QMouseEvent *);

    void keyPressEvent(QKeyEvent *);

    void resizeEvent(QResizeEvent * event);

    QSize sizeHint() const;

    float getTime() const;

signals:
    void timeChanged(float);

    void setKeyframeSelection(int);

public slots:
    void setSelection(int index)
    {
        m_Selection = index;
        m_KeyframeSelection = -1;
        repaint();
    }

    void updateSelection()
    {
        repaint();
    }

    void setKeyFrameMode(KEYFRAME_MODE mode)
    {
        m_CurrentKeyframeMode = mode;
        repaint();
    }

    void setKeyframeSelect(int)
    {
        m_KeyframeSelection = -1;
        emit setKeyframeSelection(-1);
    }

    void play(float secs);

    void stop();

    void reset();

private slots:
    void updateTime();

private:
    KEYFRAME_MODE m_CurrentKeyframeMode;
    std::vector<std::pair<Point, vanim::comp> > & m_Points;
    int m_Selection;
    int m_KeyframeSelection;
    bool m_KeyframePressed;
    QRect xSelect;
    QColor m_LineColor;
    QTimer m_Refresher;
    QTimer m_Player;
    QElapsedTimer m_ElapsedTime;
    float m_Duration;
};

#endif // TIMELINE_H
