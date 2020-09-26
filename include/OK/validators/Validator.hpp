#pragma once

#include "../Map.hpp"

namespace OK {

// Base class for all validators. A validator is a class that ensures the map meets some criteria.
class Validator {
protected:
    static unsigned long s_passes;
    static unsigned long s_fails;
    
public:
    static unsigned long getTotalPasses() {
        return s_passes;
    }
    static unsigned long getTotalFails() {
        return s_fails;
    }

    virtual bool validate(Map map) = 0;
};

} // namespace OK