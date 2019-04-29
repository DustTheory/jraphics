enum Block{
    empty = (uint8_t)0,
    wall,
    door,
    portal,
    spawnpoint
};

struct Point{
    float x, y;
};

struct Map{
    int W, H;
    uint8_t *map;
    Map(){}
    Map(int W, int H) : W(W), H(H){
        map = (uint8_t*)malloc(W*H*sizeof(uint8_t));
        memset(map, empty, W*H);
    }
    void load_map(const char *filename){
        FILE* fp = fopen(filename, "r");
        if(fp == NULL){
            printf("File does not exist: %s \n", filename);
            exit(EXIT_FAILURE);
        }

        char * line = NULL;
        size_t len = 0;
        
        if(getline(&line, &len, fp) != -1)
                sscanf(line, "%d %d", &W, &H);

        map = (uint8_t*)malloc(W*H*sizeof(uint8_t));
        uint8_t *ptr = map;
        while(fscanf(fp, "%hhd", ptr) != EOF && ptr <= map+W*H)
            ptr++;

        fclose(fp);
        free(line);
    }

    uint8_t at_coord(Point p){
        p = to_coord(p);
        return map[int(p.y)*W+int(p.x)];
    }

    bool out_of_bounds(Point p){
        return p.x < 0 || p.x > W || p.y < 0 || p.y > H;
    }

    Point to_coord(Point p){
        p.y = (float)H - p.y;
        return p;
    }

    Point get_spawnpoint(){
        for(int i = 0; i < W*H; i++)
            if(map[i] == spawnpoint){
                return to_coord({(float)(spawnpoint % 3), (float)(spawnpoint / 3)});
            }
        return {0.0, 0.0}; 
    }
};

typedef struct Map Map;


void print_map(Map *map){
 
}