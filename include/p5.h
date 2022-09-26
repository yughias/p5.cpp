#define LEFT 37
#define UP 38
#define RIGHT 39
#define DOWN 40
#define ESCAPE 27
#define BACKSPACE

extern int* pixels;
extern const int& width;
extern const int& height;
extern const int& frameCount;
extern const float& frameRate;
extern const bool& isKeyPressed;
extern const bool& isKeyReleased;
extern const bool& mousePressed;
extern const unsigned int& key;
extern const int& mouseX;
extern const int& mouseY;
extern const int& pmouseX;
extern const int& pmouseY;

void keyPressed();
void keyReleased();

void mouseMoved();

void setup();

void draw();
void size(int w, int h);

double milliseconds();
int millis();

bool setFrameRate(int);

void loadPixels();
void updatePixels();

void noFill();
void fill(int grayscale);
void fill(int r, int g, int b);

int color(int grayscale);
int color(int r, int g, int b);

void setPixel(int x, int y, int c);

void background(int grayscale);
void background(int r, int g, int b);

void noStroke();
void stroke(int grayscale);
void stroke(int r, int g, int b);

void line(int x0, int y0, int x1, int y1);

void rect(int x0, int y0, int w_, int h_);
void square(int x0, int y0, int size);

void ellipse(int x0, int y0, int r1, int r2);
void circle(int x0, int y0, int radius);

float map(float val, float start1, float stop1, float start2, float stop2);
