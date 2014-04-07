#include "TextureViewer.h"

TextureViewer::TextureViewer(std::vector<std::pair<Point, vanim::comp> > & points, QWidget *parent) :
    QWidget(parent, Qt::Window),
    m_Points(points),
    m_Selection(-1),
    m_isSelection(false)
{
}

void TextureViewer::loadTexture(QString filename)
{
    m_Img.load(filename);
}

void TextureViewer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(!m_Img.isNull())
    {
        float imgAspect = m_Img.width()/ static_cast<float>(m_Img.height());
        float windowAspect = width()/ static_cast<float>(height());
        QRect rect;

        if(imgAspect > windowAspect)
        {
            rect.setCoords(0, 0, width(), (m_Img.height() * width())/ m_Img.width());
        }
        else
        {
            rect.setCoords(0, 0, (m_Img.width() * height())/ m_Img.height(), height());
        }

        painter.drawImage(rect, m_Img);

        if(m_Selection != -1)
        {
            QPoint texs(m_Points[m_Selection].first.texSX * rect.width(), m_Points[m_Selection].first.texSY * rect.height());
            QPoint texe(m_Points[m_Selection].first.texEX * rect.width(), m_Points[m_Selection].first.texEY * rect.height());

            if(m_isSelection)
            {
                //qDebug() << texs.y() << "  " << texe.y();
                painter.fillRect(0, 0, rect.width(), texs.y(), QColor(150, 150, 150, 150));
                painter.fillRect(0, texs.y(), texs.x(), texe.y() - texs.y(), QColor(150, 150, 150, 150));
                painter.fillRect(texe.x(), texs.y(), rect.width() - texe.x(), texe.y() - texs.y(), QColor(150, 150, 150, 150));
                painter.fillRect(0, texe.y(), rect.width(), rect.height() - texe.y(), QColor(150, 150, 150, 150));
            }
        }
    }
}

void TextureViewer::mousePressEvent(QMouseEvent * event)
{
    if(m_Selection != -1)
    {
        float imgAspect = m_Img.width()/ static_cast<float>(m_Img.height());
        float windowAspect = width()/ static_cast<float>(height());
        QRect rect;

        if(imgAspect > windowAspect)
        {
            rect.setCoords(0, 0, width(), (m_Img.height() * width())/ m_Img.width());
        }
        else
        {
            rect.setCoords(0, 0, (m_Img.width() * height())/ m_Img.height(), height());
        }


        if(event->x() > 0 && event->x() < rect.width() &&
           event->y() > 0 && event->y() < rect.height())
        {
            m_isSelection = true;

            m_Points[m_Selection].first.texSX = m_Points[m_Selection].first.texEX = event->x() / static_cast<float>(rect.width());
            m_Points[m_Selection].first.texSY = m_Points[m_Selection].first.texEY = event->y() / static_cast<float>(rect.height());

            m_Points[m_Selection].second.texSX = m_Points[m_Selection].first.texSX;
            m_Points[m_Selection].second.texSY = m_Points[m_Selection].first.texSY;
            m_Points[m_Selection].second.texEX = m_Points[m_Selection].first.texEX;
            m_Points[m_Selection].second.texEY = m_Points[m_Selection].first.texEY;

        }
        emit updateSelection();
    }

    repaint();
}

void TextureViewer::mouseMoveEvent(QMouseEvent * event)
{
    if(m_Selection != -1)
    {
        float imgAspect = m_Img.width()/ static_cast<float>(m_Img.height());
        float windowAspect = width()/ static_cast<float>(height());
        QRect rect;

        if(imgAspect > windowAspect)
        {
            rect.setCoords(0, 0, width(), (m_Img.height() * width())/ m_Img.width());
        }
        else
        {
            rect.setCoords(0, 0, (m_Img.width() * height())/ m_Img.height(), height());
        }

        if(event->x() > m_Points[m_Selection].first.texSX * rect.width() && event->x() < rect.width() &&
           event->y() > m_Points[m_Selection].first.texSY * rect.height() && event->y() < rect.height() && m_isSelection)
        {
            m_Points[m_Selection].first.texEX = m_Points[m_Selection].second.texEX = event->x() / static_cast<float>(rect.width());
            m_Points[m_Selection].first.texEY = m_Points[m_Selection].second.texEY = event->y() / static_cast<float>(rect.height());
        }
        emit updateSelection();
    }
    repaint();
}
