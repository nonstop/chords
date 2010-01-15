#include <QtGui>
#include <QtSql>

#include "common.h"
#include "SongDlg.h"
#include "SongsTree.h"

#include <algorithm>

SongsTree::SongsTree()
{
    setHeaderItem(new QTreeWidgetItem(QStringList(tr("Songs"))));

    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemClicked(QTreeWidgetItem*, int)));
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));

    createActions();
    loadSongs();
    sortItems(0, Qt::AscendingOrder);
    loadCompletion();
}

SongsTree::~SongsTree()
{}

void SongsTree::loadCompletion()
{
    m_artistNames.clear();
    m_songNames.clear();
    for (ArtistsConstIter ai = m_artists.begin(); ai != m_artists.end(); ai++)
    {
        m_artistNames << ai->getName();
        for (SongsConstIter si = ai->getSongs().begin(); si != ai->getSongs().end(); si++)
            m_songNames << si->getName();
    }
}

void SongsTree::loadSongs()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "songs.dat");
    db.setDatabaseName("songs.dat");
    if (!db.open())
        throw ChException(CURRPOS, "failed to open songs.dat");
    if (!db.isOpen())
        throw ChException(CURRPOS, "songs.dat is not opened");

    QSqlQuery artistQuery(db);
    artistQuery.exec("select id, name from artists");
    while (artistQuery.next())
    {
        int artistIndex = artistQuery.value(0).toInt();
        Artist a(artistQuery.value(1).toString());
        
        QSqlQuery songQuery("select id, name from songs where id_artist = :id_artist", db);
        songQuery.bindValue(":id_artist", artistIndex);
        songQuery.exec();
        while (songQuery.next())
        {
            int songIndex = songQuery.value(0).toInt();
            Song s(songQuery.value(1).toString());
            
            QSqlQuery textQuery("select value from texts where id_song = :id_song", db);
            textQuery.bindValue(":id_song", songIndex);
            textQuery.exec();
            while (textQuery.next())
                s.addText(textQuery.value(0).toString());

            
            a.addSong(s);
        }

        m_artists.push_back(a);
    }

    db.close();

    for (ArtistsConstIter ai = m_artists.begin(); ai != m_artists.end(); ai++)
    {
        QTreeWidgetItem *artist = new QTreeWidgetItem;
        artist->setText(0, ai->getName());

        for (SongsConstIter si = ai->getSongs().begin(); si != ai->getSongs().end(); si++)
        {
            QTreeWidgetItem *song = new QTreeWidgetItem;
            song->setText(0, si->getName());
            artist->addChild(song);
        }
        addTopLevelItem(artist);
    }
    sortItems(0, Qt::AscendingOrder);
}

void SongsTree::saveSongs()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "songs.dat");
    db.setDatabaseName("songs.dat");
    db.open();
    QSqlQuery q(db);
    q.exec("drop table artists");
    q.exec("drop table songs");
    q.exec("drop table texts");
    q.exec("create table artists (id integer primary key, name text)");
    q.exec("create table songs (id integer primary key, name text, id_artist integer)");
    q.exec("create table texts (id integer primary key, value text, id_song integer)");
    
    int artistIndex = 0, songIndex = 0, textIndex = 0;
    for (ArtistsConstIter ai = m_artists.begin(); ai != m_artists.end(); ai++, artistIndex++)
    {
        q.prepare("INSERT INTO artists (id, name) VALUES (:id, :name)");
        q.bindValue(":id", artistIndex);
        q.bindValue(":name", ai->getName());
        q.exec();
        const Songs& songs = ai->getSongs();
        for (SongsConstIter si = songs.begin(); si != songs.end(); si++, songIndex++)
        {
            q.prepare("INSERT INTO songs (id, name, id_artist) VALUES (:id, :name, :id_artist)");
            q.bindValue(":id", songIndex);
            q.bindValue(":name", si->getName());
            q.bindValue(":id_artist", artistIndex);
            q.exec();
            const Texts& texts = si->getTexts();
            for (TextsConstIter ti = texts.begin(); ti != texts.end(); ti++, textIndex++)
            {
                q.prepare("INSERT INTO texts (id, value, id_song) VALUES (:id, :name, :id_song)");
                q.bindValue(":id", textIndex);
                q.bindValue(":name", QString(*ti));
                q.bindValue(":id_song", songIndex);
                q.exec();
            }
        }
    }
    db.close();
}


QTreeWidgetItem *SongsTree::addSong(const QString& artistName, const QString& songName, const QString& songText)
{
    QTreeWidgetItem *songItem = NULL, *artistItem = NULL;
    QList<QTreeWidgetItem*> items;
    items = findItems(artistName, Qt::MatchExactly);
    ArtistsIter ai = std::find(m_artists.begin(), m_artists.end(), artistName);
    if (ai == m_artists.end())  //  There is no such artist
    {
        MSG("There is no such artist");
        Artist a(artistName, Song(songName, songText));
        m_artists.push_back(a);

        artistItem = new QTreeWidgetItem(QStringList(artistName));
        
        songItem = new QTreeWidgetItem(QStringList(songName));
        artistItem->addChild(songItem);

        addTopLevelItem(artistItem);
    }
    else    //  artist already exists
    {
        MSG("artist already exists");
        if (items.size() != 1)
            throw ChException(CURRPOS, "Possible bug: failed to find artist item in tree");
        QTreeWidgetItem *artistItem = items[0];
        
        int i = 0;
        QModelIndex mi = indexFromItem(artistItem);
        QModelIndex ci = mi.child(i, 0);
        
        while (ci != QModelIndex())
        {
            QTreeWidgetItem *tmp = itemFromIndex(ci);
            if (tmp->text(0) == songName)
            {
                songItem = tmp;
                break;
            }
            ci = mi.child(++i, 0);
        }

        SongsIter si = std::find(ai->getSongs().begin(), ai->getSongs().end(), songName);
        if (si == ai->getSongs().end()) //  artist has no such song
        {
            MSG("artist has no such song");
            ai->addSong(Song(songName, songText));
            songItem = new QTreeWidgetItem(QStringList(songName));
            artistItem->addChild(songItem);
        }
        else    //  artist already has such song
        {
            MSG("artist already has such song");
            QString msg = tr("Artist ") + artistName + tr(" already has this song.\nAdd song anyway?");
            if (QMessageBox::question(this, tr("Warning"), msg, QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
                return songItem;
            si->addText(songText);
            if (!songItem)
                throw ChException(CURRPOS, "Possible bug: failed to find song item in tree");
//            songItem->setData(0, Qt::DisplayRole, si->getName());
        }
    }
    sortItems(0, Qt::AscendingOrder);
    expandItem(artistItem);
    setCurrentItem(songItem);
    return songItem;
}

void SongsTree::createActions()
{
    onAddSong = new QAction(QIcon(":add song"), tr("Add song"), this);
    connect(onAddSong, SIGNAL(triggered()), this, SLOT(addSong()));

    onDeleteSong = new QAction(QIcon(":delete song"), tr("Delete song"), this);
    connect(onDeleteSong, SIGNAL(triggered()), this, SLOT(deleteSong()));

    onSaveSongs = new QAction(QIcon(":save song"), tr("Save songs"), this);
    connect(onSaveSongs, SIGNAL(triggered()), this, SLOT(saveSongs()));

    onQuit = new QAction(QIcon(":quit"), tr("Quit"), this);
    connect(onQuit, SIGNAL(triggered()), this, SLOT(exitSongs()));
}

void SongsTree::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(onAddSong);
    menu.addAction(onDeleteSong);
    menu.addSeparator();
    menu.addAction(onSaveSongs);
    menu.addSeparator();
    menu.addAction(onQuit);

    menu.exec(event->globalPos());
}

void SongsTree::exitSongs()
{
    emit quit();
}

void SongsTree::addSong()
{
    QString currentArtist;
    
    QTreeWidgetItem *it = currentItem();
    if (!it)
        return;
    if (it->parent())
        currentArtist = it->parent()->text(0);
    else
        currentArtist = it->text(0);

    SongDlg dlg(this, currentArtist, m_artistNames, m_songNames);

    if (dlg.exec() != QDialog::Accepted)
        return;
    itemClicked(addSong(dlg.getArtistName(), dlg.getSongName(), dlg.getSongText()), 0);
    loadCompletion();
}

void SongsTree::deleteSong()
{
    QTreeWidgetItem *songItem = currentItem();
    if (!songItem)
        return;
    QTreeWidgetItem *artistItem = songItem->parent();
    if (!artistItem)
        return;
    QString msg = tr("Delete song?\nArtist: ") + songItem->parent()->text(0) + tr("\nSong: ") + songItem->text(0);
    if (QMessageBox::question(this, tr("Warning"), msg, QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;
    
    ArtistsIter ai = std::find(m_artists.begin(), m_artists.end(), artistItem->text(0));
    ai->deleteSong(songItem->text(0));
    delete songItem;
    if (!artistItem->childCount())
    {
        m_artists.erase(ai);
        delete artistItem;
    }
    itemClicked(currentItem(), 0);
    loadCompletion();
}

Song& SongsTree::findSong(const QTreeWidgetItem* item)
{
    if (!item)
        throw ChException(CURRPOS, "item is NULL");
    QTreeWidgetItem *artistItem = item->parent();
    if (!artistItem)
        throw ChException(CURRPOS, "item has no parent");

    ArtistsIter ai = std::find(m_artists.begin(), m_artists.end(), artistItem->text(0));
    if (ai == m_artists.end())
        throw ChException(CURRPOS, "artist not found");
    SongsIter si = std::find(ai->getSongs().begin(), ai->getSongs().end(), item->text(0));
    if (si == ai->getSongs().end())
        throw ChException(CURRPOS, "song not found");
    return *si;
}


void SongsTree::itemClicked(QTreeWidgetItem* item, int)
{
    if (!item || !item->parent())
    {
        emit noSongSelected();
        return;
    }

    emit songSelected(findSong(item));
}

void SongsTree::itemSelectionChanged()
{
    QTreeWidgetItem *item = currentItem();
    if (!item || !item->parent())
    {
        emit noSongSelected();
        return;
    }

    emit songSelected(findSong(item));
}

void SongsTree::deleteText(int textIndex)
{
    QTreeWidgetItem *songItem = currentItem();
    if (!songItem)
        return;
    Song& song = findSong(songItem);
    song.deleteText(textIndex);

    if (!song.getTextsCount())
        deleteSong();
    itemClicked(currentItem(), 0);
    loadCompletion();
}

void SongsTree::saveText(int textIndex, const QString& text)
{
    TRACE1(textIndex);
    TRACE1(text.toStdString());
    
    QTreeWidgetItem *songItem = currentItem();
    if (!songItem)
        return;
    Song& song = findSong(songItem);
    song.setText(textIndex, text);

    itemClicked(currentItem(), 0);
    loadCompletion();
}

