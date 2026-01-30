#include "square.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
// Constructors
Square::Square(std::string id, std::map<char, float> pos, std::map<char, float> vel, std::map<char, float> s){
    name     = id;
    position = pos;
    lastPosition = pos;
    velocity = vel;
    size     = s;
    edges    = {{'R', position['X'] + size['X']},
                {'L', position['X']}, 
                {'B', position['Y'] + size['Y']},
                {'T', position['Y']}};

    speed = 2.0;
    sprite.setSize(sf::Vector2f(size['X'], size['Y']));
    sprite.setPosition(sf::Vector2f(position['X'], position['Y']));
    spriteColor = sf::Color::White;

}

Square::~Square(){}

// Getters
std::string Square::getName(){return name;}
float Square::getPosition(char coor){return position.at(coor);}
float Square::getVelocity(char coor){return velocity.at(coor);}
float Square::getSize(char coor){return size.at(coor);}
float Square::getEdges(char edge){return edges.at(edge);}
float Square::getSpeed(){return speed;}
sf::RectangleShape Square::getSprite(){return sprite;}

//Setters
void Square::setPosition(std::map<char, float> pos){
    position = pos;
    sprite.setPosition(sf::Vector2f(position['X'], position['Y']));
}

void Square::setVelocity(std::map<char, float> vel){
    velocity = vel;
}

void Square::setPosition(char coor, float pos){
    position[coor] = pos;
    sprite.setPosition(sf::Vector2f(position['X'], position['Y']));
}

void Square::setVelocity(char coor, float vel){velocity[coor] = vel;}
void Square::resetSpeed(){speed = 2.0;};
void Square::setSpeed(float spe){speed += spe;}
// Actions
void Square::updatePosition(float deltaTime){

    if (speed != 0){

        position['X'] += velocity['X'] * deltaTime;
        position['Y'] += velocity['Y'] * deltaTime;

        edges    = {{'R', position['X'] + size['X']},
                    {'L', position['X']}, 
                    {'B', position['Y'] + size['Y']},
                    {'T', position['Y']}};

        sprite.setPosition(sf::Vector2f(position['X'], position['Y']));
    }
}

void Square::flipVelocity(char coord){
    velocity.at(coord) = velocity.at(coord) * (-1);
}

void Square::normaliseVelocity(){

    if (speed != 0){
        float mag = sqrt(pow(velocity['X'],2) + pow(velocity['Y'],2)) / speed;
        setVelocity('X',velocity['X']/mag);
        setVelocity('Y',velocity['Y']/mag);
    } else {
        setVelocity('X',0);
        setVelocity('Y',0);
    }
    
};