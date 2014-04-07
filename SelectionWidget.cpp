
#include "SelectionWidget.h"

SelectionForm::SelectionForm(std::vector<std::pair<Point, vanim::comp> >& points,
                             QWidget *parent) :
    QWidget(parent),
    m_Selection(-1),
    m_Points(points)
{
    m_Layout = new QGridLayout(this);

    m_Coords = new QLabel("Coords: ", this);
    m_TexScale = new QLabel("TexScale: ", this);
    m_RotLabel = new QLabel("Rotation: ", this);
    m_Offset = new QLabel("TexOffset: ", this);
    m_TexCoords = new QLabel("TexCoords: ", this);

    m_X = new QLineEdit(this);
    m_Y = new QLineEdit(this);
    m_Length = new QLineEdit(this);
    m_Width = new QLineEdit(this);
    m_Rot = new QLineEdit(this);
    m_OffsetX = new QLineEdit(this);
    m_OffsetY = new QLineEdit(this);
    m_TexSX = new QLineEdit(this);
    m_TexSY = new QLineEdit(this);
    m_TexEX = new QLineEdit(this);
    m_TexEY = new QLineEdit(this);

    m_Layout->addWidget(m_Coords, 0, 0, Qt::AlignLeft);
    m_Layout->addWidget(m_TexScale, 1, 0, Qt::AlignLeft);
    m_Layout->addWidget(m_RotLabel, 2, 0, Qt::AlignLeft);
    m_Layout->addWidget(m_Offset, 3, 0, Qt::AlignLeft);
    m_Layout->addWidget(m_TexCoords, 4, 0, Qt::AlignLeft);

    m_Layout->addWidget(m_X, 0, 1);
    m_Layout->addWidget(m_Y, 0, 2);

    m_Layout->addWidget(m_Width, 1, 1);
    m_Layout->addWidget(m_Length, 1, 2);

    m_Layout->addWidget(m_Rot, 2, 1);

    m_Layout->addWidget(m_OffsetX, 3, 1);
    m_Layout->addWidget(m_OffsetY, 3, 2);

    m_Layout->addWidget(m_TexSX, 4, 1);
    m_Layout->addWidget(m_TexSY, 4, 2);

    m_Layout->addWidget(m_TexEX, 5, 1);
    m_Layout->addWidget(m_TexEY, 5, 2);

    setMaximumSize(200, 400);

    setLayout(m_Layout);
}

void SelectionForm::updateSelection()
{
    QString str;
    QTextStream stream(&str);
    stream.setRealNumberPrecision(6);
    stream << fixed;

    if(m_Selection != -1)
    {
        stream << m_Points[m_Selection].first.x;
        m_X->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.y;
        m_Y->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.length;
        m_Length->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.width;
        m_Width->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.rot;
        m_Rot->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.offx;
        m_OffsetX->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.offy;
        m_OffsetY->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.texSX;
        m_TexSX->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.texSY;
        m_TexSY->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.texEX;
        m_TexEX->setText(str);
        str.clear();

        stream << m_Points[m_Selection].first.texEY;
        m_TexEY->setText(str);
        str.clear();
    }
    else
    {
        m_X->clear();
        m_Y->clear();
        m_Length->clear();
        m_Width->clear();
        m_Rot->clear();
        m_OffsetX->clear();
        m_OffsetY->clear();
        m_TexSX->clear();
        m_TexSY->clear();
        m_TexEX->clear();
        m_TexEY->clear();
    }
}


