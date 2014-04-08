#ifndef ANIMATIONWIDGET_H
#define ANIMATIONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include "vanim.h"
#include "PlotWidget.h"
#include "common.h"

class AnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationWidget(std::vector<std::pair<Point, vanim::comp> > &, QWidget *parent = 0);


signals:
    void keyframeModeChanged(KEYFRAME_MODE);

    void play(float secs);

public slots:
    void setSelection(int select)
    {
        m_Selection = select;
    }

private slots:
    void setTransKeyMode()
    {
        m_KeyframeMode = TRANS_KEYS;
        emit keyframeModeChanged(m_KeyframeMode);
    }

    void setRotateKeyMode()
    {
        m_KeyframeMode = ROTATE_KEYS;
        emit keyframeModeChanged(m_KeyframeMode);
    }

    void setScaleKeyMode()
    {
        m_KeyframeMode = SCALE_KEYS;
        emit keyframeModeChanged(m_KeyframeMode);
    }

    void setColorKeyMode()
    {
        m_KeyframeMode = COLOR_KEYS;
        emit keyframeModeChanged(m_KeyframeMode);
    }

    void propPlay()
    {
        QString str;
        QTextStream stream(&str);
        stream << m_Duration->text();
        emit play(str.toFloat());
    }

public:
    KEYFRAME_MODE m_KeyframeMode;
    std::vector<std::pair<Point, vanim::comp> > & m_Points;
    int m_Selection;
    QGridLayout * m_Layout;
    QHBoxLayout * m_ModesLayout;
    QHBoxLayout * m_TimeSet;
    QHBoxLayout * m_ActionsLayout;

    QPushButton * m_TranslateMode;
    QPushButton * m_RotateMode;
    QPushButton * m_ScaleMode;
    QPushButton * m_ColorMode;

    QLabel * m_DurLabel;
    QLineEdit * m_Duration;

    QPushButton * m_Begin;
    QPushButton * m_Play;
    QPushButton * m_Stop;

    QTableWidget * m_Keyframes;
};

#endif // ANIMATIONWIDGET_H
