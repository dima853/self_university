#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>

#define WIDTH 60
#define HEIGHT 30
#define PI 3.14159265

// Расширенный набор символов для рисования
const char *shades = ".:!*oe&#%@";
#define SHADES_COUNT 10

char buffer[HEIGHT][WIDTH];
float z_buffer[HEIGHT][WIDTH];

void clearBuffers() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            buffer[y][x] = ' ';
            z_buffer[y][x] = -9999;
        }
    }
}

void drawPoint(int x, int y, float z, int shade) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (z > z_buffer[y][x]) {
            z_buffer[y][x] = z;
            buffer[y][x] = shades[shade % SHADES_COUNT];
        }
    }
}

void render() {
    printf("\033[H");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(buffer[y][x]);
        }
        putchar('\n');
    }
}

typedef struct { float x, y, z; } Vec3;

Vec3 rotateY(Vec3 v, float angle) {
    return (Vec3){
        v.x * cos(angle) + v.z * sin(angle),
        v.y,
        -v.x * sin(angle) + v.z * cos(angle)
    };
}

Vec3 project(Vec3 v, float scale) {
    float fov = 10.0;
    float z_offset = 5.0;
    float factor = fov / (v.z + z_offset);
    return (Vec3){
        WIDTH/2 + v.x * factor * scale,
        HEIGHT/2 - v.y * factor * scale,
        v.z
    };
}

void drawTriangle(Vec3 a, Vec3 b, Vec3 c, int shade) {
    Vec3 pa = project(a, 12.0);
    Vec3 pb = project(b, 12.0);
    Vec3 pc = project(c, 12.0);
    
    // Нормаль для отбраковки невидимых граней
    Vec3 edge1 = {pb.x - pa.x, pb.y - pa.y, pb.z - pa.z};
    Vec3 edge2 = {pc.x - pa.x, pc.y - pa.y, pc.z - pa.z};
    float normal_z = edge1.x * edge2.y - edge1.y * edge2.x;
    if (normal_z <= 0) return;

    // Закрашиваем треугольник
    float minX = fmin(pa.x, fmin(pb.x, pc.x));
    float maxX = fmax(pa.x, fmax(pb.x, pc.x));
    float minY = fmin(pa.y, fmin(pb.y, pc.y));
    float maxY = fmax(pa.y, fmax(pb.y, pc.y));
    
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) continue;
            
            // Проверка принадлежности точки треугольнику
            float w1 = ((pb.y - pc.y)*(x - pc.x) + (pc.x - pb.x)*(y - pc.y)) / 
                       ((pb.y - pc.y)*(pa.x - pc.x) + (pc.x - pb.x)*(pa.y - pc.y));
            float w2 = ((pc.y - pa.y)*(x - pc.x) + (pa.x - pc.x)*(y - pc.y)) /
                       ((pb.y - pc.y)*(pa.x - pc.x) + (pc.x - pb.x)*(pa.y - pc.y));
            float w3 = 1 - w1 - w2;
            
            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                float z = w1*pa.z + w2*pb.z + w3*pc.z;
                drawPoint(x, y, z, shade);
            }
        }
    }
}

int main() {
    printf("\033[2J\033[?25l");
    
    // Вершины треугольной призмы (2 треугольника + 3 боковых прямоугольника)
    Vec3 base[3] = {
        {-2, -1.5, 2}, {2, -1.5, 2}, {0, 2, 2}
    };
    Vec3 top[3] = {
        {-2, -1.5, -2}, {2, -1.5, -2}, {0, 2, -2}
    };
    
    float angle = 0;
    while (1) {
        clearBuffers();
        
        // Вращаем все вершины
        Vec3 rotated_base[3], rotated_top[3];
        for (int i = 0; i < 3; i++) {
            rotated_base[i] = rotateY(base[i], angle);
            rotated_top[i] = rotateY(top[i], angle);
        }
        
        // Рисуем основание (темнее)
        drawTriangle(rotated_base[0], rotated_base[1], rotated_base[2], 2);
        
        // Рисуем верхнюю грань (светлее)
        drawTriangle(rotated_top[0], rotated_top[1], rotated_top[2], 8);
        
        // Рисуем боковые грани с разными оттенками
        for (int i = 0; i < 3; i++) {
            int next = (i + 1) % 3;
            drawTriangle(rotated_base[i], rotated_base[next], rotated_top[i], 5);
            drawTriangle(rotated_top[i], rotated_top[next], rotated_base[next], 5);
        }
        
        render();
        usleep(50000);
        angle += 0.05;
    }
    
    printf("\033[?25h");
    return 0;
}