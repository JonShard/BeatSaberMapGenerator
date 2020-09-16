// Base class for all factories. A factory is a class that procduces one or several notes.
#pragma once

#include "../Map.hpp"
#include "../Notation.hpp"

namespace OK {

class Factory {
protected:
    static int s_totalProduceAttempts;
    
    int static  cfg_maxAttempts;
public:
    static void LoadConfig() {
        cfg_maxAttempts = 10000;
    };
    virtual void loadConfig() = 0;
    virtual std::vector<Note> produce(Notation notation, Map map) = 0;
};

} // namespace OK