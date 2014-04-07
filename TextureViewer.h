#ifndef TEXTUREVIEWER_H
#define TEXTUREVIEWER_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include "PlotWidget.h"
#include "common.h"

class TextureViewer :
    public QWidget
{
    Q_OBJECT
public:
    explicit TextureViewer(std::vector<std::pair<Point, vanim::comp> > &, QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *);

    void mouseMoveEvent(QMouseEvent *);

    void loadTexture(QString filename);

signals:
    void updateSelection();

public slots:
    void setSelection(int select)
    {
        m_Selection = select;
        if(m_Selection != -1)
        {
            if(m_Points[m_Selection].first.texEX != 0 || m_Points[m_Selection].first.texEY != 0 ||
               m_Points[m_Selection].first.texSX != 0 || m_Points[m_Selection].first.texSY != 0)
            {
                m_isSelection = true;
            }
            else
            {
                m_isSelection = false;
            }
        }
        repaint();
    }

private:
    QImage m_Img;
    std::vector<std::pair<Point, vanim::comp> > & m_Points;
    int m_Selection;

    bool m_isSelection;
};

#endif // TEXTUREVIEWER_H
