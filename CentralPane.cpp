#include "CentralPane.h"

CentralPane::CentralPane(std::vector<std::pair<Point, vanim::comp> >& points,
                         QWidget * parent) :
    QWidget(parent),
    m_Points(points),
    m_Selection(-1)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_Layout = new QVBoxLayout(this);
    m_PlotWindow = new PlotWindow(points);
    m_PlotWidget = new PlotWidget(QWidget::createWindowContainer(m_PlotWindow, this), this);
    m_Timeline = new TimeLine(points, this);

    m_Layout->addWidget(m_PlotWidget);
    m_Layout->addWidget(m_Timeline);

    setLayout(m_Layout);

    connect(m_PlotWindow, SIGNAL(setSelection(int)),
            this, SLOT(propSetSelection(int)));

    connect(m_PlotWindow, SIGNAL(selectionChanged()),
            this, SLOT(propSelectChanged()));

    connect(m_Timeline, SIGNAL(timeChanged(float)),
            m_PlotWindow, SLOT(setTime(float)));

    connect(m_PlotWindow, SIGNAL(selectionChanged()),
            m_Timeline, SLOT(updateSelection()));

    connect(m_PlotWindow, SIGNAL(setSelection(int)),
            m_Timeline, SLOT(setSelection(int)));
}

QSize CentralPane::minimumSizeHint() const
{
    return m_Layout->minimumSize();
}

QSize CentralPane::sizeHint() const
{
    return m_Layout->sizeHint();
}

void CentralPane::resizeEvent(QResizeEvent *)
{
    m_PlotWidget->resize(width() - 40, height() - m_Timeline->height() - 30);
    m_Timeline->resize(m_PlotWidget->width(), m_Timeline->height());
}
