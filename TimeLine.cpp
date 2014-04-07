#include "TimeLine.h"

TimeLine::TimeLine(std::vector<std::pair<Point, vanim::comp> > & points, QWidget *parent) :
    QWidget(parent),
    m_CurrentKeyframeMode(AnimationWidget::TRANS_KEYS),
    m_Points(points),
    m_Selection(-1)
{
    xSelect.setCoords(-1, 25, 1, 100);
    m_LineColor.setRgb(0, 0, 0);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setFixedHeight(120);
    setMouseTracking(true);
    show();
}

void TimeLine::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(0, 25, width(), 76, QColor(200, 200, 200));
    painter.fillRect(xSelect, m_LineColor);

    for(int i = 0; i < 26; i++)
    {
        if(i % 5 == 0)
        {
            painter.drawLine( width() * (i/25.0f)-1, 75, width() *(i/25.0f) - 1, 100);
            QString str;
            QTextStream num(&str);
            num << fixed << qSetRealNumberPrecision(1) << i/25.0f;
            if( i/25.0f == 0.0f)
                painter.drawText(width() *(i/25.0f) + 3, 110, str);
            else if(i/25.0f == 1.0f)
                painter.drawText(width() *(i/25.0f) - 15, 110, str);
            else
                painter.drawText(width() *(i/25.0f) - 8, 110, str);
        }
        else
            painter.drawLine( width() * (i/25.0f), 90, width() *(i/25.0f), 100);
    }
    QString str("Time = ");
    QTextStream stream(&str);
    stream << fixed << qSetRealNumberPrecision(6) << getTime();
    painter.drawText(width() - 100, 120, str);

    if(m_Selection != -1)
    {
        if(m_CurrentKeyframeMode == AnimationWidget::TRANS_KEYS)
        {
            std::vector<vanim::comp::TranslateDelta>& transKeys = m_Points[m_Selection].second.m_TranslateDeltas;

            for(uint i = 0; i < transKeys.size(); i++)
            {
                QPainterPath path;
                path.moveTo(transKeys[i].time * width(), 25);
                path.lineTo(transKeys[i].time * width() - 2, 10);
                path.lineTo(transKeys[i].time * width() + 2, 10);
                painter.setPen(Qt::NoPen);
                painter.fillPath(path, QBrush(QColor(0.0f, 0.0f, 0.0f)));
            }

        }
        else if(m_CurrentKeyframeMode == AnimationWidget::ROTATE_KEYS)
        {
            std::vector<vanim::comp::RotateDelta>& rotateKeys = m_Points[m_Selection].second.m_RotateDeltas;
            for(uint i = 0; i < rotateKeys.size(); i++)
            {
                QPainterPath path;
                path.moveTo(rotateKeys[i].time * width(), 25);
                path.lineTo(rotateKeys[i].time * width() - 2, 10);
                path.lineTo(rotateKeys[i].time * width() + 2, 10);
                painter.setPen(QPen(QColor(1.0f, 0.0f, 0.0f)));
                painter.fillPath(path, QBrush(QColor(1.0f, 0.0f, 0.0f)));
            }
        }
        else if(m_CurrentKeyframeMode == AnimationWidget::SCALE_KEYS)
        {
            std::vector<vanim::comp::ScaleDelta>& scaleKeys = m_Points[m_Selection].second.m_ScaleDeltas;
            for(uint i = 0; i < scaleKeys.size(); i++)
            {
                QPainterPath path;
                path.moveTo(scaleKeys[i].time * width(), 25);
                path.lineTo(scaleKeys[i].time * width() - 2, 10);
                path.lineTo(scaleKeys[i].time * width() + 2, 10);
                painter.setPen(Qt::NoPen);
                painter.fillPath(path, QBrush(QColor(1.0f, 0.0f, 0.0f)));
            }
        }
        else if(m_CurrentKeyframeMode == AnimationWidget::COLOR_KEYS)
        {

        }
    }
}

void TimeLine::mouseMoveEvent(QMouseEvent * event)
{
    if(event->x() > xSelect.x() && event->x() < xSelect.x() + xSelect.width() &&
       event->y() > xSelect.y() && event->y() < xSelect.y() + xSelect.height())
    {
        m_LineColor.setRgb(255, 255, 0);
    }
    else
        m_LineColor.setRgb(0, 0, 0);

    if(event->buttons() == Qt::LeftButton &&
       event->x() > 0 && event->x() < width() &&
       event->y() > 25 && event->y() < 100)
    {
        xSelect.setCoords(event->x() - 1, 25, event->x() + 1, 100);
        m_LineColor.setRgb(255, 255, 0);
        emit timeChanged(getTime());
    }

    repaint();
}

void TimeLine::mousePressEvent(QMouseEvent * event)
{
    if(event->x() > 0 && event->x() < width())
        xSelect.setCoords( event->x() - 1, 25, event->x() + 1, 100);

    emit timeChanged(getTime());

    repaint();
}

void TimeLine::resizeEvent(QResizeEvent *)
{
    repaint();
}

QSize TimeLine::sizeHint() const
{
    return QSize(width(), 100);
}

float TimeLine::getTime() const
{
    float x = xSelect.x() + 1;
    return x / width();
}
