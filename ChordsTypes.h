#ifndef CHORDSTYPES_H_
#define CHORDSTYPES_H_

#include <QString>
#include <vector>
#include <algorithm>

typedef std::vector<QString> Texts;
typedef Texts::iterator TextsIter;
typedef Texts::const_iterator TextsConstIter;
class Song
{
public:
    Song(const QString& name) : m_name(name)
    {}
    Song(const QString& name, const QString& text) : m_name(name)
    {
        addText(text);
    }
    bool operator==(const QString& song) const
    {   return m_name == song;   }
    const QString& getName() const
    {   return m_name;  }
    int getTextsCount() const
    {   return m_texts.size();  }
    const Texts& getTexts() const
    {   return m_texts; }
    void addText(const QString& text)
    {   m_texts.push_back(text);    }
    void deleteText(int textIndex)
    {   m_texts.erase(m_texts.begin() + textIndex);    }
    void setText(int textIndex, const QString& text)
    {   m_texts.at(textIndex) = text;   }
private:
    QString m_name;
    Texts m_texts;
};
typedef std::vector<Song> Songs;
typedef Songs::iterator SongsIter;
typedef Songs::const_iterator SongsConstIter;

class Artist
{
public:
    Artist(const QString &name) : m_name(name)
    {}
    Artist(const QString &name, const Song& song) : m_name(name)
    {
        addSong(song);
    }
    bool operator==(const QString& artist) const
    {   return m_name == artist;   }
    const QString& getName() const
    {   return m_name;  }
    int getSongsCount() const
    {   return m_songs.size();  }
    const Song& getSong(int n) const
    {   return m_songs.at(n);   }
    const Songs& getSongs() const
    {   return m_songs; }
    Songs& getSongs()
    {   return m_songs; }
    void addSong(const Song& song)
    {
        m_songs.push_back(song);
    }
    void deleteSong(const QString& songName)
    {
        SongsIter si = std::find(m_songs.begin(), m_songs.end(), songName);
        if (si != m_songs.end())
            m_songs.erase(si);
    }
private:
    QString m_name;
    Songs m_songs;
};
typedef std::vector<Artist> Artists;
typedef Artists::iterator ArtistsIter;
typedef Artists::const_iterator ArtistsConstIter;


#endif /* CHORDSTYPES_H_ */

