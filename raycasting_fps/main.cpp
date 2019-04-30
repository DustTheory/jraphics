#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <math.h>

#include "gamemap.h"

Map gamemap;

sf::RenderWindow *window;
sf::Uint8 *pixels;
sf::Texture *display_texture;
sf::Sprite *display_sprite;

int W_WIDTH = 500, W_HEIGHT = 500;

struct ray_collision{
    float dist;
    uint8_t object;
};

struct Player{
    Point pos;
    float r;
    float dist(Point p){
        return sqrt((pos.x-p.x)*(pos.x-p.x) + (pos.y-p.y)*(pos.y-p.y));
    }
};

Player player;

const float deg90 = M_PI/2; 
const float deg180 = M_PI;
const float deg270 = deg90*3;
const float deg360 = 2*M_PI;

#define EPS 1e-9


ray_collision cast_ray(float x, float y, float r){
    if(abs(r) <= EPS || abs(r-deg180) < EPS || abs(r-deg90) <= EPS || abs(r-deg270) < EPS)
        r += 0.016;
    
    float a = x, b;
    float step = 0.1 * ((r < deg90 || r > deg270) ? 1 : -1);
    float k = atan(r), n = y - k*x;
    while(true){
        b = a*x+n;
        if(gamemap.out_of_bounds({a, b})){
            a-=step;
            b = a*x+n;
            return {player.dist({a, b}), gamemap.at_coord({a, b})};
        }
        if(gamemap.at_coord({a, b}) != empty)
            return {player.dist({a, b}), gamemap.at_coord({a, b})};
        a += step;
    }
}

void set_pixel(int a, int b, sf::Color c){
    pixels[(a*W_WIDTH+b)*4] = c.r;
    pixels[(a*W_WIDTH+b)*4+1] = c.g;
    pixels[(a*W_WIDTH+b)*4+2] = c.b;
    pixels[(a*W_WIDTH+b)*4+3] = c.a;
}

void drawline(int col, float h, uint8_t o){
    sf::Color c;
    if(o == 1)
        c = sf::Color::Blue;
    else if(o == 2)
        c = sf::Color::Red;
    else if(o == 3)
        c = sf::Color::Green;
    int h1 = (W_HEIGHT-(int)h)/2;
    for(int i = 0; i < h1; i++){
        set_pixel(i, col, sf::Color::White);
    }
    int h2 = h1 + h;
    for(int i = h1; i < h2; i++){
        set_pixel(i, col, c);
    }

    int h3 = W_HEIGHT;
    for(int i = h2; i < h3; i++){
        set_pixel(i, col, sf::Color::Yellow);
    }

}

uint8_t max(uint8_t a, uint8_t b){
    return a > b ? a : b;
}

float deg_to_rad(float deg){
    return deg*M_PI/180;
}

float rad_to_deg(float rad){
    return rad*180/M_PI;
}

void render_frame(){
    float depth_ratio = (float)W_HEIGHT/(float)max(gamemap.W, gamemap.H);
    float FOV = deg_to_rad(10); // 120 degree FOV
    float angle = player.r + FOV/2;
    while(angle > deg360)
        angle -= deg360;
    float step = FOV/(float)W_WIDTH;
    for(int i = 0; i < W_WIDTH; i++){
        ray_collision colision = cast_ray(player.pos.x, player.pos.y, angle);
        drawline(i, colision.dist*depth_ratio, colision.object);  
        angle -= step;
       // if(angle < 0){
       //     angle += deg360;
       // }
    }
   // printf("\n");
    display_texture->update(pixels);
    window->draw(*display_sprite);
}


void create_window(){
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "raycasting_fps");
    pixels = new sf::Uint8[W_WIDTH*W_HEIGHT*4];
    display_texture = new sf::Texture;
    display_texture->create(W_WIDTH, W_HEIGHT);
    display_sprite = new sf::Sprite(*display_texture);
}




int main(int argc, char* argv[]){
    printf("########## WELCOME! ##########\nraycasting fps game by ieexmml\n\n");


    printf("Enter map file relative path: \n");
    std::string filename;
    getline(std::cin, filename);
    gamemap.load_map(filename.c_str());
    Point spawnpoint = gamemap.get_spawnpoint();
    printf("SPAWN: %f %f\n", spawnpoint.x, spawnpoint.y);
    player = {spawnpoint.x, spawnpoint.y, 0};
    create_window();

    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
    int framecount = 0;
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        window->clear(sf::Color::Black);
        render_frame();
        framecount++;
        if(framecount == 30){
            framecount = 0;
            player.r+=0.3;
            printf("%f\n", rad_to_deg(player.r));
        }
        window->display();
    }
}