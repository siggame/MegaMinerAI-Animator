#include "mainwindow.h"

MainWindow::MainWindow()
{
    this->setWindowTitle("example");
    m_MainWidget = new MainWidget(this);
    m_TextureWidget = new TextureViewer(m_MainWidget->m_Points, this);

    m_TextureWidget->loadTexture("point.png");
    m_TextureWidget->show();



    connect(m_MainWidget, SIGNAL(setSelection(int)),
            m_TextureWidget, SLOT(setSelection(int)));

    connect(m_TextureWidget, SIGNAL(updateSelection()),
            m_MainWidget, SLOT(propSelectionChange()));



    m_File = menuBar()->addMenu("File");
    m_File->addAction("New");
    m_File->addAction("Open");
    m_File->addAction("Save");
    m_File->addAction("Save As ...");
    m_File->addAction("Exit");

    m_View = menuBar()->addMenu("View");
    m_View->addAction("Texture Viewer");

    setCentralWidget(m_MainWidget);
}

MainWindow::~MainWindow()
{

}

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{

    m_Layout = new QHBoxLayout(this);

    m_CenterPane = new CentralPane(m_Points, this);
    m_RightPane = new RightPane(m_Points, this);

    m_Layout->addWidget(m_CenterPane);
    m_Layout->addWidget(m_RightPane, 0, Qt::AlignTop);

    connect(m_CenterPane, SIGNAL(SelectionChanged()),
            m_RightPane, SLOT(updateSelection()));

    connect(m_CenterPane, SIGNAL(setSelection(int)),
            this, SLOT(propSetSelection(int)));

    connect(m_CenterPane, SIGNAL(SelectionChanged()),
            this, SLOT(propSelectionChange()));

    connect(m_CenterPane, SIGNAL(setSelection(int)),
            m_RightPane, SLOT(setSelection(int)));

    connect(m_RightPane, SIGNAL(editModeChanged(EDITOR_MODE)),
            m_CenterPane, SLOT(setEditMode(EDITOR_MODE)));

    connect(m_RightPane, SIGNAL(keyframeModeChanged(KEYFRAME_MODE)),
            m_CenterPane, SLOT(setKeyFrameEditMode(KEYFRAME_MODE)));

    connect(m_RightPane, SIGNAL(play(float)),
            m_CenterPane, SLOT(play(float)));

}

QSize MainWidget::sizeHint() const
{
    return m_Layout->sizeHint();
}

QSize MainWidget::minimumSizeHint() const
{
    return m_Layout->minimumSize();
}

void MainWidget::propSelectionChange()
{
    m_RightPane->updateSelection();
    emit selectionChanged();
}

void MainWidget::propEditModeChange(EDITOR_MODE mode)
{
    emit editModeChanged(mode);
}
