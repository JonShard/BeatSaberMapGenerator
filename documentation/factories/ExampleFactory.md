# ExampleFactory
A factory is an object that produces some amount of notes. At least one facory must always be enabled.

## Configuration
Each factory has a section in the configuration where they can be enabled/disabled and provide the factory with attributes that modify it's behaviour. The amount of notes a factory is able to produce is **not** configurable. 
The configuration is loaded into the static Config class:
```cpp
struct ExampleFactoryConfig {
    bool enabled;
    bool someBoolAttribute; // Explanation.
    float someFloatAttribute; // Explanation.
    int someIntAttribute; // Explanation.
};
```
The factory's section in the config file:
```json
"exampleFactory": {
    "enabled": true,
    "someBoolAttribute": true,
    "someFloatAttribute": 0.35,
    "someIntAttribute": 2,
},
```

```cpp
#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// ExampleFactory produces some notes, usually only a spesific kind of pattern of notes.
class ExampleFactory : public Factory {
public:
    ExampleFactory() {
        if (Config::generator.factory.example.someFloatAttribute does not satisfy some restriction) {
            printf("\nWarning: some property is misconfigured\n\n");
        }
        if (Config::generator.factory.example.someIntAttribute does not satisfy some restriction) {
            printf("\nWarning: some property is misconfigured\n\n");
        }
    }

    virtual std::string getName() { return "ExampleFactory"; }

    // Define for which amounts of notes the factory can produce.
    virtual bool canProduceAmount(int amount) { return (amount == 2); }

    virtual std::vector<Note> produce(Notation notation, Map map, int amount) {
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());
        std::vector<Note> notes;

        for (int i = 0; i < amount; i++) {
            Note note;
            note.m_parentFactory = getName();
            note.m_time= nextKeyframe.time;
            
            // Generate the note:
            note.m_type = some value;
            note.m_lineIndex = some value;
            note.m_lineLayer = some value;
            note.m_cutDirection = some value;

            notes.push_back(note);
        }
        return notes;
    }
};

} // namespace OK
```