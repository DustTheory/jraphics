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
    int area;
    uint8_t *map;
    Map(){}
    Map(int W, int H) : W(W), H(H), area(H*W){
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

    uint8_t at_coord(Point p, bool convert = true){
        int a = floor(p.x);
        int b = floor(p.y);
        int i = W*b + a;
        return map[i];
    }

    bool out_of_bounds(Point p){
        return ceil(p.x) < 0 || ceil(p.x) >= W || ceil(p.y) < 0 || ceil(p.y) >= H;
    }

    Point to_coord(Point p){
        p.y = (float)H-1 - p.y;
        return p;
    }


    Point get_spawnpoint(){
        for(int i = 0; i < W*H; i++)
            if(map[i] == spawnpoint){
                float x = (i+1) % W;
                float y = (i+1) / W;
                return to_coord({x , y});
            }
        return {0.0, 0.0}; 
    }
};
