#ifndef SELECTIONWIDGET_H
#define SELECTIONWIDGET_H

#include <QDockWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QTextStream>
#include "PlotWidget.h"
#include <QDebug>

class SelectionForm :
    public QWidget
{
    Q_OBJECT
public:
    SelectionForm(std::vector<std::pair<Point, vanim::comp> >&,QWidget * parent = 0);

    QSize sizeHint() const{ return QSize(200, 300);}

    QSize minimumSizeHint() const {return QSize(200, 300);}

public slots:
    void updateSelection();

    void setSelection(int select)
    {
        m_Selection = select;

        if(m_Selection != -1)
            updateSelection();
    }

private:
    QGridLayout * m_Layout;
    std::vector<std::pair<Point, vanim::comp> > & m_Points;
    int m_Selection;
    QLabel * m_Coords;
    QLabel * m_TexScale;
    QLabel * m_RotLabel;
    QLabel * m_Offset;
    QLabel * m_TexCoords;
    QLineEdit * m_X, * m_Y;
    QLineEdit * m_Length,* m_Width;
    QLineEdit * m_Rot;
    QLineEdit * m_OffsetX,* m_OffsetY;
    QLineEdit * m_TexSX,* m_TexSY;
    QLineEdit * m_TexEX,* m_TexEY;
};



#endif // SELECTIONWIDGET_H
