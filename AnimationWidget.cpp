#include "AnimationWidget.h"

AnimationWidget::AnimationWidget(std::vector<std::pair<Point, vanim::comp> > & points,
                                 QWidget *parent) :
    QWidget(parent),
    m_Points(points),
    m_Selection(-1)
{
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QIcon translateIco("translate.png");
    QIcon rotateIco("rotate.png");
    QIcon scaleIco("scale.png");
    QIcon colorIco("color.png");

    m_Layout = new QGridLayout(this);
    m_ModesLayout = new QHBoxLayout(this);
    m_TimeSet = new QHBoxLayout(this);
    m_ActionsLayout = new QHBoxLayout(this);

    m_TranslateMode = new QPushButton(this);
    m_TranslateMode->setIcon(translateIco);
    m_TranslateMode->setToolTip("Translation Keys");
    m_RotateMode = new QPushButton(this);
    m_RotateMode->setIcon(rotateIco);
    m_RotateMode->setToolTip("Rotation Keys");
    m_ScaleMode = new QPushButton(this);
    m_ScaleMode->setIcon(scaleIco);
    m_ScaleMode->setToolTip("Scaling Keys");
    m_ColorMode = new QPushButton(this);
    m_ColorMode->setIcon(colorIco);
    m_ColorMode->setToolTip("Color Change Keys");

    m_Keyframes = new QTableWidget(0,1, this);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    m_Keyframes->setSizePolicy(sizePolicy);

    m_DurLabel = new QLabel("Duration (s):", this);
    m_Duration = new QLineEdit(this);

    m_Begin = new QPushButton("<", this);
    m_Play = new QPushButton("Play", this);
    m_Stop = new QPushButton("Stop", this);

    m_ModesLayout->addWidget(m_TranslateMode);
    m_ModesLayout->addWidget(m_RotateMode);
    m_ModesLayout->addWidget(m_ScaleMode);
    m_ModesLayout->addWidget(m_ColorMode);

    m_TimeSet->addWidget(m_DurLabel);
    m_TimeSet->addWidget(m_Duration);
    m_Duration->resize(40, m_Duration->height());

    m_ActionsLayout->addWidget(m_Begin);
    m_ActionsLayout->addWidget(m_Play);
    m_ActionsLayout->addWidget(m_Stop);

    m_Layout->addLayout(m_ModesLayout,0, 0);
    m_Layout->addWidget(m_Keyframes, 1, 0);
    m_Layout->addLayout(m_TimeSet, 2, 0);
    m_Layout->addLayout(m_ActionsLayout, 3, 0);

    setLayout(m_Layout);

    connect(m_TranslateMode, SIGNAL(pressed()),
            this, SLOT(setTransKeyMode()));

    connect(m_RotateMode, SIGNAL(pressed()),
            this, SLOT(setRotateKeyMode()));

    connect(m_ScaleMode, SIGNAL(pressed()),
            this, SLOT(setScaleKeyMode()));

    connect(m_ColorMode, SIGNAL(pressed()),
            this, SLOT(setColorKeyMode()));
}
