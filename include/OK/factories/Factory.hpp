#pragma once

#include "../Map.hpp"
#include "../Notation.hpp"

namespace OK {

// Base class for all factories. A factory is a class that procduces one or several notes.
class Factory {
protected:
    static unsigned long s_totalProduceAttempts;
    
public:
    static unsigned long getTotalProduceAttempts() {
        return s_totalProduceAttempts;
    }
    virtual std::vector<Note> produce(Notation notation, Map map) = 0;
};

} // namespace OK