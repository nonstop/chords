
#include <QtGui>

#include "common.h"
#include "SongEdit.h"
#include "SongDlg.h"


SongDlg::SongDlg(QWidget *parent, const QString& artistName, const QStringList& artistNames, const QStringList& songNames)    
    : QDialog(parent), m_artistNames(artistNames), m_songNames(songNames)
{
    setWindowTitle(tr("Add song"));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    
    QHBoxLayout *spl = new QHBoxLayout;

    QVBoxLayout *left = new QVBoxLayout;

    QHBoxLayout *hbArtistName = new QHBoxLayout;
    QLabel *lArtistName = new QLabel(tr("Artist:"));
    m_leArtistName = new QLineEdit(artistName);
    m_leArtistName->setCompleter(&m_artistNames);

    hbArtistName->addWidget(lArtistName, 0);
    hbArtistName->addWidget(m_leArtistName);

    QHBoxLayout *hbSongName = new QHBoxLayout;
    QLabel *lSongName = new QLabel(tr("Song:"));
    m_leSongName = new QLineEdit;
    m_leSongName->setCompleter(&m_songNames);

    hbSongName->addWidget(lSongName, 0);
    hbSongName->addWidget(m_leSongName);

    m_bFromFile = new QPushButton(tr("Load from file..."));

    left->addLayout(hbArtistName, 0);
    left->addLayout(hbSongName, 0);
    left->addWidget(m_bFromFile, 0);
    left->addStretch(1);

    m_teText = new SongEdit;
    m_teText->setDocument(&m_tdText);

    spl->addLayout(left, 1);
    spl->addWidget(m_teText, 2);

    QHBoxLayout *hbBtns = new QHBoxLayout;
    m_bOk = new QPushButton(tr("Ok"));
    m_bOk->setDefault(true);
    m_bCancel = new QPushButton(tr("Cancel"));
    hbBtns->addStretch(1);
    hbBtns->addWidget(m_bOk, 0);
    hbBtns->addWidget(m_bCancel, 0);

    mainLayout->addLayout(spl);
    mainLayout->addLayout(hbBtns);
    
    setSizeGripEnabled(true);

    connect(m_bFromFile, SIGNAL(clicked()), this, SLOT(loadFromFile()));
    connect(m_bOk, SIGNAL(clicked()), this, SLOT(onOk()));
    connect(m_bCancel, SIGNAL(clicked()), this, SLOT(reject()));


    QFontDatabase db;
    QStringList lst = db.families();
    for (int i=0; i<lst.size(); i++)
        TRACE1(lst[i].toStdString());
}
 
SongDlg::~SongDlg()
{
}

QString SongDlg::getSongText() const
{
    return m_tdText.toPlainText();
}
QString SongDlg::getArtistName() const
{
    return m_leArtistName->text();
}
QString SongDlg::getSongName() const
{
    return m_leSongName->text();
}

void SongDlg::onOk()
{
    if (!m_leArtistName->text().length())
    {
        QMessageBox::information(this, tr("Error"), tr("Artist name is empty"), QMessageBox::Ok);
        return;
    }
    if (!m_leSongName->text().length())
    {
        QMessageBox::information(this, tr("Error"), tr("Song name is empty"), QMessageBox::Ok);
        return;
    }
    accept();
}

void SongDlg::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open song file"), QDir::currentPath(), tr("Text files (*.txt);;All files (*.*)"));
    if (fileName == "")
        return;

    QFile fl(fileName);
    if (!fl.open(QIODevice::ReadOnly))
    {
        QString msg = tr("Can't open file: ") + fileName;
        QMessageBox::critical(this, tr("Error"), msg);
        return;
    }
    QTextStream txt(&fl);
    m_tdText.setPlainText(txt.readAll());
}

