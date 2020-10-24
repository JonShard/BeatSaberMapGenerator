# ExampleValidator
A validator is a an object that is given a map and verify that a single spesific rule is followed throughout the map. Validators can be disabled or configured to change the behaviour of the generator. Disableing vaidators will degrade the playablility of the maps generatred.
```cpp
#pragma once

#include "Validator.hpp"
#include "../Config.hpp"

namespace OK {

// ExampleValidator ensures that some condition is met in the map.
class ExampleValidator : public Validator {
public:
    ExampleValidator() {
        m_passes = 0;
        m_fails = 0;
        // Make sure the validator has a valid configuration
        if (Config::generator.validator.example.someProperty > someRequirement) {
            printf("\nWarning: some property is misconfigured\n\n");
        }
    }

    virtual std::string getName() { return "ExampleValidator"; }

    virtual bool validate(Map map) {
        for (int i = map.m_notes.size(); i > 0; i--) {
            Note current = map.m_notes[i];
            if (current does not qualify to be validated) {
                continue;
            }

            if (current note does not satisfy some rule){
                m_fails++;
                Validator::s_totalFails++;
                return false;
            }
        }
        m_passes++;
        Validator::s_totalPasses++;
        return true;
    }
};

} // Namespace OK
```