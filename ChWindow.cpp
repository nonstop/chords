
#include <QtGui>
#include <QtSql>
#include "common.h"

#include "ChWindow.h"
#include "SongsTree.h"
#include "RightPaneWidget.h"

ChWindow::ChWindow()
{
    QWidget *centralWidget = new QWidget;
    
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    QSplitter *spl = new QSplitter(Qt::Horizontal);

    m_tabLeft = createLeftPane();

    m_wRight = new RightPaneWidget;
    
    spl->addWidget(m_tabLeft);
    spl->addWidget(m_wRight);

    spl->setStretchFactor(0, 0);
    spl->setStretchFactor(1, 1);
    spl->setCollapsible(0, false);
    spl->setCollapsible(1, false);
    
    mainLayout->addWidget(spl);

    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);


    connect(m_trSongs, SIGNAL(songSelected(const Song&)), m_wRight, SLOT(songSelected(const Song&)));
    connect(m_trSongs, SIGNAL(noSongSelected()), m_wRight, SLOT(noSongSelected()));
    connect(m_trSongs, SIGNAL(quit()), this, SLOT(onQuit()));

    void setText(int textIndex, const QString& text);
    void deleteText(int textIndex);
    connect(m_wRight, SIGNAL(setText(int, const QString&)), m_trSongs, SLOT(saveText(int, const QString&)));
    connect(m_wRight, SIGNAL(deleteText(int)), m_trSongs, SLOT(deleteText(int)));
    
    setWindowTitle(tr("Chords"));
    setMinimumSize(160, 160);
    resize(600, 400);
}

ChWindow::~ChWindow()
{}

void ChWindow::onQuit()
{
    close();
}

QTabWidget *ChWindow::createLeftPane()
{
    QTabWidget *tw = new QTabWidget;

    m_trSongs = new SongsTree;
    tw->addTab(m_trSongs, tr("Songs"));

    QWidget *wdg = createSearchPane();
    tw->addTab(wdg, tr("Search"));

    return tw;
}

QWidget *ChWindow::createSearchPane()
{
    QWidget *wdg = new QWidget;

    QVBoxLayout *vb = new QVBoxLayout;
    QHBoxLayout *hb = new QHBoxLayout;

    m_leSearch = new QLineEdit;
    m_bSearch = new QPushButton(tr("Search"));
    
    hb->addWidget(m_leSearch, 1);
    hb->addWidget(m_bSearch, 0);

    m_lSearch = new QListWidget;

    vb->addLayout(hb);
    vb->addWidget(m_lSearch, 1);
    
    wdg->setLayout(vb);
    return wdg;
}

