#ifndef SONGTEXT_H_
#define SONGTEXT_H_

#include <QString>
#include <QVector>
#include <QTreeWidget>

#include "ChordsTypes.h"

class SongText
    : public QTabWidget
{
    Q_OBJECT
public:
    SongText();
    virtual ~SongText();
    
    QString getText();
    void songSelected(const Song& song);
    void noSongSelected();
};

#endif /* SONGTEXT_H_ */

