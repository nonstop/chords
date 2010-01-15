
#include <QtGui>

#include "SongEdit.h"
#include "SongText.h"

#include "common.h"

SongText::SongText()
{
}

SongText::~SongText()
{
}

QString SongText::getText()
{
    SongEdit *se = dynamic_cast<SongEdit*>(currentWidget());
    if (!se)
        throw ChException(CURRPOS, "no SongEdit available");
    return se->toPlainText();
}

void SongText::songSelected(const Song& song)
{
    TRACE;
    const int textsCnt = song.getTextsCount();
    if (count() > textsCnt)
        while (count() > textsCnt)
        {
            QWidget *wd = widget(count() - 1);
            removeTab(count() - 1);
            delete wd;
        }
    else
        while (count() < textsCnt)
        {
            addTab(new SongEdit, "");
        }

    int tabIndex = 0;
    for (TextsConstIter ti = song.getTexts().begin(); ti != song.getTexts().end(); ti++, tabIndex++)
    {
        if (!tabIndex)
            setTabText(0, song.getName() + " - 1");
        else
            setTabText(tabIndex, QVariant(tabIndex + 1).toString());

        SongEdit *te = dynamic_cast<SongEdit*>(widget(tabIndex));
        te->setSongText(*ti);
    }
}

void SongText::noSongSelected()
{
    TRACE;
    while (count())
    {
        QWidget *wd = widget(count() - 1);
        removeTab(count() - 1);
        delete wd;
    }
}

