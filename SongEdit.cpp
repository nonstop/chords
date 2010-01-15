

#include <QtGui>
#include "Highlighter.h"

#include "SongEdit.h"
#include "Preferences.h"

#include "common.h"

SongEdit::SongEdit(QWidget *parent)
    : QTextEdit(parent), m_hl(NULL)
{
    TRACE;
    m_hl = new Highlighter(this);
    setSongText("");
}

SongEdit::~SongEdit()
{
    if (m_hl)
        delete m_hl;
}

void SongEdit::setSongText(const QString& text)
{
    setText(text);
    setCurrentFont(QFont(Preferences::Instance().getSongTextFontName(), Preferences::Instance().getSongTextFontSize()));
    setLineWrapMode(QTextEdit::NoWrap);
}


