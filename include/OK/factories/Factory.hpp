#pragma once

#include "../Map.hpp"

namespace OK {

class Factory {
protected:
    static int s_totalProduceAttempts;
    int maxAttempts;
    int attempts;
    virtual void loadConfig() = 0;
public:
    virtual std::vector<Note> produce(std::vector<Notation> notations, Map map) = 0;
};

} // namespace OK