
#include <QSettings>

#include "Preferences.h"


Preferences& Preferences::Instance()
{
    static Preferences preferences;

    return preferences;
}


Preferences::Preferences()
    : m_settings("Nonstop", "Chords")
{
}
    
void Preferences::setSongTextFont(const QString& fontName, int fontSize)
{
    m_settings.setValue("editor/fontName", fontName);
    m_settings.setValue("editor/fontSize", fontSize);
}

QString Preferences::getSongTextFontName()
{
    QString fontName = m_settings.value("editor/fontName").toString();
    if (fontName != "")
        return fontName;
    else
        return "Courier New";
}

int Preferences::getSongTextFontSize()
{
    int fontSize = m_settings.value("editor/fontSize").toInt();
    if (fontSize)
        return fontSize;
    else
        return 14;
}

