#include <stdio.h>
#include <stdlib.h>
 
typedef struct {
    int x;
    int y;
} Point;
 
typedef enum {
    Triangle,
    Square,
    Circle
} ShapeType;
 
typedef struct {
    Point p;
    ShapeType type;
    char* name;
} Shape;
 
typedef struct {
    Shape* shapes;
    int size;
} Container;
 
int main() {
    Container* ct = init_container();
 
    // TO DO:
    // add_new_shape
    // print
    // remove_shape _by_index
 
    free(ct);
}
 
Container * init_container() {
    Container* ct = malloc(sizeof(Container));
    ct->shapes = NULL;
    ct->size = 0;
    return ct;
}
 
void add_new_shape(Container*, Point, char*, ShapeType type) {
    // init shapes
    //ct->shapes
    // create shape
    // shapes <= new shape
    // size++
}
 
void print(Container*){
}
 
void remove_shape_by_index(Container*, int) {
 
}