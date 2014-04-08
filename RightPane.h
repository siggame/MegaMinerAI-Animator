#ifndef RIGHTPANE_H
#define RIGHTPANE_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <PlotWidget.h>
#include "SelectionWidget.h"
#include "AnimationWidget.h"

class RightPane : public QWidget
{
    Q_OBJECT
public:
    explicit RightPane(std::vector<std::pair<Point, vanim::comp> >&,QWidget *parent = 0);

    QSize sizeHint() const {return QSize(300, 700);}

    QSize minimumSizeHint() const {return sizeHint();}

signals:
    void selectionChanged();

    void editModeChanged(EDITOR_MODE);

    void keyframeModeChanged(KEYFRAME_MODE);

    void play(float);

public slots:
    void propSelectionChange()
    {
        emit selectionChanged();
    }

    void propKeyframeModeChange(KEYFRAME_MODE mode)
    {
        emit keyframeModeChanged(mode);
    }

    void updateSelection()
    {
        m_SelectWidget->updateSelection();
    }

    void setSelection(int select)
    {
        m_Selection = select;
        m_SelectWidget->setSelection(select);
        m_AnimWidget->setSelection(select);
    }

    void setComponentMode()
    {
        m_AnimWidget->hide();
        emit editModeChanged(COMPONENT_EDITOR);
    }

    void setAnimationMode()
    {
        m_AnimWidget->show();
        emit editModeChanged(ANIM_EDITOR);
    }

private slots:
    void propPlay(float duration)
    {
        emit play(duration);
    }

private:
    std::vector<std::pair<Point, vanim::comp> >& m_Points;
    int m_Selection;

    QGridLayout * m_Layout;
    QHBoxLayout * m_ButtonLayout;

    QPushButton * m_CompEditMode;
    QPushButton * m_AnimEditMode;

    SelectionForm * m_SelectWidget;
    AnimationWidget* m_AnimWidget;
};

#endif // RIGHTPANE_H
