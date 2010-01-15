
#ifndef PREFERENCES_H_
#define PREFERENCES_H_

class QString;
class QSettings;

class Preferences
{
public:
    static Preferences& Instance();

    void setSongTextFont(const QString& fontName, int fontSize);
    QString getSongTextFontName();
    int getSongTextFontSize();
private:
    Preferences();

    QSettings m_settings;
};


#endif /* PREFERENCES_H_ */

