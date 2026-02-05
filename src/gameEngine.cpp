#include "gameEngine.h"
#include "square.h"
#include "fontPixelZone.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <map>

// Constructors
gameEngine::gameEngine(){

    gameTitle = "PONG";

    screenWidth  = 480.0;
    screenHeight = 270.0;

    screenCentre = {{'X', screenWidth/2},{'Y',screenHeight/2}};
    
    fps          = 60.0;
    deltaTime    = 1/fps;
    coyoteSpace  = 5.0;
    scoreA = 0;
    scoreB = 0;
    gameState = 0;

    gameWindow.create(sf::VideoMode(sf::Vector2u(screenWidth, screenHeight)),gameTitle,sf::Style::None);

    objs = { 
             {"ball",    Square("ball",    screenCentre, {{'X',1.0},{'Y',0.0}}, {{'X',6.0},{'Y',6.0}})},
             {"paddleA", Square("paddleA", {{'X', 6}, {'Y',117}},{{'X',0.0},{'Y',0.0}}, {{'X',6.0},{'Y',36.0}} )},
             {"paddleB", Square("paddleB", {{'X', 468},{'Y',117}},{{'X',0.0},{'Y',0.0}}, {{'X',6.0},{'Y',36.0}} )}   
           };

}

gameEngine::~gameEngine(){}

void gameEngine::inputCheck(){

    objs.at("paddleA").setVelocity({{'X',0.0},{'Y',0.0}});
    objs.at("paddleB").setVelocity({{'X',0.0},{'Y',0.0}});

    //Quit controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){gameWindow.close();}

    //Paddle A controls
    if ((screenEdgeCollision("paddleA") != "T") and (ballPaddleCollision("ball", "paddleA") == -1)){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){objs.at("paddleA").setVelocity({{'X',0.0},{'Y',-2.0}});}
    }

    if ((screenEdgeCollision("paddleA") != "B") and (ballPaddleCollision("ball", "paddleA") == -1)){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){objs.at("paddleA").setVelocity({{'X',0.0},{'Y', 2.0}});}
    }

    //Paddle B controls
    if ((screenEdgeCollision("paddleB") != "T") and (ballPaddleCollision("ball", "paddleB") == -1)){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)){objs.at("paddleB").setVelocity({{'X',0.0},{'Y',-2.0}});} 
    }

    if ((screenEdgeCollision("paddleB") != "B") and (ballPaddleCollision("ball", "paddleB") == -1)){
        if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L) ){objs.at("paddleB").setVelocity({{'X',0.0},{'Y', 2.0}});}
    }
}

float gameEngine::ballPaddleCollision(std::string ballID, std::string paddleID) {

    float collision = -1.0;

    float lefEdgePaddle = objs.at(paddleID).getEdges('L');
    float rigEdgePaddle = objs.at(paddleID).getEdges('R');
    float topEdgePaddle = objs.at(paddleID).getEdges('T');
    float botEdgePaddle = objs.at(paddleID).getEdges('B');

    float lefEdgeBall = objs.at(ballID).getEdges('L');
    float rigEdgeBall = objs.at(ballID).getEdges('R');
    float topEdgeBall = objs.at(ballID).getEdges('T');
    float botEdgeBall = objs.at(ballID).getEdges('B');

    if (paddleID == "paddleA"){
        if ((lefEdgeBall < rigEdgePaddle) and (((topEdgePaddle-coyoteSpace) < topEdgeBall) and ((botEdgePaddle + coyoteSpace) > botEdgeBall))){
            collision =  topEdgeBall - (topEdgePaddle-coyoteSpace);
        }
    }

    if (paddleID == "paddleB"){
        if ((rigEdgeBall > lefEdgePaddle) and (((topEdgePaddle-coyoteSpace) < topEdgeBall) and ((botEdgePaddle+coyoteSpace) > botEdgeBall))) {
            collision =  topEdgeBall - (topEdgePaddle-coyoteSpace);
        }
    }

    return collision;
}

std::string gameEngine::screenEdgeCollision(std::string objID){
    
    std::string collision = "";

    float topEdge = objs.at(objID).getEdges('T');
    float botEdge = objs.at(objID).getEdges('B');
    float lefEdge = objs.at(objID).getEdges('L');
    float rigEdge = objs.at(objID).getEdges('R');

    if (topEdge < 0){collision += "T";}
    if (botEdge > screenHeight){collision += "B";}
    if (lefEdge < 0){collision += "L";}
    if (rigEdge > screenWidth){collision += "R";}

    return collision;
}
    
void gameEngine::collisionCheck(){

    //ball walls collision
    if ((screenEdgeCollision("ball") == "T") or (screenEdgeCollision("ball") == "B")){
        objs.at("ball").flipVelocity('Y');
        return;
    }

    if (screenEdgeCollision("ball") == "L"){
        scoreB += 1;
        objs.at("ball").flipVelocity('X');
        return;
    }

    if (screenEdgeCollision("ball") == "R"){
        scoreA += 1;
        objs.at("ball").flipVelocity('X');
        return;
    }

    if (screenEdgeCollision("ball").length() > 1){
        objs.at("ball").flipVelocity('X');
        objs.at("ball").flipVelocity('Y');
        return;
    }

    float ballPaddleCollisionLocationA = ballPaddleCollision("ball", "paddleA");
    //ball paddle collision
    if (ballPaddleCollisionLocationA != -1.0){
        objs.at("ball").setSpeed(0.02);
        objs.at("ball").flipVelocity('X');
        objs.at("ball").setVelocity('Y', (ballPaddleCollisionLocationA - 18)/10);
        return;
    }

    float ballPaddleCollisionLocationB = ballPaddleCollision("ball", "paddleB");
    if (ballPaddleCollisionLocationB != -1.0){
        objs.at("ball").setSpeed(0.02);
        objs.at("ball").flipVelocity('X');
        objs.at("ball").setVelocity('Y', (ballPaddleCollisionLocationB - 18)/10);
        return;
    }

    // paddle wall collision
    if (screenEdgeCollision("paddleA") == "T"){
        objs.at("paddleA").setVelocity({{'X',0.0},{'Y',0.0}});
        return;
    } else if (screenEdgeCollision("paddleA") == "B"){
        objs.at("paddleA").setVelocity({{'X',0.0},{'Y',0.0}});
        return;
    }

    if (screenEdgeCollision("paddleB") == "T"){
        objs.at("paddleB").setVelocity({{'X',0.0},{'Y',0.0}});
        return;
    } else if (screenEdgeCollision("paddleB") == "B"){
        objs.at("paddleB").setVelocity({{'X',0.0},{'Y',0.0}});
        return;
    }

}

float gameEngine::clamp(float value, float min, float max){
    if (value < min){return min;}
    if (value > max){return max;}
    return value;
}

void gameEngine::mainGameLoop(){

    sf::Clock deltaClock;
    std::array line = {sf::Vertex{sf::Vector2f(240.f, 0.f)}, sf::Vertex{sf::Vector2f(240.f, 270.f)}};
    int gameCounter = 0;

    sf::Font font(Pixelzone_ttf, Pixelzone_ttf_len);
    sf::Text titleText(font);
    sf::Text scoreAText(font);
    sf::Text scoreBText(font);
    sf::Text tutText(font);

    titleText.setCharacterSize(24*3);
    scoreAText.setCharacterSize(36);
    scoreBText.setCharacterSize(36);
    tutText.setCharacterSize(18);

    titleText.setString("PONG");
    titleText.setPosition(sf::Vector2f(205, 40));

    tutText.setString("Space to Start and Reset\nWS for left paddle\nOL for right paddle\nQ to quit");
    tutText.setPosition(sf::Vector2f(195, 150));
    
    float deltaFrameTime = 0.0;
    float ballResetDirection = 1.0;

    gameWindow.display();


    while (gameWindow.isOpen()) {

        gameWindow.display();

        while (auto event = gameWindow.pollEvent()){ if (event->is<sf::Event::Closed>()){gameWindow.close();}}
        gameWindow.clear();
        gameCounter++;

        switch (gameState){

            case 0:

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
                    gameState=1;
                    objs.at("ball").resetSpeed();
                    objs.at("ball").setVelocity({{'X',ballResetDirection},{'Y',0.0}});
                }


                gameWindow.draw(titleText);
                gameWindow.draw(tutText);
                gameWindow.draw(objs.at("ball").getSprite());
                gameWindow.draw(objs.at("paddleA").getSprite());
                gameWindow.draw(objs.at("paddleB").getSprite());
                
                if ((gameCounter % 10) == 0){inputCheck();}
                objs.at("paddleA").updatePosition(deltaTime);
                objs.at("paddleB").updatePosition(deltaTime);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){gameWindow.close();}

                break;

            case 1:
                
                if ((screenEdgeCollision("ball") == "L") or (screenEdgeCollision("ball") == "R")){
                    
                    objs.at("ball").resetSpeed();
                    if (screenEdgeCollision("ball") == "L"){
                        ballResetDirection = 1;
                        scoreB++;
                    }
                    if (screenEdgeCollision("ball") == "R"){
                        ballResetDirection = (-1);
                        scoreA++;
                    }

                    objs.at("ball").setPosition({{'X', 237},{'Y',132}});
                    gameState = 2;
                }

                
                gameWindow.clear();
                //Check for inputs
                if ((gameCounter % 10) == 0){inputCheck();}
                
                //Update objects
                objs.at("ball").updatePosition(deltaTime);
                objs.at("paddleA").updatePosition(deltaTime);
                objs.at("paddleB").updatePosition(deltaTime);
                
                if ((gameCounter % 10) == 0){collisionCheck();}

                objs.at("ball").normaliseVelocity();

                gameWindow.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
                gameWindow.draw(objs.at("ball").getSprite());
                gameWindow.draw(objs.at("paddleA").getSprite());
                gameWindow.draw(objs.at("paddleB").getSprite());
                
                scoreAText.setString(std::to_string(scoreA));
                scoreAText.setPosition(sf::Vector2f(120, 10));
                scoreBText.setString(std::to_string(scoreB));
                scoreBText.setPosition(sf::Vector2f(360, 10));

                gameWindow.draw(scoreAText);
                gameWindow.draw(scoreBText);
                break;

            case 2:
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
                    gameState=1;
                    objs.at("ball").setVelocity({{'X',ballResetDirection},{'Y',0.0}});
                }

                gameWindow.clear();

                gameWindow.draw(objs.at("ball").getSprite());
                gameWindow.draw(objs.at("paddleA").getSprite());
                gameWindow.draw(objs.at("paddleB").getSprite());
                gameWindow.draw(scoreAText);
                gameWindow.draw(scoreBText);

                if ((gameCounter % 10) == 0){inputCheck();}
                objs.at("paddleA").updatePosition(deltaTime);
                objs.at("paddleB").updatePosition(deltaTime);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
                    gameWindow.close();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)){
                    gameState=0;
                    ballResetDirection = 1.0;
                    scoreA=0;
                    scoreB=0;
                }

                break;

        }
    }
}