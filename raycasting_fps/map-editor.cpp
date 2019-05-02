#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <cstring>
#include <ncurses.h>
#include <string>
#include <math.h>
#include "gamemap.h"
#include <iostream>

Map map;

bool blinker = false;
int blinker_pos = 0;

void clear_screen(){
    erase();
}

bool edit_mode = false;

void print_map(){
    for(int i = 0; i < map.H*map.W; i++){
        if(blinker && i == blinker_pos)
            printw("_ ");
        else
            printw("%hhd ", map.map[i]);
        if((i+1) % map.W == 0)
            printw("\n");
    }
}

void keyboard_input(){
    unsigned char c = getch();
    if((c == 'a' || c == 'A') && blinker_pos - 1 >= 0)
        blinker_pos--;
    else if((c == 's' || c == 'S') && blinker_pos + map.W < map.W*map.H)
        blinker_pos += map.W;
    else if((c == 'd' || c == 'D') && blinker_pos + 1 < map.W*map.H)
        blinker_pos++;
    else if((c == 'w' || c == 'W') && blinker_pos - map.W >= 0)
        blinker_pos -= map.W;
    else if(c >= '0' && c <= '9'){
        map.map[blinker_pos] = c-'0';
    }else if(c == 'e' || c == 'E'){
        edit_mode = false;
    }
}

void update_screen(){
    clear_screen();
    print_map();
    refresh();
}

void save_map(char *map_name){
    FILE *f = fopen(((std::string)(map_name) + ".map").c_str(), "w");
    if(f == NULL){
        printf("Error saving map");
        return;
    }
    fprintf(f, "%d %d\n", map.W, map.H);
    for(int i = 0; i < map.W*map.H; i++){
        fprintf(f, "%d ", map.map[i]);
        if((i+1) % map.W == 0)
            fprintf(f, "\n");
    }
    fclose(f);
}


void edit_map(){
    initscr();
    cbreak();
    noecho();
    timeout(0);
    uint8_t frame_count;
    edit_mode = true;
    while(edit_mode){
        keyboard_input();
        update_screen();
        usleep(16666); // 60 fps
        frame_count++;
        if(frame_count == 30){
            frame_count = 0;
            blinker = !blinker;
        }
    }
    printw("\nDo you want to save this map (y/n)?\n");
    while(true){
        int c = getch();
        if(c == 'n' || c == 'N'){
            endwin();
            return;
        }
        else if(c == 'y' || c == 'Y')
            break;
    }
    endwin();
    char map_name[100];
    printf("Enter map name: ");
    scanf("%s", map_name);
    save_map(map_name);
}


int main(int argc, char* argv[]){
    endwin();
    bool success = false;
    while(!success){
        int option;
        printf("Pick an option:\n\t1 -New map\n\t2 - Load map\n");
        printf("ENTER NUMBER: ");
        scanf("%d", &option);
        printf("\n");
        if(option == 1){
            int w, h;
            printf("Enter map width:\n");
            scanf("%d %d", &w, &h);
            map = Map(w, h);
            edit_map();
            success = true;
        }else if(option == 2){
            std::string filename;
            printf("Enter map relative direcory: \n");
            getchar();
            getline(std::cin, filename);
            map.load_map(filename.c_str());
            edit_map();
            success = true;
        }else{
            printf("INVALID OPTION VALUE! TRY AGAIN!\n");
        }
    }
}