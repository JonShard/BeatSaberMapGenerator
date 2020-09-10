#pragma once

#include "../Map.hpp"

namespace OK {

class Validator {
    static int s_passes;
    static int s_fails;
    virtual void loadConfig() = 0;
    virtual bool validate(Map map) = 0;
};

} // namespace OK