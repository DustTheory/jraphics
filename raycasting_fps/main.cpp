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

sf::RectangleShape *minimap;
sf::Uint8 *minimap_pixels;
sf::Texture *minimap_texture;
sf::Sprite *minimap_sprite;


int W_WIDTH = 500, W_HEIGHT = 500;

void create_window(){    
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "raycasting_fps");
   
    pixels = new sf::Uint8[W_WIDTH*W_HEIGHT*4];
    display_texture = new sf::Texture;
    display_texture->create(W_WIDTH, W_HEIGHT);
    display_sprite = new sf::Sprite(*display_texture);

    minimap_pixels = new sf::Uint8[gamemap.W*gamemap.H*4];
    minimap_texture = new sf::Texture;
    minimap_texture->create(gamemap.W, gamemap.H);
    minimap_sprite = new sf::Sprite(*minimap_texture);
}

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

void set_pixel(int a, int b, sf::Color c){
    pixels[(a*W_WIDTH+b)*4] = c.r;
    pixels[(a*W_WIDTH+b)*4+1] = c.g;
    pixels[(a*W_WIDTH+b)*4+2] = c.b;
    pixels[(a*W_WIDTH+b)*4+3] = c.a;
}


ray_collision cast_ray(float x, float y, float r){
        
}


void drawline(int col, float h, uint8_t o){

    int h1 = (W_HEIGHT-(int)h)/2;
    for(int i = 0; i < h1; i++){
        set_pixel(i, col, sf::Color(255, 255, 255, 50));
    }
    int h2 = h1 + h;
    for(int i = h1; i < h2; i++){
        set_pixel(i, col, sf::Color(255,0, 0, 50));
    }

    int h3 = W_HEIGHT;
    for(int i = h2; i < h3; i++){
        set_pixel(i, col, sf::Color(255, 255, 0, 50));
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

    int gamemap_area = gamemap.W*gamemap.H*4;
    int a, b;
    uint8_t o;
    for(int i = 0; i < gamemap_area; i+=4){
        o = gamemap.map[i/4];
        if(o == empty){
            minimap_pixels[i] = 255;
            minimap_pixels[i+1] = 255;
            minimap_pixels[i+2] = 255;
            minimap_pixels[i+3] = 50;
        }else if(o == wall){
            minimap_pixels[i] = 70;
            minimap_pixels[i+1] = 70;
            minimap_pixels[i+2] = 70;
            minimap_pixels[i+3] = 50;
        }else{
            minimap_pixels[i] = 255;
            minimap_pixels[i+1] = 0;
            minimap_pixels[i+2] = 0;
            minimap_pixels[i+3] = 50;
        }
    }


    float scale_y = (float)W_HEIGHT/3.0/(float)gamemap.H;
    float scale_x = (float)gamemap.W/(float)gamemap.H*scale_y;

    float depth_ratio = (float)W_HEIGHT/(gamemap.H+gamemap.W);
    float FOV = deg_to_rad(120); // 120 degree FOV
    float angle = player.r + FOV/2;
    while(angle > deg360)
        angle -= deg360;
    float step = FOV/(float)W_WIDTH;
    for(int i =  W_WIDTH-1; i >=0; i--){
        ray_collision colision = cast_ray(player.pos.x, player.pos.y, angle);
        drawline(i, colision.dist*depth_ratio, colision.object);  
        angle -= step;
    }

    minimap_texture->update(minimap_pixels);
    minimap_sprite->setScale(scale_x, scale_y);
    minimap_sprite->setPosition(scale_x*0.5, scale_y*0.5);

    display_texture->update(pixels);
    window->draw(*display_sprite);
    window->draw(*minimap_sprite);

    angle = player.r + FOV/2;
    while(angle > deg360)
        angle -= deg360;

    for(int i = 0; i < W_WIDTH; i++){
        sf::RectangleShape line(sf::Vector2f(150.f, 1.f));
        line.setPosition((player.pos.x)*scale_x, scale_y*(gamemap.H-player.pos.y));
        line.rotate(rad_to_deg(-angle));
        line.setFillColor(sf::Color(0, 0, 0, 10));
        window->draw(line);
        angle -= step;
    }
}

int main(int argc, char* argv[]){
    printf("########## WELCOME! ##########\nraycasting fps game by ieexmml\n\n");

    printf("Enter map file relative path: \n");
    std::string filename;
    getline(std::cin, filename);
    gamemap.load_map(filename.c_str());
    Point spawnpoint = gamemap.get_spawnpoint();
    printf("SPAWN: %f %f\n", spawnpoint.x, spawnpoint.y);
    player = {spawnpoint.x, spawnpoint.y, deg270};
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
        if(framecount == 60){
            framecount = 0;
            player.r+=0.3;
        }
        window->display();
    }
}