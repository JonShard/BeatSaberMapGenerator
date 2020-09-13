// Base class for all factories. A factory is a class that procduces one or several notes.
#pragma once

#include "../Map.hpp"
#include "../Notation.hpp"

namespace OK {

class Factory {
protected:
    static int s_totalProduceAttempts;
    int maxAttempts;
    int attempts;
    virtual void loadConfig() = 0;
public:
    virtual std::vector<Note> produce(std::vector<Keyframe> keyframes, Map map) = 0;
};

} // namespace OK