#include "view.h"

sf::View view;

void setplayercoordinateforview(float x, float y){
    float tempX = x;
    float tempY = y;
    view.setCenter(tempX,tempY);
}

void viewmap(float time){

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        view.move(0.2*time,0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        view.move(-0.2*time,0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        view.move(0,-0.2*time);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        view.move(0,0.2*time);
    }
}

void changeview(){

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::U)){
        view.zoom(1.0100f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
        view.rotate(1);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)){
        view.setSize(640,480);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
        view.setSize(540,380);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        view.setViewport(sf::FloatRect(0,0,0.5f,1));
    }
}
