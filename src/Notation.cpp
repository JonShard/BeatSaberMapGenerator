#include "../include/OK/Notation.hpp"

namespace OK {

Notation::Notation() {
    m_name = "";
    std::vector<Keyframe> m_keyframes;
}

Notation::Notation(const std::string name) {
    m_name = name;
    std::vector<Keyframe> m_keyframes;
}

bool Notation::load(const std::string fileName) {
    std::ifstream file(fileName);
    if (file.fail()) {
        return false;
    }
    nlohmann::json jsNotation;
    file >> jsNotation;

    for (nlohmann::json jn : jsNotation) {
        Keyframe k {
            jn.at("id").get_to(k.id),
            jn.at("time").get_to(k.time),
            jn.at("concurrent").get_to(k.concurrent),
        };
        m_keyframes.push_back(k);
    }
    return true;
}

void Notation::save() {
    printf("Saving Notation with keyframes: %ld\n", m_keyframes.size());

    nlohmann::json jsKeyframes;
    
    for (Keyframe k : m_keyframes) {
        printf("Adding keyframe at time (real time in audio file): %f\n", k.time);
        nlohmann::json jn;
        jn["id"] = k.id;
        jn["time"] = k.time;
        jn["concurrent"] = k.concurrent;
        jsKeyframes.push_back(jn);
    }
    printf("Saving notation file to: %s\n", m_name.data());
    std::ofstream out(m_name);
    out << jsKeyframes;
    out.close();
}

void Notation::print() {
    printf("Notation: %s\n", m_name.data());
    for (Keyframe k : m_keyframes) {
        printf("Keyframe: id: %ld time: %f\t concurrent: %d\t\n", k.id, k.time, k.concurrent);
    }
}

std::string Notation::getName() { return m_name; }

Keyframe Notation::getNextKeyframe(float time) {
    for (Keyframe k : m_keyframes)
    {
        if (k.time > time) 
            return k;
    }
    return Keyframe();   
}

Notation Notation::operator+=(Keyframe k) {
    m_keyframes.push_back(k);
    return *this;
}

Notation Notation::operator+=(std::vector<Keyframe> keyframes) {
    for (Keyframe k : keyframes) {
        m_keyframes.push_back(k);
    }
    return *this;
}

} // Namespace OK