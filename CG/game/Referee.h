#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>

#include "Game.h"
#include "Player.h"
#include "Spell.h"

class Referee {

    int MAX_TURNS = 100;
    std::vector<Player> players;
    Game game;
    long seed;
    int maxFrames;

    void init() {
        
        seed = static_cast<long>(rand());

        maxFrames = MAX_TURNS;

        try {

            game.init(seed, players);
            sendGlobalInfo();

        } catch (const std::exception& e) {
            
            std::cerr << "Referee failed to initialize" << std::endl;
            abort();
        }
    }

    void abort() {
        std::cerr << "Unexpected game end" << std::endl;
    }

    void sendGlobalInfo() {
        for (Player player : players) {
            for (std::string line : game.getGlobalInfoFor(player)) {
                player.sendInputLine(line);
            }
        }
    }

    void gameTurn(int turn) {
        game.resetGameTurnData();

        // Give input to players
        for (Player player : players) {
            for (std::string line : game.getCurrentFrameInfoFor(player)) {
                player.sendInputLine(line);
            }
            player.execute();
        }
        // Get output from players
        handlePlayerCommands();

        game.performGameUpdate();
    }

    void handlePlayerCommands() {
        for (Player player : players) {
            // TODO Plugg the bot execution and timeout management
            /*
            try {
                commandManager.parseCommands(player, player.getOutputs(), game);
            } catch (TimeoutException e) {
                commandManager.deactivatePlayer(player, "Timeout!");
                gameSummaryManager.addPlayerTimeout(player);
                gameSummaryManager.addPlayerDisqualified(player);
            }
            */
        }

    }

    void onEnd() {
        
        game.onEnd();
        // TODO manage return  of info when game is finished
        /*
        int scores[] = gameManager.getPlayers().stream()
            .mapToInt(Player::getScore)
            .toArray();

        eem.setScores(scores);
        */
    }

};
