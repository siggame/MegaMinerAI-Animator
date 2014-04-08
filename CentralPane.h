#ifndef CENTRALPANE_H
#define CENTRALPANE_H

#include <QVBoxLayout>
#include "PlotWidget.h"
#include "TimeLine.h"
#include "AnimationWidget.h"
#include "common.h"

class CentralPane :
    public QWidget
{
    Q_OBJECT

public:
    CentralPane(std::vector<std::pair<Point, vanim::comp> >& points,
                QWidget * parent = 0);

    QSize minimumSizeHint() const;

    QSize sizeHint() const;

    void resizeEvent(QResizeEvent *);

signals:
    void SelectionChanged();

    void setSelection(int);

    void editModeChanged(EDITOR_MODE);

public slots:
    void setEditMode(EDITOR_MODE mode)
    {
        m_PlotWindow->setEditMode(mode);
    }

    void setKeyFrameEditMode(KEYFRAME_MODE mode)
    {
        m_PlotWindow->setKeyFrameMode(mode);
        m_Timeline->setKeyFrameMode(mode);
    }

    void play(float d)
    {
        m_Timeline->play(d);
    }

private slots:
    void propSelectChanged()
    {
        emit SelectionChanged();
    }

    void propSetSelection(int index)
    {
        emit setSelection(index);
    }

    void propEditModeChanged(EDITOR_MODE mode)
    {
        emit propEditModeChanged(mode);
    }

private:
    QVBoxLayout * m_Layout;
    PlotWindow * m_PlotWindow;
    PlotWidget * m_PlotWidget;
    TimeLine * m_Timeline;

    std::vector<std::pair<Point, vanim::comp> >& m_Points;
    int m_Selection;
};

#endif // CENTRALPANE_H
