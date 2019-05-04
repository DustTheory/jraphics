#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <unistd.h>

#include "gamemap.h"

Map gamemap;
sf::RenderWindow *window;
sf::Uint8 *pixels;
sf::Texture *display_texture;
sf::Sprite *display_sprite;

int W_WIDTH = 500, W_HEIGHT = 500;

void create_window(){    
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "raycasting_fps");
   
    pixels = new sf::Uint8[W_WIDTH*W_HEIGHT*4];
    display_texture = new sf::Texture;
    display_texture->create(W_WIDTH, W_HEIGHT);
    display_sprite = new sf::Sprite(*display_texture);
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

uint8_t max(uint8_t a, uint8_t b){
    return a > b ? a : b;
}

float deg_to_rad(float deg){
    return deg*M_PI/180;
}

float rad_to_deg(float rad){
    return rad*180/M_PI;
}

void set_pixel(int a, int b, sf::Color c){
    pixels[(a*W_WIDTH+b)*4] = c.r;
    pixels[(a*W_WIDTH+b)*4+1] = c.g;
    pixels[(a*W_WIDTH+b)*4+2] = c.b;
    pixels[(a*W_WIDTH+b)*4+3] = c.a;
}

sf::Color colormapping[5] = {
    sf::Color::Red,
    sf::Color::Magenta,
    sf::Color::Cyan,
    sf::Color::Blue,
    sf::Color::Green
};

void drawline(int col, float h, uint8_t o){

    int h1 = (W_HEIGHT-(int)h)/2;
    for(int i = 0; i < h1; i++){
        set_pixel(i, col, sf::Color(255, 255, 255, 50));
    }
    int h2 = h1 + h;
    sf::Color c;
    
    for(int i = h1; i < h2; i++){
        set_pixel(i, col, colormapping[o]);
    }

    int h3 = W_HEIGHT;
    for(int i = h2; i < h3; i++){
        set_pixel(i, col, sf::Color(255, 255, 0, 50));
    }

}

void render_frame(){

	double FOV = deg_to_rad(66)/2; // 66 degrees FOV

	double dir_x = cos(player.r), dir_y = sin(player.r);

    double plane_x = 1, plane_y = -dir_x/dir_y; // Vector perpendicular to dir vector
	// Turn it into unit vector;
	double plane_len = sqrt(plane_x*plane_x + plane_y*plane_y);
	plane_x /= plane_len;
	plane_y /= plane_len;
	// Scale to match FOV
	plane_x *= abs(tan(FOV));
	plane_y *= abs(tan(FOV));

	for(int x = 0; x < W_WIDTH; x++){
	
		double camera_x = (dir_y < 0 ? 1 : -1)*(2 * x/double(W_WIDTH) - 1); // Map x to range (-1, +1)
		double ray_dir_x = dir_x + plane_x*camera_x;
        double ray_dir_y = dir_y + plane_y*camera_x;
		
		int map_x = int(player.pos.x), map_y = int(player.pos.y); // Current square
		double side_dist_x, side_dist_y; // Dist to next x or y side
		double delta_dist_x = sqrt(1 + (ray_dir_y)*(ray_dir_y)/(ray_dir_x)*(ray_dir_x));
		double delta_dist_y = sqrt(1 + (ray_dir_x)*(ray_dir_x)/(ray_dir_y)*(ray_dir_y));
        double prep_wall_dist;
		
		bool hit = false;
		int side;
        int step_x, step_y;
		
		if(ray_dir_x < 0){
			step_x = -1;
			side_dist_x = (player.pos.x - map_x) * delta_dist_x;
		}else{
			step_x = 1;
			side_dist_x = (map_x + 1 - player.pos.x) * delta_dist_x;
		}
		
		if(ray_dir_y < 0){
			step_y = -1;
			side_dist_y = (player.pos.y - map_y) * delta_dist_y;
		}else{
			step_y = 1;
			side_dist_y = (map_y + 1 - player.pos.y) * delta_dist_y;
		}
		// DDA
        uint8_t o;
		while(!hit){
			
				if(side_dist_x  < side_dist_y){
					side_dist_x += delta_dist_x;
					map_x += step_x;
					side = 0;
				}else{
					side_dist_y += delta_dist_y;
					map_y += step_y;
					side = 1;
				}
				o = gamemap.at_coord({(float)map_x, (float)map_y});
				if(o != empty && o != spawnpoint)
                	hit = true;
		}
        if(side == 0)
            prep_wall_dist = (map_x - player.pos.x + (1-step_x)/2)/ ray_dir_x;
        else 
            prep_wall_dist = (map_y - player.pos.y + (1-step_y)/2)/ ray_dir_y;

        prep_wall_dist = (double)W_HEIGHT / prep_wall_dist;
        drawline(x, (int)(prep_wall_dist), o);  
	}


    display_texture->update(pixels);
    window->draw(*display_sprite);

 }

int main(int argc, char* argv[]){
    printf("########## WELCOME! ##########\nraycasting fps game by ieexmml\n\n");

    printf("Enter map file relative path: \n");
    std::string filename;
    getline(std::cin, filename);
    gamemap.load_map(filename.c_str());
    Point spawnpoint = gamemap.get_spawnpoint();
    printf("SPAWN: %f %f\n", spawnpoint.x, spawnpoint.y);
    player = {spawnpoint.x, spawnpoint.y, deg90/2};
    create_window();

    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);


    float rotate_speed = 0.1;
    float move_speed = 0.1;
    char key;
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed){
                key = event.key.code + 'a';
                printf("%c\n", key);
                if (key == 'w' || key == 'W'){
                    player.pos.x += cos(player.r)*move_speed;
                    player.pos.y += sin(player.r)*move_speed;
                }
                if (key == 'a' || key == 'A'){
                    player.r -= rotate_speed;
                }  
                if (key == 's' || key == 'S'){
                    player.pos.x -= cos(player.r)*move_speed;
                    player.pos.y -= sin(player.r)*move_speed;
                }
                if (key == 'd' || key == 'D'){
                    player.r += rotate_speed;
                }
            }

            if (event.type == sf::Event::Closed)
                window->close();
        }
        window->clear(sf::Color::Black);
        render_frame();
        window->display();
    }
}
