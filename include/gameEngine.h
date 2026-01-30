#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "square.h"


class gameEngine {

private:
    std::string gameTitle;
    sf::RenderWindow gameWindow;
    std::map<std::string, Square> objs;
    int gameState;
    float screenWidth;
    float screenHeight;
    float fps;
    float deltaTime;
    float coyoteSpace;
    int scoreA;
    int scoreB;
    

public:

    gameEngine();
    ~gameEngine();

    void mainGameLoop();

    bool intersects(const Square& a, const Square& b);
    float clamp(float value, float min, float max);
    void inputCheck();
    float ballPaddleCollision(std::string ballID, std::string paddleID);
    std::string screenEdgeCollision(std::string objID);
    void collisionCheck();


};

#endif