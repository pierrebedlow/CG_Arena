#pragma once
#include <string>
#include <vector>

#include "Recipe.h"
#include "Player.h"
#include "Spell.h"
#include "Action.h"
#include "Game.h"

class Player {

    public:
    Recipe inventory;
    std::vector<PlayerSpell> spells;
    Action action;
    int deliveriesCount = 0;
    std::string message;

    Player() {
    }

    int getExpectedOutputLines() {
        return 1;
    }

    void initSpells() {
        spells.add(new PlayerSpell(new Recipe(2, 0, 0, 0), this));
        spells.add(new PlayerSpell(new Recipe(-1, 1, 0, 0), this));
        spells.add(new PlayerSpell(new Recipe(0, -1, 1, 0), this));
        spells.add(new PlayerSpell(new Recipe(0, 0, -1, 1), this));
    }

    void reset() {
        setAction(Action.NO_ACTION);
        message = null;
    }

    std::string getMessage() {
        return message;
    }

    bool canAfford(Recipe recipe, int repeats) {
        for (int i = 0; i < Game.INGREDIENT_TYPE_COUNT; ++i) {
            if (getInventory().delta[i] + recipe.delta[i] * repeats < 0) {
                return false;
            }
        }
        return true;
    }

    bool enoughSpace(Recipe recipe, int repeats) {
        return recipe.getTotal() * repeats + getInventory().getTotal() <= Game.MAX_SPACE;
    }

    bool canDeliver(Recipe need) {
        for (int i = 0; i < Game.INGREDIENT_TYPE_COUNT; ++i) {
            if (getInventory().delta[i] + need.delta[i] < 0) {
                return false;
            }
        }
        return true;
    }

    void addScore(int n) {
        setScore(getScore() + n);
    }

    void addDelivery() {
        this.deliveriesCount++;
    }

    int getDeliveriesCount() {
        return this.deliveriesCount;
    }

    Recipe getInventory() {
        return inventory;
    }

    List<PlayerSpell> getSpells() {
        return spells;
    }

    Action getAction() {
        return action;
    }

    void setAction(Action action) {
        this.action = action;
    }

    int[] getDelta() {
        return inventory.delta;
    }

    public void setMessage(String message) {
        this.message = message;
    }
};
