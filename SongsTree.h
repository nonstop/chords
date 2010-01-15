#ifndef SONGSTREE_H_
#define SONGSTREE_H_

#include <QString>
#include <QTreeWidget>

#include "ChordsTypes.h"

class SongsTree
    : public QTreeWidget
{
    Q_OBJECT
public:
    SongsTree();
    virtual ~SongsTree();

private slots:
    void addSong();
    void deleteSong();
    void saveSongs();
    void exitSongs();
    void itemClicked(QTreeWidgetItem*, int);
    void itemSelectionChanged();
    void deleteText(int textIndex);
    void saveText(int textIndex, const QString& text);
signals:
     void songSelected(const Song& song);
     void noSongSelected();
     void quit();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QAction *onAddSong;
    QAction *onDeleteSong;
    QAction *onSaveSongs;
    QAction *onQuit;

    QStringList m_artistNames;
    QStringList m_songNames;
    Artists m_artists;
    
    Song& findSong(const QTreeWidgetItem* item);
    void createActions();
    void loadSongs();
    void loadCompletion();
    QTreeWidgetItem* addSong(const QString& artistName, const QString& songName, const QString& songText);
};

#endif /* SONGSTREE_H_ */

