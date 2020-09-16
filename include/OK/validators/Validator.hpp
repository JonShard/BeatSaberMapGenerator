// Base class for all validators. A validator is a class that ensures the map meets some criteria.
#pragma once

#include "../Map.hpp"

namespace OK {

class Validator {
protected:
    static int s_passes;
    static int s_fails;
    
    static float s_validateTimeAfterNote;    // Time after a note in which the validator will care if an illegal note is placed (sec).

public:
    static void LoadConfig() {
        s_validateTimeAfterNote = 3;
    };
    virtual void loadConfig() = 0;
    virtual bool validate(Map map) = 0;
};

} // namespace OK