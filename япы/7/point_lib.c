// point_lib.c
#include <stdint.h>

#ifdef _WIN32
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif

typedef struct {
    int32_t x;
    int32_t y;
} Point;

typedef int (__cdecl *PointPred)(Point p);

EXPORT int __cdecl filter(const Point* in_points, int count, Point* out_points, PointPred pred) {
    int written = 0;
    for (int i = 0; i < count; ++i) {
        Point p = in_points[i];
        if (pred(p)) {
            out_points[written++] = p;
        }
    }
    return written;
}

EXPORT void __cdecl process_point(Point* p) {
    if (!p) return;
    p->x += 10;
    p->y += 20;
}

EXPORT void __cdecl procces_array_point(Point* p, int count) {
    for (int i = 0; i < count; ++i) {
        p[i].x += 1;
        p[i].y += 1;
    }
}

typedef int (__cdecl *MyFunc)(int);
EXPORT void __cdecl foo(int a, MyFunc func) {
    if (!func) return;
    for (int i = 0; i < a; ++i) {
        func(i);
    }
}
