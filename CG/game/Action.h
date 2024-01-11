#pragma once
#include <string>


class Action {
public:
    int spellId;
    std::string str;

    Action() {
        this->str = "NO_ACTION";
    }

    std::string getStr() {
        return str;
    }

    bool isSpell() {
        return false;
    }

     bool isReset() {
        return false;
    }

    bool isWait() {
        return false;
    }

    int getRepeats() {
        return 1;
    }

};

class ResetAction : Action {

public:

    bool isReset() {
        return true;
    }
};

class SpellAction : Action {

public:

    int param;

    SpellAction(std::string str, int spellId, int param=1) {
        this->str = str;
        this->spellId = spellId;
        this->param = param;
    }

    bool isSpell() {
        return true;
    }

    int getRepeats() {
        return param;
    }

};

class WaitAction : Action {

    bool isWait() {
        return true;
    }
};
