#include "RightPane.h"

RightPane::RightPane(std::vector<std::pair<Point, vanim::comp> >& points,
                     QWidget *parent) :
    QWidget(parent),
    m_Points(points)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    setMaximumWidth(300);

    m_Layout = new QGridLayout;
    m_ButtonLayout = new QHBoxLayout;

    m_CompEditMode = new QPushButton("Component Mode", this);
    m_AnimEditMode = new QPushButton("Animation Mode", this);

    m_SelectWidget = new SelectionForm(m_Points, this);
    m_AnimWidget = new AnimationWidget(m_Points, this);

    m_ButtonLayout->addWidget(m_CompEditMode);
    m_ButtonLayout->addWidget(m_AnimEditMode);

    m_Layout->addLayout(m_ButtonLayout, 0, 0);
    m_Layout->addWidget(m_SelectWidget, 1, 0);
    m_Layout->addWidget(m_AnimWidget, 2, 0);

    connect(this, SIGNAL(selectionChanged()),
            m_SelectWidget, SLOT(updateSelection()));

    connect(m_CompEditMode, SIGNAL(pressed()),
            this, SLOT(setComponentMode()));

    connect(m_AnimEditMode, SIGNAL(pressed()),
            this, SLOT(setAnimationMode()));

    connect(m_AnimWidget, SIGNAL(keyframeModeChanged(KEYFRAME_MODE)),
            this, SLOT(propKeyframeModeChange(KEYFRAME_MODE)));

    connect(m_AnimWidget, SIGNAL(play(float)),
            this, SLOT(propPlay(float)));

    m_AnimWidget->hide();

    setLayout(m_Layout);
}
