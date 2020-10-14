#pragma once

#include "../Map.hpp"

namespace OK {

// Base class for all validators. A validator is a class that ensures the map meets some criteria.
class Validator {
protected:
    static unsigned long s_totalPasses;
    static unsigned long s_totalFails;
    unsigned long m_passes;
    unsigned long m_fails;

public:
    static unsigned long getTotalPasses() {
        return s_totalPasses;
    }
    static unsigned long getTotalFails() {
        return s_totalFails;
    }

    virtual std::string getName() = 0;
    virtual bool validate(Map map) = 0;
    void printReport() {
        float failRatio = m_fails / (float)Validator::s_totalFails;
        float passRatio = m_passes / (float)Validator::s_totalPasses;
        printf("%s:\tPass ratio: %f\t Fail ratio: %f\tPasses: %ld\t\tFails: %ld\n", 
            getName().data(), passRatio, failRatio, m_passes, m_fails);

    }
};

} // namespace OK