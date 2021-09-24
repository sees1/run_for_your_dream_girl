#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include "g_mission/mission.h"
#include "g_map/map.h"
#include "g_view/view.h"


using namespace sf;

enum class Dir{UP = 0,DOWN,LEFT,RIGHT};

class Player {
public:
    Sprite sprite;
    Player(String F,float X,float Y,float W,float H)
    :x(0),y(0),w(0),h(0),dx(0),dy(0),speed(0),playerScore(0),health(100),life(true)
    {
        File = F;
        w = W;
        h = H;
        image.loadFromFile("image/"+File);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X;
        y = Y;
        sprite.setTextureRect(IntRect(0,0,w,h));
    }


    void Update(float time)
    {
        if(health <= 0){life = false;speed = 0;health = 0;}
        switch(dir)
        {
            case Dir::LEFT : dx = -speed; dy = 0; break;
            case Dir::RIGHT : dx = speed; dy = 0; break;
            case Dir::DOWN : dx = 0; dy = speed; break;
            case Dir::UP : dx = 0; dy = -speed; break;
        }

        x+=dx*time;
        y+=dy*time;

        speed = 0;
        sprite.setPosition(x,y);
        interactionWithMap();
    }


    void SetDir(Dir d){
        dir = d;
    }
    void SetSpeed(float s){
        speed = s;
    }
    float GetplayercoordinateX(){
        return x;
    }
    float GetplayercoordinateY(){
        return y;
    }
    bool IsLife(){
        return life;
    }
    int GetHealth(){
        return health;
    }

    void interactionWithMap()
    {
        for(int i = y/32; i<(y+h)/32;i++)
        {
            for (int j = x/32;j<(x+w)/32;j++)
            {
                if(TileMap[i][j] == '0')
                {
                    if(dy > 0) y = i * 32 - h;
                    if(dy < 0) y = i * 32 + 32;
                    if(dx > 0) x = j * 32 - w;
                    if(dx < 0) x = j * 32 + 32;
                }

                if (TileMap[i][j] == 's')
                {
                    if(dy > 0) y = i * 32 - h;
                    if(dy < 0) y = i * 32 + 32;
                    if(dx > 0) x = j * 32 - w;
                    if(dx < 0) x = j * 32 + 32;
                }
                if (TileMap[i][j] == 'f')
                {
                    health-=40;
                    TileMap[i][j] = ' ';
                }
                if (TileMap[i][j] == 'h')
                {
                    health+=20;
                    TileMap[i][j] = ' ';
                }
            }
        }
    }

private:
    float x,y,w,h,dx,dy,speed;
    int playerScore,health;
    bool life;
    Dir dir;
    String File;
    Image image;
    Texture texture;
};






int main(){

    RenderWindow window(VideoMode(1366,768),"Run for your dream girl!");
    view.reset(sf::FloatRect(0,0,640,480));

    //Timers
    float CurrentFrame = 0;
    Clock clock;
    Clock gameTimeClock;
    gameTimeClock.restart();
    int gameTime = 0;

    //Text's
    sf::Font font;
    font.loadFromFile("image/CyrilicOld.TTF");
    sf::Text text("",font,20);
    text.setColor(sf::Color::Black);

    //Player's
    Player kara("mayu.png",32,32,32,48);

    //Map's
    Image map_image;
    map_image.loadFromFile("image/map.png");
    Texture map_texture;
    map_texture.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map_texture);
    randomMapGenerate();

    //Mission
    bool showMissionText = true;
    Image quest_image;
    quest_image.loadFromFile("image/missionbg.jpg");
    quest_image.createMaskFromColor(sf::Color(0,0,0));
    Texture quest_texture;
    quest_texture.loadFromImage(quest_image);
    Sprite s_quest;
    s_quest.setTexture(quest_texture);
    s_quest.setTextureRect(IntRect(0,0,340,510));
    s_quest.setScale(0.6f,0.6f);

    while(window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        if(kara.IsLife()) gameTime = gameTimeClock.getElapsedTime().asSeconds();

        clock.restart();
        time = time/800;
        Event event;

        while (window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();

            if(event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard::Tab)
                {
                    switch (showMissionText)
                    {
                        case true:
                        {
                            std::ostringstream playerHealthString,task;                            playerHealthString << kara.GetHealth();
                            task << getTextMission(
                                getCurrentMission(kara.GetplayercoordinateX()));
                            text.setString("Health:" + playerHealthString.str() + "\n" + task.str());
                            text.setPosition(view.getCenter().x + 115,view.getCenter().y - 130);
                            s_quest.setPosition(view.getCenter().x + 115,view.getCenter().y - 130);
                            showMissionText = false;
                            break;
                        }
                        case false:
                        {
                            text.setString("");
                            showMissionText = true;
                            break;
                        }
                    }
                }
            }

        }

        if(kara.IsLife()){
            if(Keyboard::isKeyPressed(Keyboard::Left)) {
                kara.SetDir(Dir::LEFT);
                kara.SetSpeed(0.1);
                CurrentFrame += 0.005*time;
                if(CurrentFrame > 4) CurrentFrame = 0;
                kara.sprite.setTextureRect(IntRect(32*static_cast<int>(CurrentFrame),48,32,48));
            }
            if(Keyboard::isKeyPressed(Keyboard::Right)) {
                kara.SetDir(Dir::RIGHT);
                kara.SetSpeed(0.1);
                CurrentFrame += 0.005*time;
                if(CurrentFrame > 4) CurrentFrame = 0;
                kara.sprite.setTextureRect(IntRect(32*static_cast<int>(CurrentFrame),96,32,48));
            }
            if(Keyboard::isKeyPressed(Keyboard::Up)) {
                kara.SetDir(Dir::UP);
                kara.SetSpeed(0.1);
                CurrentFrame += 0.005*time;
                if(CurrentFrame > 4) CurrentFrame = 0;
                kara.sprite.setTextureRect(IntRect(32*static_cast<int>(CurrentFrame),144,32,48));
            }
            if(Keyboard::isKeyPressed(Keyboard::Down)) {
                kara.SetDir(Dir::DOWN);
                kara.SetSpeed(0.1);
                CurrentFrame += 0.005*time;
                if(CurrentFrame > 4) CurrentFrame = 0;
                kara.sprite.setTextureRect(IntRect(32*static_cast<int>(CurrentFrame),0,32,48));
            }
        }
        sf::Vector2i localPosition = Mouse::getPosition(window);
        if(localPosition.x<6) view.move(-0.2*time,0);
        if(localPosition.x>window.getSize().x-3) view.move(0.2*time,0);
        if(localPosition.y<6) view.move(0,-0.2*time);
        if(localPosition.y>window.getSize().y-3) view.move(0,0.2*time);


        kara.Update(time);

        viewmap(time);
        changeview();
        window.setView(view);

        window.clear();
        for(int i = 0;i < HEIGHT_MAP;i++)
        {
            for(int j = 0;j < WIDTH_MAP;j++)
            {
                if(TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0,0,32,32));
                if(TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32,0,32,32));
                if(TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64,0,32,32));
                if(TileMap[i][j] == 'f') s_map.setTextureRect(IntRect(96,0,32,32));
                if(TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(128,0,32,32));

                s_map.setPosition(j*32,i*32);
                window.draw(s_map);
            }
        }


        window.draw(kara.sprite);
        if(!showMissionText) {
            text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
            s_quest.setPosition(view.getCenter().x + 115,view.getCenter().y - 130);
            window.draw(s_quest);window.draw(text);
        }
        window.display();
    }

    return 0;
}
