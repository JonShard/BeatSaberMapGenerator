#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

enum SymmertyMode { CENTER_POINT, HORIZONTAL, VERTICAL };

// SymmetricalFactory generates a pair of a blue and a red note in symmery. Symmetrical either around a point, or to a line, horizontal or vertical.
class SymmetricalFactory : public Factory {
public:

    SymmetricalFactory() {
        if (Config::generator.factory.symmetrical.centerPointMode == false  &&
            Config::generator.factory.symmetrical.horizontalMode == false  &&
            Config::generator.factory.symmetrical.verticalMode == false) {
            printf("Warning: SymmetricalFactory is enabled but has no modes enabled. It will not generate any notes.\n");
        }
    }

    virtual std::string getName() { return "SymmetricalFactory"; }

    virtual bool canProduceAmount(int amount) { return (amount == 2); }

    virtual Cluster produce(Notation notation, Map map, int amount) {        
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());
        Cluster cluster;

        if (Config::generator.factory.symmetrical.centerPointMode == false  &&
            Config::generator.factory.symmetrical.horizontalMode == false  &&
            Config::generator.factory.symmetrical.verticalMode == false) {
            return cluster;
        }

        Note noteBlue;
        noteBlue.m_parentFactory = getName();
        noteBlue.m_time = nextKeyframe.time;
        noteBlue.m_type = BLUE;
        noteBlue.m_cutDirection = (CutDirection)Util::rng(0, 8);
        noteBlue.m_lineLayer = Util::rng(0, 3);
        
        // Blue is more likely to be on the right.
        int number = Util::rng(0, 100);
        if      (number < 40) noteBlue.m_lineIndex = 3; 
        else if (number < 80) noteBlue.m_lineIndex = 2;
        else if (number < 90) noteBlue.m_lineIndex = 1;
        else noteBlue.m_lineIndex = 0;

        if (noteBlue.isInCenter()) {
            if (!Config::generator.factory.symmetrical.allowNotesInCenter) {
                if (noteBlue.m_lineIndex == 1) {
                    noteBlue.m_lineIndex--;
                }
                else if (noteBlue.m_lineIndex == 2){
                    noteBlue.m_lineIndex++;
                }
            }
            else if ((noteBlue.m_lineIndex == 1 || noteBlue.m_lineIndex == 2) 
                    && (noteBlue.m_cutDirection == RIGHT || noteBlue.m_cutDirection == LEFT)) { // Clap not allowed.
                while (noteBlue.m_cutDirection == RIGHT || noteBlue.m_cutDirection == LEFT) {
                    noteBlue.m_cutDirection = (CutDirection)Util::rng(0, 8);
                }
            }
        }
        
        Note noteRed = noteBlue;
        while (true)
        {
            int mode = Util::rng(0, 3);
            if (mode == CENTER_POINT && Config::generator.factory.symmetrical.centerPointMode) {
                noteRed.invertNote();
                noteRed.invertPosition();
                break;
            }
            else if (mode == HORIZONTAL && Config::generator.factory.symmetrical.horizontalMode) {
                while (noteBlue.isVertical()) { // Ensure horizontal.
                    noteBlue.m_cutDirection = (CutDirection)Util::rng(0, 8);
                }
                if (noteBlue.m_lineLayer == 1) { // If in center, move one up or down.
                    noteBlue.m_lineLayer += (Util::cointoss()) ? 1 : -1;
                }
                noteRed = noteBlue;
                noteRed.invertColor();
                noteRed.invertVertical();
                break;
            } 
            else if (mode == VERTICAL && Config::generator.factory.symmetrical.verticalMode) {
                if (!Config::generator.factory.symmetrical.allowOffsetPlane) { // If offset not allowed, blue note is one one of the horizontal edges.
                    noteBlue.m_lineIndex = (Util::cointoss()) ? 0 : 3;
                }
                if (std::abs(noteBlue.m_lineIndex - noteRed.m_lineIndex) < 2) { // If both notes not on horizontal edges, ensure they are not horizontal.
                    while (noteBlue.isHorizontal()) {                           // It's very hard to get a straight hit on the block then. Example: [<]_[>]_
                        noteBlue.m_cutDirection = (CutDirection)Util::rng(0, 8);
                    }
                }
                noteRed = noteBlue;
                noteRed.invertColor();
                noteRed.invertHorizontal();
                break;
            }
        }

        cluster += noteBlue;
        cluster += noteRed;
        return cluster;
    }
};

} // namespace OK