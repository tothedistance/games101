#include "discrete_math.hpp"

namespace games {

vector<Vector3i> rasterize_line(Vector3f begin, Vector3f end) {
    int x0 = begin.x();
    int y0 = begin.y();
    int x1 = end.x();
    int y1 = end.y();

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;

    int err = dx - dy;

    vector<Vector3i> pixels;

    while (true) {
        pixels.push_back(Vector3i(x0, y0, 0));

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
    return pixels;
}

}  // namespace games
