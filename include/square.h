#ifndef SQUARE_H
#define SQUARE_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

class Square {

private:
    std::string name;
    std::map<char, float> position;
    std::map<char, float> topLeftCornerPosition;
    std::map<char, float> velocity;
    std::map<char, float> size;
    std::map<char, float> edges;
    float speed;
    sf::RectangleShape sprite;
    sf::Color spriteColor;

public:
    //Constructors
    Square(std::string id, std::map<char, float> pos, std::map<char, float> vel, std::map<char, float> s);
    ~Square();
    
    //Getters
    std::string getName();
    float getPosition(char coor);
    float getVelocity(char coor);
    float getSize(char coor);
    float getEdges(char edge);
    float getSpeed();
    void resetSpeed();

    sf::RectangleShape getSprite();

    //Setters
    void setPosition(char coor, float pos);
    void setVelocity(char coor, float vel);
    void setPosition(std::map<char, float> pos);
    void setVelocity(std::map<char, float> vel);
    void setSpeed(float spe);

    //Actions
    void updatePosition(float deltaTime);
    void flipVelocity(char coord);
    void normaliseVelocity();
    

};
#endif