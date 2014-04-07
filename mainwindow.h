#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CentralPane.h"
#include "RightPane.h"
#include "TextureViewer.h"
#include "common.h"

class MainWidget :
    public QWidget
{
    Q_OBJECT

public:

    MainWidget(QWidget * parent = 0);

    QSize sizeHint() const;

    QSize minimumSizeHint() const;

signals:
    void selectionChanged();

    void setSelection(int);

    void editModeChanged(EDITOR_MODE);

public slots:
    void propSelectionChange();

    void propSetSelection(int select)
    {
        emit setSelection(select);
    }

    void propEditModeChange(EDITOR_MODE);

    void keyframeModeChanged(AnimationWidget::KEYFRAME_MODE mode)
    {
        m_CenterPane->setKeyFrameEditMode(mode);
    }

public:
    QHBoxLayout * m_Layout;
    CentralPane * m_CenterPane;
    RightPane * m_RightPane;

    std::vector<std::pair<Point, vanim::comp> > m_Points;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT  

public:
    MainWindow();

    ~MainWindow();

private:
    MainWidget * m_MainWidget;
    TextureViewer * m_TextureWidget;
};

#endif
