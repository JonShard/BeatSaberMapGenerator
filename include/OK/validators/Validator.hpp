// Base class for all validators. A validator is a class that ensures the map meets some criteria.
#pragma once

#include "../Map.hpp"

namespace OK {

class Validator {
protected:
    static int s_passes;
    static int s_fails;
    
public:

    virtual bool validate(Map map) = 0;
};

} // namespace OK