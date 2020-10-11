#pragma once

#include "Utilities.hpp"
#include "Map.hpp"
#include "Notation.hpp"


namespace OK {

const std::string c_infoFileName = "Info.dat";
const std::string c_notationExtenton = ".json";
const std::string c_mapFileExtention = ".dat";

struct Info {
  std::string version;
  std::string songName;
  std::string songSubName;
  std::string songAuthorName;
  std::string levelAuthorName;
  float beatsPerMinute = 120;
  float shuffle;
  float shufflePerio;
  float previewStartTime;
  float previewDuration;
  std::string songFilename;
  std::string coverImageFilename;
  std::string environmentName;
  float songTimeOffset;
};

// Song is a representaion of a the collection of maps Beat Saber song.
class Song {
private:
    Info m_info;
    std::string m_path;
    std::vector<Map> m_maps;
    std::vector<Notation> m_notations;

public:

    Song();
    Song(std::string path);    

    bool loadSongFromDirectory(std::string path);   // Loads all resources from a path to a driectory.
    bool saveSongToDirectory();     // Saves all the resources of the song to the stored path.

    void loadInfo(const std::string fileName);
    void saveInfo();

    Map getMap(int index);
    Map getMap(const std::string name);
    int addMap(Map map);
    int loadMap(const std::string fileName);
    bool saveMap(int index);
    bool saveMap(const std::string name);
    void removeMap(const std::string name);

    Notation getNotation(int index);
    Notation getNotation(const std::string name);
    int loadNotation(const std::string fileName);
    int createNotationFromMap(Map map); // Constructs a Notation from a collection of keyframes extraced from a Map.
    bool saveNotation(int index);
    bool saveNotation(const std::string name);

    int addNotation(Notation notation);
    void removeNotation(const std::string name);
    
};

} // Namespace OK
