
#include <QtGui>

#include "SongText.h"
#include "SongEdit.h"
#include "RightPaneWidget.h"

#include "common.h"

RightPaneWidget::RightPaneWidget()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    
    m_twTexts = new SongText;

    QHBoxLayout *btnLayout = new QHBoxLayout;

    m_bSave = new QPushButton(tr("Save"));
    m_bDelete = new QPushButton(tr("Delete"));

    btnLayout->addStretch(1);
    btnLayout->addWidget(m_bSave, 0);
    btnLayout->addWidget(m_bDelete, 0);

    mainLayout->addWidget(m_twTexts, 1);
    mainLayout->addLayout(btnLayout, 0);
    setLayout(mainLayout);

    connect(m_bSave, SIGNAL(clicked()), this, SLOT(onSaveText()));
    connect(m_bDelete, SIGNAL(clicked()), this, SLOT(onDeleteText()));
}

RightPaneWidget::~RightPaneWidget()
{
}

void RightPaneWidget::songSelected(const Song& song)
{
    m_twTexts->songSelected(song);
}

void RightPaneWidget::noSongSelected()
{
    m_twTexts->noSongSelected();
}

void RightPaneWidget::onSaveText()
{
    TRACE1(m_twTexts->count());
    if (!m_twTexts->count())
        return;
    int textIndex = m_twTexts->currentIndex();
    emit setText(textIndex, m_twTexts->getText());
}

void RightPaneWidget::onDeleteText()
{
    TRACE1(m_twTexts->count());
    if (!m_twTexts->count())
        return;
    int textIndex = m_twTexts->currentIndex();
    emit deleteText(textIndex);
}

