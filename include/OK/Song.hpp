#pragma once

#include "Utilities.hpp"
#include "Map.hpp"
#include "Notation.hpp"


namespace OK {

const std::string c_infoFileName = "Info.dat";
const std::string c_notationSuffix = "_notation.json";

struct Info {
  std::string version;
  std::string songName;
  std::string songSubName;
  std::string songAuthorName;
  std::string levelAuthorName;
  float beatsPerMinute;
  float shuffle;
  float shufflePerio;
  float previewStartTime;
  float previewDuration;
  std::string songFilename;
  std::string coverImageFilename;
  std::string environmentName;
  float songTimeOffset;
};

// Song is a representaion of a the collection of maps  Beat Saber song.
class Song {
private:
    Info m_info;
    std::string m_path;
    std::vector<Map> m_maps;
    std::vector<Notation> m_notations;

public:
    static Notation CreateNotationFromMap(Map map); // Constructs a Notation from a collection of keyframes extraced from a Map.

    Song();
    Song(std::string path);    

    bool loadSongFromDirectory();   // Loads all resources from a path to a driectory.
    bool saveSongToDirectory();     // Saves all the resources of the song to the stored path.

    void loadInfo(const std::string fileName);
    void saveInfo();

    Map getMap(const std::string name);
    void addMap(Map map);
    bool loadMap(const std::string fileName);
    bool saveMap(const std::string name);
    void removeMap(const std::string name);

    Notation getNotation(const std::string name);
    bool loadNotation(const std::string fileName);
    bool saveNotation(const std::string name);

    void addNotation(Notation notation);
    void removeNotation(const std::string name);
    
};

} // Namespace OK
