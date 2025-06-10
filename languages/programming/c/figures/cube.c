#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A, B, C;

int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
int distanceCam = 100;
float horizontalOffset;
int cubeWidth = 20;
float k1 = 40;

float speedincrement = 1.0;

float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + 
    j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch){
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceCam;

    ooz = 1 / z;
    xp = (int)(width / 2 - 2 * cubeWidth +  k1 * ooz * x * 2);
    yp = (int)(height / 2 + k1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height){
        if (ooz > zBuffer[idx]){
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main(){
    printf("\x1b[2J");
    while(1){
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);
        cubeWidth = 20;
        horizontalOffset = -0.5 * cubeWidth;
        
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += speedincrement){
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += speedincrement){
                calculateForSurface(cubeX, cubeY, -cubeWidth, '.');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }   

        printf("\x1b[H");
        for (int k = 0; k < width * height; k++){
            putchar(k % width ? buffer[k] : 10);
        }
        A += 0.05;
        B += 0.05;
        C += 0.01;
        usleep(0);

    }
    return 0;
}           