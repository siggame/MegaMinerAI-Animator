#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <iostream>
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

    void keyframeModeChanged(KEYFRAME_MODE mode)
    {
        m_CenterPane->setKeyFrameEditMode(mode);
    }

    void NewAnim()
    {
    }

    void SaveAnim()
    {
    }

    void OpenAnim()
    {
    }

    void Exit()
    {
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

    QMenu * m_File;
    QMenu * m_View;
};

#endif
