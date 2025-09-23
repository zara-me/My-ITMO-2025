#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Container* init_container(void);
int add_new_shape(Container* ct, Point p, const char* name, ShapeType type);
void print_container(const Container* ct);
int remove_shape_by_index(Container* ct, int index);
void free_container(Container* ct);

Container* init_container(void) {
    Container* ct = malloc(sizeof *ct);
    if (!ct) return NULL;
    ct->shapes = NULL;
    ct->size = 0;
    return ct;
}

static char* str_clone(const char* s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    // +1
    // "hello" + '\0'
    char* p = malloc(len + 1);
    if (p) memcpy(p, s, len + 1);
    return p;
}

static const char* shape_type_to_str(ShapeType t) {
    switch (t) {
        case Triangle: return "Triangle";
        case Square:   return "Square";
        case Circle:   return "Circle";
        default:       return "Unknown";
    }
}


int add_new_shape(Container* ct, Point p, const char* name, ShapeType type) {
    if (!ct) return -1;

    char* name_copy = strdup(name);
    if (name && name_copy == NULL) return -1;

    int new_size = ct->size + 1;
    Shape* tmp = realloc(ct->shapes, new_size * sizeof(Shape));
    if (!tmp) {
        free(name_copy);
        return -1;
    }

    ct->shapes = tmp;

    ct->shapes[ct->size].p = p;
    ct->shapes[ct->size].type = type;
    ct->shapes[ct->size].name = name_copy;

    ct->size = new_size;
    return 0;
}

void print_container(const Container* ct) {
    if (!ct) return;
    printf("Container size = %d\n", ct->size);
    for (int i = 0; i < ct->size; ++i) {
        const Shape* s = &ct->shapes[i];
        printf("[%d] name = \"%s\", type = %s, point = (%d, %d)\n",
               i,
               s->name ? s->name : "(null)",
               shape_type_to_str(s->type),
               s->p.x, s->p.y);
    }
}


int remove_shape_by_index(Container* ct, int index) {
    if (!ct) return -1;
    if (index < 0 || index >= ct->size) return -1;

    free(ct->shapes[index].name);

    if (index < ct->size - 1) {
        memmove(&ct->shapes[index],
                &ct->shapes[index + 1],
                (ct->size - index - 1) * sizeof *ct->shapes);
    }

    ct->size -= 1;

    if (ct->size == 0) {
        free(ct->shapes);
        ct->shapes = NULL;
    } else {
        Shape* tmp = realloc(ct->shapes, ct->size * sizeof *tmp);
        if (tmp) ct->shapes = tmp;
    }

    return 0;
}

void free_container(Container* ct) {
    if (!ct) return;
    for (int i = 0; i < ct->size; ++i) {
        free(ct->shapes[i].name);
    }
    free(ct->shapes);
    free(ct);
}
/* test test test*/
int main(void) {
    Container* ct = init_container();
    if (!ct) {
        fprintf(stderr, "failed to init container\n");
        return 1;
    }

    add_new_shape(ct, (Point){1, 2}, "Alpha", Triangle);
    add_new_shape(ct, (Point){3, 4}, "Box", Square);
    add_new_shape(ct, (Point){5, 6}, "Round", Circle);

    puts("=== After adds ===");
    print_container(ct);

    puts("\nRemoving index 1 ...");
    if (remove_shape_by_index(ct, 1) != 0) {
        fprintf(stderr, "remove failed\n");
    }

    puts("=== After remove ===");
    print_container(ct);

    free_container(ct);
    return 0;
}
