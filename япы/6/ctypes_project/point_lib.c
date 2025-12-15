#include <math.h>
#include <stddef.h>

#ifdef _WIN32
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct PointPair {
    Point a;
    Point b;
} PointPair;

/*
 * pairs: pointer to array of PointPair
 * n: number of pairs
 * out: pointer to preallocated array of double (length n)
 */
EXPORT void compute_distances(const PointPair* pairs, size_t n, double* out) {
    for (size_t i = 0; i < n; ++i) {
        long long dx = (long long)pairs[i].a.x - pairs[i].b.x;
        long long dy = (long long)pairs[i].a.y - pairs[i].b.y;
        double sq = (double)(dx*dx + dy*dy);
        out[i] = sqrt(sq);
    }
}
