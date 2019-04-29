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

int W_WIDTH = 200, W_HEIGHT = 200;

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
    if(abs(r) <= EPS || abs(r-deg180) < EPS){
        int a = floor(x), b = floor(y);
        int dir = r < 1 ? 1 : -1;
        while(true){
            a+=dir;
            if(gamemap.out_of_bounds({(float)a, (float)b}))
                return {player.dist({(float)(a-dir), (float)b}), gamemap.at_coord({(float)(a-dir), (float)b})};
            if(gamemap.at_coord({(float)a, (float)b}) != empty)
                return {player.dist({(float)a, (float)b}), gamemap.at_coord({(float)a, (float)b})};
        }
    }
    if(abs(r-deg90) <= EPS || abs(r-deg270) < EPS){
        int a = floor(x), b = floor(y);
        int dir = abs(r-deg90) <= EPS ? 1 : -1;
        while(true){
            b+=dir;
            if(gamemap.out_of_bounds({(float)a, (float)b}))
                return {player.dist({(float)(a), (float)(b-dir)}), gamemap.at_coord({(float)(a), (float)(b-dir)})};
            if(gamemap.at_coord({(float)a, (float)b}) != empty)
                return {player.dist({(float)a, (float)b}), gamemap.at_coord({(float)a, (float)b})};
        }
    }
    int k = atan(r); // slope of the ray
    int n = y - k*x; // y = k*x + n
    int dir_a, dir_b;
    if(r < deg90 || r > deg270)
        dir_a = 1;
    else 
        dir_a = -1;
    if(r < deg180)
        dir_b = 1;
    else
        dir_a = -1;
    int a = floor(x+dir_a), b = floor(y+dir_b);
    float tmp;
    while(true){
        tmp = abs((a*k + n) - y);
        if(floor(tmp+EPS) == b){
            a+=dir_a;
            if(gamemap.out_of_bounds({(float)a, (float)b}))
                return {player.dist({(float)(a-dir_a), (float)b}), gamemap.at_coord({(float)(a-dir_a), (float)b})};
            if(gamemap.at_coord({(float)a, (float)b}) != empty)
                return {player.dist({(float)a, (float)b}), gamemap.at_coord({(float)a, (float)b})};
            
        }else{
            b+=dir_b;
            if(gamemap.out_of_bounds({(float)a, (float)b}))
                return {player.dist({(float)a, (float)(b-dir_b)}), gamemap.at_coord({(float)a, (float)(b-dir_b)})};
            if(gamemap.at_coord({(float)a, (float)b}) != empty)
                return {player.dist({(float)a, (float)b}), gamemap.at_coord({(float)a, (float)b})};
        }
    }
}

void drawline(int col, float h, uint8_t o){
    int screenarea = (W_HEIGHT-1)*(W_WIDTH-1);
    int h1 = (W_HEIGHT-h)/2;
    for(int i = 0; i < h1; i++){
        pixels[(i*W_WIDTH+col)*4] = 0;
        pixels[(i*W_WIDTH+col)*4+1] = 0;
        pixels[(i*W_WIDTH+col)*4+2] = 0;
        pixels[(i*W_WIDTH+col)*4+3] = 255;
    }
    int h2 = h1 + h;
    for(int i = h1; i < h2; i++){
        pixels[(i*W_WIDTH+col)*4] = 0;
        pixels[(i*W_WIDTH+col)*4+1] = 0;
        pixels[(i*W_WIDTH+col)*4+2] = 0;
        pixels[(i*W_WIDTH+col)*4+3] = 255;
    }

    int h3 = W_HEIGHT;
    for(int i = h2; i < h2; i++){
        pixels[(i*W_WIDTH+col)*4] = 0;
        pixels[(i*W_WIDTH+col)*4+1] = 0;
        pixels[(i*W_WIDTH+col)*4+2] = 0;
        pixels[(i*W_WIDTH+col)*4+3] = 255;
    }

}

uint8_t max(uint8_t a, uint8_t b){
    return a < b;
}

void render_frame(){

    float depth_ratio = W_HEIGHT/(float)max(gamemap.W, gamemap.H);
    for(int i = 0; i < W_WIDTH; i++){
        ray_collision colision = cast_ray(player.pos.x, player.pos.y, player.r);
        drawline(i, colision.dist*depth_ratio, colision.object);  
    }

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
    player = {spawnpoint.x, spawnpoint.y, 0.0};
    create_window();
    while(true){
        render_frame();    
    }
     window->setVerticalSyncEnabled(true);
     window->setFramerateLimit(60);
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
         display_texture->update(pixels);
         window->draw(*display_sprite);
         window->display();
     }
}