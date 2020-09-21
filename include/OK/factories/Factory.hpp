#pragma once

#include "../Map.hpp"
#include "../Notation.hpp"

namespace OK {

// Base class for all factories. A factory is a class that procduces one or several notes.
class Factory {
protected:
    static int s_totalProduceAttempts;
    
public:
    virtual std::vector<Note> produce(Notation notation, Map map) = 0;
};

} // namespace OK