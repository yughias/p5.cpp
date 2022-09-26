#include "../include/p5.h"
#include <windows.h>
#include <time.h>
#include <math.h>

//-> P5.CPP FUNCTIONS <-\\

int w = 800;
int h = 600;
const int& width = w;
const int& height = h;

int fCount;
const int& frameCount = fCount;
int fps = 60;

int colorStroke = 0xFFFFFFFF;
bool isStroke = true;

int colorFill = 0xFF000000;
bool isFill = true;

int* pixels;
int* internBuffer;

void size(int w_, int h_){
    w = w_;
    h = h_;
    delete[] pixels;
    delete[] internBuffer;
    pixels = new int[w*h];
    internBuffer = new int[w*h];
}

double milliseconds(){
    return (double)clock()/CLOCKS_PER_SEC*1000;
}

int millis(){
    return milliseconds();
}

bool setFrameRate(int val){
    if(val <= 0)
        return false;
    fps = val;
    return true;
}

void set(int x, int y, int c){
    if(x < 0 || x >= width || y < 0 || y >= height)
        return;
    internBuffer[x + y*width] = c;
}

int get(int x, int y){
    if(x < 0 || x >= width || y < 0 || y >= height)
        return 0;
    return internBuffer[x+y*width];
}

int color(int grayscale){
    return (255<<24) | (grayscale<<16) | (grayscale<<8) | grayscale;
}

int color(int r, int g, int b){
    return (255<<24) | (r<<16) | (g<<8) | b;
}

int red(int color){
    return (color & 0xFF0000) >> 16;
}

int green(int color){
    return (color & 0xFF00) >> 8;
}

int blue(int color){
    return color & 0xFF;
}

void noFill(){
    isFill = false;
}

void fill(int grayscale){
    colorFill = color(grayscale);
}

void fill(int r, int g, int b){
    colorFill = color(r, g, b);
}

void background(int grayscale){
    int c = color(grayscale);
    for(int i = 0; i < w*h; i++)
        internBuffer[i] = c;
}

void background(int r, int g, int b){
    int c = color(r, g, b);
    for(int i = 0; i < w*h; i++)
        internBuffer[i] = c;
}

void noStroke(){
    isStroke = false;
}

void stroke(int grayscale){
    colorStroke = color(grayscale);
}

void stroke(int r, int g, int b){
    colorStroke = color(r, g, b);
}

void line(int x0, int y0, int x1, int y1){
    int Dx = x1 - x0;
    int Dy = y1 - y0;

    //# Increments
    int Sx = (Dx<0)? -1 : 1;
    int Sy = (Dy<0)? -1 : 1;

    //# Segment length
    Dx = abs(Dx);
    Dy = abs(Dy);
    int D = (Dx > Dy)? Dx : Dy;

    //# Initial remainder
    double R = D / 2;

    int X = x0;
    int Y = y0;
    if(Dx > Dy)
    {
        //# Main loop
        for(int I=0; I<D; I++)
        {
            set(X, Y, colorStroke);
            //# Update (X, Y) and R
            X += Sx;
            R+= Dy; //# Lateral move
            if (R >= Dx)
            {
                Y += Sy;
                R -= Dx; //# Diagonal move
            }
        }
    }
    else
    {
        //# Main loop
        for(int I=0; I<D; I++)
        {
            set(X, Y, colorStroke);
            //# Update (X, Y) and R
            Y += Sy;
            R += Dx; //# Lateral move
            if(R >= Dy)
            {
                X += Sx;
                R -= Dy; //# Diagonal move
            }
        }
    }
}

void rect(int x0, int y0, int w_, int h_){
    if(isStroke){
        line(x0, y0, x0+w_, y0);
        line(x0+w_, y0, x0+w_, y0+h_);
        line(x0+w_, y0+h_, x0, y0+h_);
        line(x0, y0+h_, x0, y0);
    }
    if(isFill){
        for(int y = y0+1; y < y0+h_; y++)
            for(int x = x0+1; x < x0+w_; x++)
                set(x, y, colorFill);
    }
}

void square(int x0, int y0, int size){
    rect(x0, y0, size, size);
}

void ellipse(int x0, int y0, int rx, int ry){
    float dx, dy, d1, d2, x, y;
    x = 0;
    y = ry;

    // Initial decision parameter of region 1
    d1 = (ry * ry) - (rx * rx * ry) +
         (0.25 * rx * rx);
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    // For region 1
    while (dx < dy)
    {

        if(isStroke) {
            set(x0+x, y+y0, colorStroke);
            set(x0-x, y+y0, colorStroke);
            set(x0+x, -y+y0, colorStroke);
            set(x0-x, -y+y0, colorStroke);
        }

        if(isFill){
            for(int xx = x0-x+1; xx < x0+x; xx++){
                if(int(y) != ry) {
                    set(xx, y0-y, colorFill);
                    set(xx, y0+y, colorFill);
                }
            }
        }

        // Checking and updating value of
        // decision parameter based on algorithm
        if (d1 < 0)
        {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        }
        else
        {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }

    // Decision parameter of region 2
    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
         ((rx * rx) * ((y - 1) * (y - 1))) -
         (rx * rx * ry * ry);

    // Plotting points of region 2
    while (y >= 0)
    {

        if(isStroke) {
            // Print points based on 4-way symmetry
            set(x0+x, y+y0, colorStroke);
            set(x0-x, y+y0, colorStroke);
            set(x0+x, -y+y0, colorStroke);
            set(x0-x, -y+y0, colorStroke);
        }

        if(isFill){
            for(int xx = x0-x+1; xx < x0+x; xx++){
                if(int(y) != ry) {
                    set(xx, y0-y, colorFill);
                    set(xx, y0+y, colorFill);
                }
            }
        }

        // Checking and updating parameter
        // value based on algorithm
        if (d2 > 0)
        {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        }
        else
        {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
}

void circle(int x0, int y0, int radius){
    ellipse(x0, y0, radius, radius);
}

void loadPixels(){
    for(int i = 0; i < width*height; i++)
        pixels[i] = internBuffer[i];
}

void updatePixels() {
    for (int i = 0; i < width * height; i++)
        internBuffer[i] = pixels[i];
}

float map(float value, float start1, float stop1, float start2, float stop2){
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

float dist(float x0, float y0, float x1, float y1){
    return sqrtf((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0));
}

float lerp(float v0, float v1, float t){
    return (1 - t) * v0 + t * v1;
}

void playAsyncWav(const char* filename){
    PlaySound(TEXT(filename), NULL, SND_FILENAME | SND_ASYNC);
}