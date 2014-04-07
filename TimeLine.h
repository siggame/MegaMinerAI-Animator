#ifndef TIMELINE_H
#define TIMELINE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTextStream>
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

    void resizeEvent(QResizeEvent * event);

    QSize sizeHint() const;

    float getTime() const;

signals:
    void timeChanged(float);

public slots:
    void setSelection(int index)
    {
        m_Selection = index;
    }

    void updateSelection()
    {
        repaint();
    }

    void setKeyFrameMode(AnimationWidget::KEYFRAME_MODE mode)
    {
        m_CurrentKeyframeMode = mode;
        repaint();
    }

private:
    AnimationWidget::KEYFRAME_MODE m_CurrentKeyframeMode;
    std::vector<std::pair<Point, vanim::comp> > & m_Points;
    int m_Selection;
    QRect xSelect;
    QColor m_LineColor;
};

#endif // TIMELINE_H
