#pragma once

#include "Recipe.h"
#include "Player.h"

enum class SpellType {
    CAST,
    OPPONENT_CAST,
    LEARN,
    BREW
};

static int INSTANCE_COUNT = 0;

class Spell {

public:
    Recipe recipe;
    int id = INSTANCE_COUNT++;

    int getId() { return id; }
    int getScore() { return 0; }
    int getStock() { return -1; }
    bool isActive() { return false; }
    bool isOwner(Player* player) { return false; }
    int* getDelta() { return recipe.delta; }
    bool isRepeatable() { return false; }
    std::string toString() { return std::to_string(id); }
};

class DeliverySpell : Spell {

public:    
    int score;

    DeliverySpell(Recipe need, int score) {
        this->recipe = Recipe( -need.delta[0], -need.delta[1], -need.delta[2], -need.delta[3] );
        this->score = score;
    }

    int getScore() {
        return score;
    }

};

class TomeSpell : Spell {

public:
    int stock;
    bool repeatable;

    TomeSpell(Recipe recipe) {
        stock = 0;
        this->recipe = recipe;
        repeatable = false;
        for (auto it = std::begin(recipe.delta); it!=std::end(recipe.delta); ++it) {
            if (*it < -1) { repeatable = true; }
        }
    }

    int getStock() {
        return stock;
    }

    bool isRepeatable() {
        return repeatable;
    }
};


class PlayerSpell : Spell {

public:
    bool active;
    bool repeatable;
    Player* owner;

    PlayerSpell(Recipe recipe, Player* owner, bool repeatable) {
        this->recipe = recipe;
        this->active = true;
        this->owner = owner;
        this->repeatable = repeatable;
    }

    PlayerSpell(Recipe recipe, Player* owner) {
        PlayerSpell(recipe, owner, false);
    }

    PlayerSpell(TomeSpell* learnt, Player* owner) {
        PlayerSpell(Recipe(learnt->recipe), owner, learnt->isRepeatable());
    }

    bool isActive() {
        return active;
    }

    bool isOwner(Player* player) {
        return player == owner;
    }

    bool isRepeatable() {
        return repeatable;
    }

    void activate() {
        active = true;
    }

    void deactivate() {
        active = false;
    }
};
