#include "TimeLine.h"

TimeLine::TimeLine(std::vector<std::pair<Point, vanim::comp> > & points, QWidget *parent) :
    QWidget(parent),
    m_CurrentKeyframeMode(TRANS_KEYS),
    m_Points(points),
    m_Selection(-1),
    m_KeyframeSelection(-1)
{
    xSelect.setCoords(-1, 25, 1, 100);
    m_LineColor.setRgb(0, 0, 0);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setFocusPolicy(Qt::ClickFocus);
    setFixedHeight(120);
    setMouseTracking(true);
    show();

    m_Player.setSingleShot(true);

    connect(&m_Player, SIGNAL(timeout()),
            this, SLOT(stop()));

    connect(&m_Refresher, SIGNAL(timeout()),
            this, SLOT(updateTime()));
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

    int TriangleWidth = 3;

    if(m_Selection != -1)
    {
        if(m_KeyframeSelection != -1)
        {
            if(m_CurrentKeyframeMode == TRANS_KEYS)
            {
                QPainterPath path;
                std::vector<vanim::comp::TranslateDelta>& transKeys = m_Points[m_Selection].second.m_TranslateDeltas;
                path.setFillRule(Qt::WindingFill);
                path.moveTo(transKeys[m_KeyframeSelection].time * width(), 26);
                path.lineTo(transKeys[m_KeyframeSelection].time * width() - (TriangleWidth +1), 9);
                path.lineTo(transKeys[m_KeyframeSelection].time * width() + (TriangleWidth +1), 9);
                painter.setPen(Qt::NoPen);
                painter.fillPath(path, QBrush(QColor(255, 255, 0)));
            }
        }

        if(m_CurrentKeyframeMode == TRANS_KEYS)
        {
            std::vector<vanim::comp::TranslateDelta>& transKeys = m_Points[m_Selection].second.m_TranslateDeltas;

            for(uint i = 0; i < transKeys.size(); i++)
            {
                QPainterPath path;
                path.setFillRule(Qt::WindingFill);
                path.moveTo(transKeys[i].time * width(), 25);
                path.lineTo(transKeys[i].time * width() - TriangleWidth, 10);
                path.lineTo(transKeys[i].time * width() + TriangleWidth, 10);
                painter.setPen(Qt::NoPen);
                painter.fillPath(path, QBrush(QColor(0, 0, 0)));
            }

        }
        else if(m_CurrentKeyframeMode == ROTATE_KEYS)
        {
            std::vector<vanim::comp::RotateDelta>& rotateKeys = m_Points[m_Selection].second.m_RotateDeltas;
            for(uint i = 0; i < rotateKeys.size(); i++)
            {
                QPainterPath path;
                path.moveTo(rotateKeys[i].time * width(), 25);
                path.lineTo(rotateKeys[i].time * width() - TriangleWidth, 10);
                path.lineTo(rotateKeys[i].time * width() + TriangleWidth, 10);
                painter.setPen(Qt::NoPen);
                painter.fillPath(path, QBrush(QColor(255, 0, 0)));
            }
        }
        else if(m_CurrentKeyframeMode == SCALE_KEYS)
        {
            std::vector<vanim::comp::ScaleDelta>& scaleKeys = m_Points[m_Selection].second.m_ScaleDeltas;
            for(uint i = 0; i < scaleKeys.size(); i++)
            {
                QPainterPath path;
                path.moveTo(scaleKeys[i].time * width(), 25);
                path.lineTo(scaleKeys[i].time * width() - TriangleWidth, 10);
                path.lineTo(scaleKeys[i].time * width() + TriangleWidth, 10);
                painter.setPen(Qt::NoPen);
                painter.fillPath(path, QBrush(QColor(0, 255, 0)));
            }
        }
        else if(m_CurrentKeyframeMode == COLOR_KEYS)
        {
            // to be implemented
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

    if(m_Selection != -1)
    {
        if(m_KeyframeSelection != -1)
        {
            if(m_KeyframePressed)
            {
                if(m_CurrentKeyframeMode == TRANS_KEYS)
                {
                    std::vector<vanim::comp::TranslateDelta>& transKeys = m_Points[m_Selection].second.m_TranslateDeltas;
                    auto iter = transKeys.begin() + m_KeyframeSelection;
                    auto prev = std::prev(iter, 1);
                    auto next = std::next(iter, 1);
                    transKeys[m_KeyframeSelection].time = event->x() / static_cast<float>(width());

                    if(prev != transKeys.begin()-1 && transKeys[m_KeyframeSelection].time < (*prev).time)
                    {
                        std::iter_swap(iter, prev);
                        m_KeyframeSelection--;
                    }

                    if(next != transKeys.end() && transKeys[m_KeyframeSelection].time > (*next).time)
                    {
                        std::iter_swap(iter, next);
                        m_KeyframeSelection++;
                    }
                }

                xSelect.setCoords(event->x() - 1, 25, event->x() + 1, 100);
            }
        }
    }

    repaint();
}

void TimeLine::mouseReleaseEvent(QMouseEvent *)
{
    m_KeyframePressed = false;

}

void TimeLine::mousePressEvent(QMouseEvent * event)
{
    if(event->x() > 0 && event->x() < width() &&
       event->y() > 25 && event->y() < 100)
    {
        xSelect.setCoords( event->x() - 1, 25, event->x() + 1, 100);
    }

    if(event->x() > 0 && event->x() < width() &&
       event->y() > 0 && event->y() < 25)
    {
        if(m_CurrentKeyframeMode == TRANS_KEYS && m_Selection != -1)
        {
            std::vector<vanim::comp::TranslateDelta>& transKeys = m_Points[m_Selection].second.m_TranslateDeltas;
            for(uint i = 0; i < transKeys.size(); i++)
            {
                if(event->x() > transKeys[i].time * width() - 4 && event->x() < transKeys[i].time *width() + 4 &&
                   event->y() > 0 && event->y() < 25)
                {
                    m_KeyframeSelection = i;
                    xSelect.setCoords((transKeys[i].time * width()) - 1, 25, (transKeys[i].time * width()) + 1, 100);
                    m_KeyframePressed = true;
                    break;
                }
            }
        }
    }
    else
    {
        m_KeyframeSelection = -1;
    }

    emit setKeyframeSelection(m_KeyframeSelection);
    emit timeChanged(getTime());

    repaint();
}

void TimeLine::keyPressEvent(QKeyEvent * event)
{
    qDebug() << "button pressed\n";
    if(event->key() == Qt::Key_Delete && m_KeyframeSelection != -1)
    {
        qDebug() << "delete pressed";

        if(m_KeyframeSelection != -1)
        {
            if(m_CurrentKeyframeMode == TRANS_KEYS)
            {
                std::vector<vanim::comp::TranslateDelta>& transKeys = m_Points[m_Selection].second.m_TranslateDeltas;
                auto iter = transKeys.begin() + m_KeyframeSelection;
                transKeys.erase(iter);
                m_KeyframeSelection = -1;
            }
            else if(m_CurrentKeyframeMode == ROTATE_KEYS)
            {

            }
            else if(m_CurrentKeyframeMode == SCALE_KEYS)
            {

            }
            else if(m_CurrentKeyframeMode == COLOR_KEYS)
            {

            }
            emit setKeyframeSelection(m_KeyframeSelection);
        }

    }

    repaint();

    QWidget::keyPressEvent(event);
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

void TimeLine::play(float secs)
{
    m_Duration = secs;
    m_Refresher.start(16);
    m_Player.start(secs * 1000);
    m_ElapsedTime.start();
}

void TimeLine::stop()
{
    qDebug() << "stop";
    m_Refresher.stop();
    m_Player.stop();
    m_ElapsedTime.elapsed();
}

void TimeLine::reset()
{
    m_Refresher.stop();
    m_Player.stop();
    m_ElapsedTime.elapsed();

    xSelect.setCoords(-1, 25, 1, 100);
    emit timeChanged(getTime());
}

void TimeLine::updateTime()
{
    float t = (m_ElapsedTime.elapsed() / (m_Duration* 1000)) * width();

    qDebug() << "full ham";
    xSelect.setCoords( t-1, 25, t+1, 100);
    emit timeChanged(getTime());
    repaint();
}
