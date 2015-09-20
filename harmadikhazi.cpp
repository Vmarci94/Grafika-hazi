//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D,
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi,
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : SZABÓ TAMÁS
// Neptun : NEPTUN
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
 
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
 
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...
 
float delta = 0.01f;
 
struct Vector {
    float x, y, z;
    
    Vector() {
        x = y = z = 0;
    }
    
    Vector(float x0, float y0, float z0 = 0) {
        x = x0; y = y0; z = z0;
    }
    
    Vector operator*(float a) {
        return Vector(x * a, y * a, z * a);
    }
    
    Vector operator+(const Vector& v) {
        return Vector(x + v.x, y + v.y, z + v.z);
    }
    
    Vector operator-(const Vector& v) {
        return Vector(x - v.x, y - v.y, z - v.z);
    }
    
    float operator*(const Vector& v) {
        return (x * v.x + y * v.y + z * v.z);
    }
    
    float operator^(float c) {
        return (powf(x, c) + powf(y, c) + powf(z, c));
    }
    
    Vector operator/(const Vector& v){
        return Vector(x / v.x, y / v.y, z / v.z);
    }
    
    Vector operator/(float v){
        return Vector(x / v, y / v, z / v);
    }
    
    Vector operator%(const Vector& v) {
        return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }
    
    float Length() {
        return sqrt(x * x + y * y + z * z);
    }
    
    bool operator ==(const Vector& v){
        return fabsf(x - v.x) < delta && fabsf(y - v.y) < delta && fabsf(z - v.z) < delta;
    }
    
    Vector Norm() {
        return *this / Length();
    }
};
 
struct Color {
    float r, g, b;
    
    Color() {
        r = g = b = 0;
    }
    
    Color(float r0, float g0, float b0) {
        r = r0; g = g0; b = b0;
    }
    
    Color operator*(float a) {
        return Color(r * a, g * a, b * a);
    }
    
    Color operator*(const Color& c) {
        return Color(r * c.r, g * c.g, b * c.b);
    }
    
    Color operator/(float n) {
        return Color(r / n, g / n, b / n);
    }
    
    Color operator/(const Color& c) {
        return Color(r / c.r, g / c.g, b / c.b);
    }
    
    Color operator^(float c) {
        return Color(powf(r, c), powf(g, c), powf(b, c));
    }
    
    Color operator+(const Color& c) {
        return Color(r + c.r, g + c.g, b + c.b);
    }
    
    Color operator-(const Color& c) {
        return Color(r - c.r, g - c.g, b - c.b);
    }
};
 
const int screenWidth = 600;
const int screenHeight = 600;
 
float pi = 3.1415926536;
bool shownormals = false;
float rotate = 0;
float elore = 0;
bool spaceWasPressed = false;
long voltTime;
 
Vector eye = Vector(0, 20, 30);
 
void makeTexture(Color color[], int resolution){
    unsigned int texids;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texids);
    glBindTexture(GL_TEXTURE_2D, texids);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution, resolution, 0, GL_RGB, GL_FLOAT, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
 
void makeTexturePenguin(Color color[], int resolution){
    unsigned int texids;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texids);
    glBindTexture(GL_TEXTURE_2D, texids);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution, resolution, 0, GL_RGB, GL_FLOAT, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
 
struct LightSource{
    float position[4];
    float ls[4];
    float ld[4];
    float la[4];
    
    void setMaterial(Color inls, Color inld, Color inla, int shine){
        ls[0] = inls.r;
        ls[1] = inls.g;
        ls[2] = inls.b;
        ls[3] = 1;
        ld[0] = inld.r;
        ld[1] = inld.g;
        ld[2] = inld.b;
        ld[3] = 1;
        la[0] = inla.r;
        la[1] = inla.g;
        la[2] = inla.b;
        la[3] = 1;
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, la);
        glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
        glEnable(GL_LIGHT0);
    }
    
    LightSource(Vector pos){
        position[0] = pos.x;
        position[1] = pos.y;
        position[2] = pos.z;
        position[3] = 0;
        setMaterial(Color(0.1, 0.1, 0.1), Color(0.9921568627, 0.7215686275, 0.7450980392), Color(0.9921568627 / 100, 0.7215686275 / 100, 0.7450980392 / 100), 20);
    }
};
 
struct Plane{
    Vector position;
    float ks[4];
    float kd[4];
    float ka[4];
    float shininess[1];
    
    void setMaterial(Color inks, Color inkd, Color inka, int shine){
        ks[0] = inks.r;
        ks[1] = inks.g;
        ks[2] = inks.b;
        ks[3] = 1;
        kd[0] = inkd.r;
        kd[1] = inkd.g;
        kd[2] = inkd.b;
        kd[3] = 1;
        ka[0] = inka.r;
        ka[1] = inka.g;
        ka[2] = inka.b;
        ka[3] = 1;
        shininess[0] = shine;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
        glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
    
    Plane(){}
    Plane(Vector pos){
        setMaterial(Color(0.5, 0.5, 0.5), Color(0.5, 0.5, 0.5), Color(1, 1, 1), 20);
        position = pos;
    }
    
    void draw(){
        Color color[] = {Color(0.6823529412, 0.9333333333, 0.9333333333),
                         Color(0.95490196078, 0.7490196078, 0.9137254902),
                         Color(0.06470588235, 0.09490196078, 0.5529411765),
                         Color(0.9098039216, 0.8117647059, 0.9921568627)};
        makeTexture(color, 2);
        
        glBegin(GL_QUADS);
        
        glNormal3f(position.x, position.y, position.z);
        
        glTexCoord2f(0, 0);
        glVertex3f(-1000, 0, -1000);
        
        glTexCoord2f(1, 0);
        glVertex3f(-1000, 0, 1000);
        
        glTexCoord2f(0, 1);
        glVertex3f(1000, 0, 1000);
        
        glTexCoord2f(1, 1);
        glVertex3f(1000, 0, -1000);
        
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
};
 
struct Egg{
    Vector position;
    float szelesseg, magassag;
    float resolution;
    float ks[4];
    float kd[4];
    float ka[4];
    float shininess[1];
    bool iSvisible;
    
    void setMaterial(Color inks, Color inkd, Color inka, int shine){
        ks[0] = inks.r;
        ks[1] = inks.g;
        ks[2] = inks.b;
        ks[3] = 1;
        kd[0] = inkd.r;
        kd[1] = inkd.g;
        kd[2] = inkd.b;
        kd[3] = 1;
        ka[0] = inka.r;
        ka[1] = inka.g;
        ka[2] = inka.b;
        ka[3] = 1;
        shininess[0] = shine;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
        glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
    
    void setup(float szeles, float magas, float felbontas, Vector pos){
        szelesseg = szeles;
        magassag = magas;
        resolution = felbontas;
        position = pos;
    }
    
    Egg(){}
    Egg(float szeles, float magas, float felbontas, Vector pos){
        szelesseg = szeles;
        magassag = magas;
        resolution = felbontas;
        position = pos;
    }
    
    Vector whereIs(float inu, float inv){
        Vector temp;
        temp.x = szelesseg * cosf(inu) * sinf(inv);
        temp.y = magassag * sinf(inu) * sinf(inv);
        temp.z = szelesseg * cosf(inv);
        
        return temp;
    }
    
    Vector whereNorm(float inu, float inv){
        if (inv < delta)
            return Vector(0, 0, 1);
        if (inv > pi - delta)
            return Vector(0, 0, -1);
        Vector u, v;
        
        u.x = szelesseg * -sinf(inu) * sinf(inv);
        u.y = magassag * cosf(inu) * sinf(inv);
        u.z = 0;
        
        v.x = szelesseg * cosf(inu) * cosf(inv);
        v.y = magassag * sinf(inu) * cos(inv);
        v.z = szelesseg * -sinf(inv);
        
        return (v%u).Norm();
    }
    
    void draw(){
        if(iSvisible){
            setMaterial(Color(1, 1, 1), Color(1, 0.9607843137, 0.7647058824), Color(1, 0.9607843137, 0.7647058824), 20);
            float ui, vi;
            float ures = resolution * 1.5;
            float vres = resolution;
            float u = 2 * pi / (ures - 1);
            float v = pi / (vres - 1);
            Vector kozep, kozepNorm;
            
            glBegin(GL_QUADS);
            
            for (int i = 0; i < ures; i++){
                ui = 2 * pi*i / (ures - 1);
                for (int j = 0; j < vres; j++){
                    vi = pi*j / (vres - 1);
                    Vector a = whereIs(ui, vi);
                    Vector aNorm = whereNorm(ui, vi);
                    Vector b = whereIs(ui + u, vi);
                    Vector bNorm = whereNorm(ui + u, vi);
                    Vector c = whereIs(ui + u, vi + v);
                    Vector cNorm = whereNorm(ui + u, vi + v);
                    Vector d = whereIs(ui, vi + v);
                    Vector dNorm = whereNorm(ui, vi + v);
                    
                    kozep = (a + b + c + d) / 4;
                    kozepNorm = (aNorm + bNorm + cNorm + dNorm) / 4;
                    
                    glNormal3f(aNorm.x, aNorm.y, aNorm.z);
                    glVertex3f(a.x, a.y, a.z);
                    
                    glNormal3f(bNorm.x, bNorm.y, bNorm.z);
                    glVertex3f(b.x, b.y, b.z);
                    
                    glNormal3f(cNorm.x, cNorm.y, cNorm.z);
                    glVertex3f(c.x, c.y, c.z);
                    
                    glNormal3f(dNorm.x, dNorm.y, dNorm.z);
                    glVertex3f(d.x, d.y, d.z);
                }
            }
            
                glEnd();
        }
    }
};
 
struct HalfEgg{
    float szelesseg;
    float resolution;
    float ks[4];
    float kd[4];
    float ka[4];
    float shininess[1];
    
    void setMaterial(Color inks, Color inkd, Color inka, int shine){
        ks[0] = inks.r;
        ks[1] = inks.g;
        ks[2] = inks.b;
        ks[3] = 1;
        kd[0] = inkd.r;
        kd[1] = inkd.g;
        kd[2] = inkd.b;
        kd[3] = 1;
        ka[0] = inka.r;
        ka[1] = inka.g;
        ka[2] = inka.b;
        ka[3] = 1;
        shininess[0] = shine;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
        glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
    
    HalfEgg(){}
    HalfEgg(float szeles, float felbontas){
        szelesseg = szeles;
        resolution = felbontas;
    }
    
    Vector whereIs(float inu, float inv){
        Vector temp;
        temp.x = szelesseg * cosf(inu) * sinf(inv);
        temp.y = szelesseg * sinf(inu) * sinf(inv);
        temp.z = szelesseg * cosf(inv);
        
        return temp;
    }
    
    Vector whereNorm(float inu, float inv){
        if (inv < delta)
            return Vector(0, 0, 1);
        if (inv > pi - delta)
            return Vector(0, 0, -1);
        Vector u, v;
        
        u.x = szelesseg * -sinf(inu) * sinf(inv);
        u.y = szelesseg * cosf(inu) * sinf(inv);
        u.z = 0;
        
        v.x = szelesseg * cosf(inu) * cosf(inv);
        v.y = szelesseg * sinf(inu) * cos(inv);
        v.z = szelesseg * -sinf(inv);
        
        return (v%u).Norm();
    }
    
    void draw(){
        setMaterial(Color(0.9, 0.9, 0.9), Color(1, 0.4588235294, 0.2196078431), Color(1, 0.4588235294, 0.2196078431), 20);
        float ui, vi;
        float ures = resolution;
        float vres = resolution;
        float u = 2 * pi / (ures - 1);
        float v = pi / (vres - 1);
        
        glBegin(GL_QUADS);
        
        for (int i = 0; i < ures / 2-1 ; i++){
            ui = 2 * pi*i / (ures - 1);
            for (int j = 0; j < vres -1; j++){
                vi = pi*j / (vres - 1);
                Vector a = whereIs(ui, vi);
                Vector aNorm = whereNorm(ui, vi);
                Vector b = whereIs(ui + u, vi);
                Vector bNorm = whereNorm(ui + u, vi);
                Vector c = whereIs(ui + u, vi + v);
                Vector cNorm = whereNorm(ui + u, vi + v);
                Vector d = whereIs(ui, vi + v);
                Vector dNorm = whereNorm(ui, vi + v);
                
                glNormal3f(aNorm.x, aNorm.y, aNorm.z);
                glVertex3f(a.x, a.y, a.z);
                
                
                glNormal3f(bNorm.x, bNorm.y, bNorm.z);
                glVertex3f(b.x, b.y, b.z);
                
                glNormal3f(cNorm.x, cNorm.y, cNorm.z);
                glVertex3f(c.x, c.y, c.z);
                
                glNormal3f(dNorm.x, dNorm.y, dNorm.z);
                glVertex3f(d.x, d.y, d.z);
            }
        }
        
        glEnd();
        
        glBegin(GL_TRIANGLE_FAN);
        
        for (int i = 0; i < 2; i++){
            ui = 2 * pi*i / (ures - 1);
            Vector a = whereIs(0, 0);
            Vector aNorm = whereNorm(0, 0);
            Vector b = whereIs(u + ui, 0);
            Vector bNorm = whereNorm(ui, 0);
            Vector c = whereIs(ui, 0);
            Vector cNorm = whereNorm(ui, 0);
            
            glNormal3f(aNorm.x, 0, aNorm.z);
            glVertex3f(a.x, 0, a.z);
            
            
            glNormal3f(bNorm.x, 0, bNorm.z);
            glVertex3f(b.x, 0, b.z);
            
            glNormal3f(cNorm.x, 0, cNorm.z);
            glVertex3f(c.x, 0, c.z);
        }
        
        glEnd();
    }
};
 
struct Cylinder{
    float szelesseg, magassag;
    float resolution;
    float ks[4];
    float kd[4];
    float ka[4];
    float shininess[1];
    
    void setMaterial(Color inks, Color inkd, Color inka, int shine){
        ks[0] = inks.r;
        ks[1] = inks.g;
        ks[2] = inks.b;
        ks[3] = 1;
        kd[0] = inkd.r;
        kd[1] = inkd.g;
        kd[2] = inkd.b;
        kd[3] = 1;
        ka[0] = inka.r;
        ka[1] = inka.g;
        ka[2] = inka.b;
        ka[3] = 1;
        shininess[0] = shine;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
        glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
    
    Cylinder(){}
    Cylinder(float szeles, float magas, float felbontas){
        szelesseg = szeles;
        magassag = magas;
        resolution = felbontas;
    }
    
    Vector whereIs(float inu, float inv){
        Vector temp;
        temp.x = szelesseg * cosf(inu);
        temp.y = inv;
        temp.z = szelesseg * sinf(inu);
        
        return temp;
    }
    
    Vector whereNorm(float inu, float inv){
        Vector u, v;
        
        u.x = szelesseg * -sinf(inu);
        u.y = 0;
        u.z = szelesseg * cosf(inu);
        
        v.x = 0;
        v.y = 1;
        v.z = 0;
        
        return (v%u).Norm();
    }
    
    void draw(){
        setMaterial(Color(0.9, 0.9, 0.9), Color(1, 0.4588235294, 0.2196078431), Color(1, 0.4588235294, 0.2196078431), 20);
        float ui, vi;
        float ures = resolution * 1.5;
        float vres = resolution;
        float u = 2 * pi / (ures - 1);
        float v = magassag / (vres - 1);
        
        glBegin(GL_QUADS);
        
        for (int i = 0; i < ures; i++){
            ui = 2 * pi*i / (ures - 1);
            for (int j = 0; j < vres; j++){
                vi = magassag*j / (vres - 1);
                Vector a = whereIs(ui, vi);
                Vector aNorm = whereNorm(ui, vi);
                Vector b = whereIs(ui + u, vi);
                Vector bNorm = whereNorm(ui + u, vi);
                Vector c = whereIs(ui + u, vi + v);
                Vector cNorm = whereNorm(ui + u, vi + v);
                Vector d = whereIs(ui, vi + v);
                Vector dNorm = whereNorm(ui, vi + v);
                
                glNormal3f(aNorm.x, aNorm.y, aNorm.z);
                glNormal3f(bNorm.x, bNorm.y, bNorm.z);
                glNormal3f(cNorm.x, cNorm.y, cNorm.z);
                glNormal3f(dNorm.x, dNorm.y, dNorm.z);
                
                glNormal3f(aNorm.x, aNorm.y, aNorm.z);
                glVertex3f(a.x, a.y, a.z);
                
                
                glNormal3f(bNorm.x, bNorm.y, bNorm.z);
                glVertex3f(b.x, b.y, b.z);
                
                glNormal3f(cNorm.x, cNorm.y, cNorm.z);
                glVertex3f(c.x, c.y, c.z);
                
                glNormal3f(dNorm.x, dNorm.y, dNorm.z);
                glVertex3f(d.x, d.y, d.z);
            }
        }
        
        glEnd();
    }
};
 
struct Cone{
    float magassag;
    Vector position;
    float resolution;
    float angle;
    float ks[4];
    float kd[4];
    float ka[4];
    float shininess[1];
    
    void setMaterial(Color inks, Color inkd, Color inka, int shine){
        ks[0] = inks.r;
        ks[1] = inks.g;
        ks[2] = inks.b;
        ks[3] = 1;
        kd[0] = inkd.r;
        kd[1] = inkd.g;
        kd[2] = inkd.b;
        kd[3] = 1;
        ka[0] = inka.r;
        ka[1] = inka.g;
        ka[2] = inka.b;
        ka[3] = 1;
        shininess[0] = shine;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
        glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
    
    Cone(){}
    Cone(float magas, float szog, float felbontas){
        magassag = magas;
        resolution = felbontas;
        angle = szog;
    }
    
    Vector whereIs(float inu, float inv){
        Vector temp;
        temp.x = inv * tanf(angle) * cosf(inu);
        temp.y = inv;
        temp.z = inv * tanf(angle) * sinf(inu);
        
        return temp;
    }
    
    Vector whereNorm(float inu, float inv){
        Vector u, v;
        
        u.x = inv * tanf(angle) * -sinf(inu);
        u.y = 0;
        u.z = inv * tanf(angle) * cosf(inu);
        
        v.x = tanf(angle) * cosf(inu);
        v.y = 1;
        v.z = tanf(angle) * sinf(inu);
        
        return (v%u).Norm();
    }
    
    void draw(){
        setMaterial(Color(0.9, 0.9, 0.9), Color(1, 0.6235294118, 0), Color(1, 0.6235294118, 0), 20);
        float ui, vi;
        float ures = resolution * 1.5;
        float vres = resolution;
        float u = 2 * pi / (ures - 1);
        float v = magassag / (vres - 1);
        
        glBegin(GL_QUADS);
        
        for (int i = 0; i < ures; i++){
            ui = 2 * pi*i / (ures - 1);
            for (int j = vres-1; j >= 0; j--){
                vi = magassag*j / (vres - 1);
                Vector a = whereIs(ui, vi);
                Vector aNorm = whereNorm(ui, vi);
                Vector b = whereIs(ui + u, vi);
                Vector bNorm = whereNorm(ui + u, vi);
                Vector c = whereIs(ui + u, vi + v);
                Vector cNorm = whereNorm(ui + u, vi + v);
                Vector d = whereIs(ui, vi + v);
                Vector dNorm = whereNorm(ui, vi + v);
                
                glNormal3f(aNorm.x, aNorm.y, aNorm.z);
                glVertex3f(a.x, a.y, a.z);
                
                
                glNormal3f(bNorm.x, bNorm.y, bNorm.z);
                glVertex3f(b.x, b.y, b.z);
                
                glNormal3f(cNorm.x, cNorm.y, cNorm.z);
                glVertex3f(c.x, c.y, c.z);
                
                glNormal3f(dNorm.x, dNorm.y, dNorm.z);
                glVertex3f(d.x, d.y, d.z);
            }
        }
        
        glEnd();
    }
};
 
struct Hedge;
struct Face;
struct Vertex;
 
struct Hedge{
    Vertex* endVertex;
    Face* borderedFace;
    Hedge* pairHedge;
    Hedge* nextHedge;
    Hedge* prevHedge;
    Vector middlePoint;
};
 
struct Vertex{
    Vector vector;
    Vector norm;
    Hedge* oneEmanatingHedge;
};
 
struct Face{
    Hedge* oneBorder;
    Vector norm;
    Vector middle;
};
 
struct Mesh{
    Vertex verts[14000];
    int vertsDb;
    Face faces[14000];
    int faceDb;
    Hedge edges[14000];
    
    Mesh(){
        vertsDb = 0;
        faceDb = 0;
    }
    
    void addVertex(Vector in){
        verts[vertsDb++].vector = in;
    }
    
    void setup(){
        for (int i = 0; i < vertsDb; i += 4){
            for (int j = 0; j < 4; j++){
                edges[j + i].endVertex = &verts[j + i];
                edges[j + i].borderedFace = &faces[faceDb];
                verts[j + i].oneEmanatingHedge = &edges[j + i];
                if (j == 3){
                    edges[i + j].nextHedge = &edges[i];
                }
                else{
                    edges[i + j].nextHedge = &edges[i + j + 1];
                }
                if (j == 0){
                    edges[i].prevHedge = &edges[i + 3];
                }
                else{
                    edges[i + j].prevHedge = &edges[i + j - 1];
                }
            }
            faces[faceDb].oneBorder = &edges[i];
            faces[faceDb].middle = (edges[i].endVertex->vector + edges[i].nextHedge->endVertex->vector + edges[i].nextHedge->nextHedge->endVertex->vector + edges[i].prevHedge->endVertex->vector) / 4;
            faces[faceDb++].norm = ((edges[i].prevHedge->endVertex->vector - edges[i].endVertex->vector) % (edges[i].nextHedge->endVertex->vector - edges[i].endVertex->vector)).Norm();
        }
        
        for (int i = 0; i < vertsDb; i++){
            for (int j = 0; j < vertsDb; j++){
                if (i != j && edges[i].endVertex->vector == edges[j].nextHedge->endVertex->vector && edges[j].endVertex->vector == edges[i].nextHedge->endVertex->vector){
                    edges[i].pairHedge = &edges[j];
                }
            }
            edges[i].middlePoint = (edges[i].endVertex->vector + edges[i].nextHedge->endVertex->vector) / 2;
        }
 
        for(int i = 0; i< vertsDb; i++){
            Hedge *v = verts[i].oneEmanatingHedge;
            Vector vec;
            int hatarololapok = 0;
            do{
                hatarololapok++;
                vec = vec + v->borderedFace->norm;
                v = v->pairHedge->nextHedge;
            } while (v != verts[i].oneEmanatingHedge);
            verts[i].norm = (vec/hatarololapok).Norm();
        }
    }
    
    void update(Vector vectors[], int vectorsDb){
        for (int i = 0; i < vertsDb; i++){
            verts[i].vector = Vector();
            verts[i].norm = Vector();
            verts[i].oneEmanatingHedge = NULL;
            
            edges[i].borderedFace = NULL;
            edges[i].endVertex = NULL;
            edges[i].middlePoint = Vector();
            edges[i].nextHedge = NULL;
            edges[i].pairHedge = NULL;
            edges[i].prevHedge = NULL;
        }
        
        for (int i = 0; i < faceDb; i++){
            faces[i].middle = Vector();
            faces[i].norm = Vector();
            faces[i].oneBorder = NULL;
        }
        
        faceDb = 0;
        vertsDb = 0;
        
        for (int i = 0; i < vectorsDb; i++){
            addVertex(vectors[i]);
        }
    }
};
 
struct Penguin{
    Mesh mesh;
    Vector position;
    Vector direction;
    float ks[4];
    float kd[4];
    float ka[4];
    float shininess[1];
    Cylinder leg1;
    Cylinder leg2;
    HalfEgg foot1;
    HalfEgg foot2;
    Egg eye1;
    Egg eye2;
    Cone mouth;
    bool sitting;
    bool iSvisible;
    long sittingFor;
    
    float whereIsUonTexture(float xTengely, float zTengely){
        if(zTengely > -delta || zTengely < delta)
            return 1 / (2.0 * pi)*(atan2f(xTengely+3*delta, xTengely+3*delta) + pi);
        return 1 / (2.0 * pi)*(atan2f(zTengely+3*delta, xTengely+3*delta) + pi);
    }
    
    float whereIsVonTexture(float magas){
        return magas / 0.923;
    }
    
    void setMaterial(Color inks, Color inkd, Color inka, int shine){
        ks[0] = inks.r;
        ks[1] = inks.g;
        ks[2] = inks.b;
        ks[3] = 1;
        kd[0] = inkd.r;
        kd[1] = inkd.g;
        kd[2] = inkd.b;
        kd[3] = 1;
        ka[0] = inka.r;
        ka[1] = inka.g;
        ka[2] = inka.b;
        ka[3] = 1;
        shininess[0] = shine;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
        glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }
    
    void subdivide(){
        Vector pontok[11000];
        int pontokDb = 0;
        for (int i = 0; i < mesh.vertsDb; i++){
            Hedge *h = &mesh.edges[i];
            Vector piros;
            Vector zoldsum;
            int hatarololapok = 0;
            do{
                hatarololapok++;
                piros = piros + h->borderedFace->middle;
                zoldsum = zoldsum + h->middlePoint;
                h = h->pairHedge->nextHedge;
            } while (h != &mesh.edges[i]);
            
            Vector Sarga = (piros)*(1.0 / powf(hatarololapok, 2)) +
            (zoldsum)*(2.0 / powf(hatarololapok, 2)) + (mesh.edges[i].endVertex->vector)*(((float)hatarololapok - 3) / hatarololapok);
            Vector Zold = (mesh.edges[i].borderedFace->middle + mesh.edges[i].pairHedge->borderedFace->middle)*(powf(1.0 / 2, 2)) + (mesh.edges[i].middlePoint)*(1.0 / 2);
            Vector Zold2 = (mesh.edges[i].borderedFace->middle + mesh.edges[i].prevHedge->pairHedge->borderedFace->middle)*(powf(1.0 / 2, 2)) + (mesh.edges[i].prevHedge->middlePoint)*(1.0 / 2);
            pontok[pontokDb++] = Sarga;
            pontok[pontokDb++] = Zold;
            pontok[pontokDb++] = mesh.edges[i].borderedFace->middle;
            pontok[pontokDb++] = Zold2;
        }
        
        mesh.update(pontok, pontokDb);
        mesh.setup();
    }
 
    void buildTexture(Color dark, Color light){
        Color texture[1000];
        for(int i = 0; i < 16;i++){
            for(int j = 0; j < 16; j++){
                if(j>2 && j < 12 && i > 4  && i< 12)texture[j*16+i] = light;
                else texture[j*16+i] = dark;
            }
        }
        
        makeTexturePenguin(texture, 16);
    }
    
    void setup(int subdivision){
        position = Vector(0, 1, 0);
        direction = Vector(0,0,-1);
        //setMaterial(Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), 20);
        Vector a = Vector(0.323, 0, 0.38);
        Vector b = Vector(-0.277, 0, 0.38);
        Vector c = Vector(-0.277, 0, -0.38);
        Vector d = Vector(0.323, 0, -0.38);
        Vector e = Vector(0.281, 0.81, 0.227);
        Vector f = Vector(-0.235, 0.81, 0.227);
        Vector g = Vector(-0.235, 0.81, -0.227);
        Vector h = Vector(0.281, 0.81, -0.227);
        Vector i = Vector(0.161, 0.923, 0.118);
        Vector j = Vector(-0.07, 0.923, 0.118);
        Vector k = Vector(-0.07, 0.923, -0.118);
        Vector l = Vector(0.161, 0.923, -0.118);
        
        mesh.addVertex(a);
        mesh.addVertex(e);
        mesh.addVertex(h);
        mesh.addVertex(d);
        
        mesh.addVertex(b);
        mesh.addVertex(f);
        mesh.addVertex(e);
        mesh.addVertex(a);
        
        mesh.addVertex(c);
        mesh.addVertex(g);
        mesh.addVertex(f);
        mesh.addVertex(b);
        
        mesh.addVertex(d);
        mesh.addVertex(h);
        mesh.addVertex(g);
        mesh.addVertex(c);
        
        mesh.addVertex(e);
        mesh.addVertex(i);
        mesh.addVertex(l);
        mesh.addVertex(h);
        
        mesh.addVertex(f);
        mesh.addVertex(j);
        mesh.addVertex(i);
        mesh.addVertex(e);
        
        mesh.addVertex(g);
        mesh.addVertex(k);
        mesh.addVertex(j);
        mesh.addVertex(f);
        
        mesh.addVertex(h);
        mesh.addVertex(l);
        mesh.addVertex(k);
        mesh.addVertex(g);
        
        mesh.addVertex(i);
        mesh.addVertex(j);
        mesh.addVertex(k);
        mesh.addVertex(l);
        
        mesh.addVertex(a);
        mesh.addVertex(d);
        mesh.addVertex(c);
        mesh.addVertex(b);
        
        build(subdivision);
    }
    
    Penguin(){}
    Penguin(int subdivision){
        sitting = false;
        position = Vector(0, 1, 0);
        direction = Vector(0,0,-1);
        Vector a = Vector(0.323, 0, 0.38);
        Vector b = Vector(-0.277, 0, 0.38);
        Vector c = Vector(-0.277, 0, -0.38);
        Vector d = Vector(0.323, 0, -0.38);
        Vector e = Vector(0.281, 0.81, 0.227);
        Vector f = Vector(-0.235, 0.81, 0.227);
        Vector g = Vector(-0.235, 0.81, -0.227);
        Vector h = Vector(0.281, 0.81, -0.227);
        Vector i = Vector(0.161, 0.923, 0.118);
        Vector j = Vector(-0.07, 0.923, 0.118);
        Vector k = Vector(-0.07, 0.923, -0.118);
        Vector l = Vector(0.161, 0.923, -0.118);
        
        mesh.addVertex(a);
        mesh.addVertex(e);
        mesh.addVertex(h);
        mesh.addVertex(d);
        
        mesh.addVertex(b);
        mesh.addVertex(f);
        mesh.addVertex(e);
        mesh.addVertex(a);
        
        mesh.addVertex(c);
        mesh.addVertex(g);
        mesh.addVertex(f);
        mesh.addVertex(b);
        
        mesh.addVertex(d);
        mesh.addVertex(h);
        mesh.addVertex(g);
        mesh.addVertex(c);
        
        mesh.addVertex(e);
        mesh.addVertex(i);
        mesh.addVertex(l);
        mesh.addVertex(h);
        
        mesh.addVertex(f);
        mesh.addVertex(j);
        mesh.addVertex(i);
        mesh.addVertex(e);
        
        mesh.addVertex(g);
        mesh.addVertex(k);
        mesh.addVertex(j);
        mesh.addVertex(f);
        
        mesh.addVertex(h);
        mesh.addVertex(l);
        mesh.addVertex(k);
        mesh.addVertex(g);
        
        mesh.addVertex(i);
        mesh.addVertex(j);
        mesh.addVertex(k);
        mesh.addVertex(l);
        
        mesh.addVertex(a);
        mesh.addVertex(d);
        mesh.addVertex(c);
        mesh.addVertex(b);
        
        build(subdivision);
    }
    
    void build(int resolution){
        mesh.setup();
        leg1 = Cylinder(0.055, 0.2, 9);
        leg2 = Cylinder(0.055, 0.2, 9);
        foot1 = HalfEgg(0.1, 9);
        foot2 = HalfEgg(0.1, 9);
        mouth = Cone(0.1, 60, 9);
        eye1 = Egg(0.04, 0.04, 9, Vector(0,0,0));
        eye2 = Egg(0.04, 0.04, 9, Vector(0,0,0));
        for(int i = 0; i < resolution; i++){
            subdivide();
        }
    }
    
    void draw(){
        eye1.iSvisible = true;
        eye2.iSvisible = true;
        if(iSvisible){
            glPushMatrix();
            glRotatef(90, 0, 1, 0);
            glPushMatrix();
            
            Vector temp;
            if(sitting)glTranslatef(0, -0.1, 0);
            else glTranslatef(0, 0.06, 0);
 
            buildTexture(Color(0, 0, 0), Color(1, 1, 1));
            glRotatef(-90, 0, 1, 0);
            for (int i = 0; i < mesh.faceDb; i++){
                Vector A = mesh.faces[i].oneBorder->endVertex->vector;
                Vector B = mesh.faces[i].oneBorder->nextHedge->endVertex->vector;
                Vector C = mesh.faces[i].oneBorder->nextHedge->nextHedge->endVertex->vector;
                Vector D = mesh.faces[i].oneBorder->prevHedge->endVertex->vector;
 
                glBegin(GL_QUADS);
                
                glNormal3f(mesh.faces[i].oneBorder->endVertex->norm.x, mesh.faces[i].oneBorder->endVertex->norm.y, mesh.faces[i].oneBorder->endVertex->norm.z);
                temp = (A - Vector(0,A.y,0)).Norm()*0.76+Vector(0,A.y,0);
                glTexCoord2f(whereIsUonTexture(temp.x, temp.z), whereIsVonTexture(temp.y));
                glVertex3f(A.x, A.y, A.z);
                
                glNormal3f(mesh.faces[i].oneBorder->nextHedge->endVertex->norm.x, mesh.faces[i].oneBorder->nextHedge->endVertex->norm.y, mesh.faces[i].oneBorder->nextHedge->endVertex->norm.z);
                temp = (B - Vector(0,B.y,0)).Norm()*0.76+Vector(0,B.y,0);
                glTexCoord2f(whereIsUonTexture(temp.x, temp.z), whereIsVonTexture(temp.y));
                glVertex3f(B.x, B.y, B.z);
                
                glNormal3f(mesh.faces[i].oneBorder->nextHedge->nextHedge->endVertex->norm.x, mesh.faces[i].oneBorder->nextHedge->nextHedge->endVertex->norm.y, mesh.faces[i].oneBorder->nextHedge->nextHedge->endVertex->norm.z);
                temp = (C - Vector(0,C.y,0)).Norm()*0.76+Vector(0,C.y,0);
                glTexCoord2f(whereIsUonTexture(temp.x, temp.z), whereIsVonTexture(temp.y));
                glVertex3f(C.x, C.y, C.z);
                
                glNormal3f(mesh.faces[i].oneBorder->prevHedge->endVertex->norm.x, mesh.faces[i].oneBorder->prevHedge->endVertex->norm.y, mesh.faces[i].oneBorder->prevHedge->endVertex->norm.z);
                temp = (D - Vector(0,D.y,0)).Norm()*0.76+Vector(0,D.y,0);
                glTexCoord2f(whereIsUonTexture(temp.x, temp.z), whereIsVonTexture(temp.y));
                glVertex3f(D.x, D.y, D.z);
                
                glEnd();
            }
            
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
            
            glPushMatrix();
            if(sitting)glTranslatef(0, -0.1, 0);
            else glTranslatef(0, 0.06, 0);
            
            glPushMatrix();
            glTranslatef(0, 0.75, 0.33);
            glRotatef(180, 0, 1, 0);
            glRotatef(90, 1, 0, 0);
            mouth.draw();
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(-0.065, 0.81, 0.19);
            eye1.draw();
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0.065, 0.81, 0.19);
            eye2.draw();
            glPopMatrix();
            
            glPopMatrix();
 
            glPushMatrix();
            glTranslatef(-0.12, 0, 0.05);
            leg1.draw();
            foot1.draw();
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0.12, 0, 0.05);
            leg2.draw();
            foot2.draw();
            glPopMatrix();
            glPopMatrix();
        }
    }
};
 
Penguin player(2);
 
void onInitialization() {
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 1000);
}
 
struct Scene{
    Penguin penguin[10];
    Egg tojasok[10];
    int db;
    
    Scene(){
        db = 0;
        player.iSvisible = true;
        player.sitting = false;
        for(int i = 0; i < 10; i++){
            tojasok[i].setup(0.5, 0.7, 9, Vector(rand()%100-50, 0, rand()%100-50));
            tojasok[i].iSvisible = true;
            penguin[i].setup(2);
            penguin[i].position = tojasok[i].position;
            penguin[i].iSvisible = false;
        }
    }
    
    void logic(){
        if(player.sitting && (3000 < (glutGet(GLUT_ELAPSED_TIME)-player.sittingFor))){
            penguin[db].iSvisible = true;
            tojasok[db].iSvisible = false;
            player.sitting = false;
            db++;
            glutPostRedisplay();
        }
    }
    
    void draw(){
        for (int i = 0; i < 10; i++) {
            if(tojasok[i].iSvisible){
                glPushMatrix();
                glTranslatef(tojasok[i].position.x, tojasok[i].position.y+3, tojasok[i].position.z);
                glScalef(5, 5, 5);
                tojasok[i].draw();
                glPopMatrix();
            }
            
            if(penguin[i].iSvisible){
                glPushMatrix();
                glTranslatef(penguin[i].position.x, penguin[i].position.y, penguin[i].position.z);
                glScalef(20, 20, 20);
                penguin[i].draw();
                glPopMatrix();
            }
        }
    }
};
 
Scene scene;
 
void onDisplay() {
    glClearColor(0.5294117647, 0.8078431373, 0.9803921569, 0.8f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(eye.x, eye.y, eye.z, player.position.x, player.position.y+10, player.position.z, 0, 1, 0);
    
    LightSource l(Vector(500, 500, 0));
    Plane p(Vector(0, 1, 0));
    p.draw();
    
    scene.draw();
    
    glPushMatrix();
    glTranslatef(player.position.x, player.position.y, player.position.z);
    glRotatef(-rotate + 90, 0, 1, 0);
    
    glScalef(20, 20, 20);
    player.draw();
    glPopMatrix();
 
    
    glDisable(GL_LIGHTING);
    float translationMatrix[4][4] = {
        { 1, 0, 0, 0 },
        { -l.position[0] / l.position[1], 0, -l.position[2] / l.position[1], 0 },
        { 0, 0, 1, 0 },
        { 0, delta, 0, 1 }
    };
    glMultMatrixf(&translationMatrix[0][0]);
    glColor3f(0, 0, 0);
    glDisable(GL_TEXTURE_2D);
    scene.draw();
 
    glPushMatrix();
    glTranslatef(player.position.x, player.position.y, player.position.z);
    glRotatef(-rotate + 90, 0, 1, 0);
 
    glScalef(20, 20, 20);
    player.draw();
    glPopMatrix();
    glutSwapBuffers();
    
}
 
void setDir(){
    if (rotate > 360) {
        rotate -= 360;
    }
    if (rotate < 0) {
        rotate += 360;
    }
    if (rotate == 0 || rotate == 360) {
        player.direction = Vector(0,0,-1);
        rotate = 0;
    }
    if (rotate == 45) {
        player.direction = Vector(1,0,-1);
    }
    if (rotate == 90) {
        player.direction = Vector(1,0,0);
    }
    if (rotate == 135) {
        player.direction = Vector(1,0,1);
    }
    if (rotate == 180) {
        player.direction = Vector(0,0,1);
    }
    if (rotate == 225) {
        player.direction = Vector(-1,0,1);
    }
    if (rotate == 270) {
        player.direction = Vector(-1,0,0);
    }
    if (rotate == 315) {
        player.direction = Vector(-1,0,-1);
    }
}
 
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'w') {
        rotate += 45;
        setDir();
        glutPostRedisplay();
    }
    
    if (key == 'r') {
        rotate -= 45;
        setDir();
        glutPostRedisplay();
    }
    
    if (key == 'e') {
        player.position = player.position + player.direction;
        glutPostRedisplay();
    }
    
    if (key == ' ') {
        player.sittingFor = glutGet(GLUT_ELAPSED_TIME);
        player.sitting = !player.sitting;
        glutPostRedisplay();
    }
}
 
void onKeyboardUp(unsigned char key, int x, int y) {
    
}
 
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        glutPostRedisplay();
}
 
void onMouseMotion(int x, int y){
    
}
 
void onIdle() {
    scene.logic();
}
 
// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv);                 // GLUT inicializalasa
    glutInitWindowSize(600, 600);            // Alkalmazas ablak kezdeti merete 600x600 pixel
    glutInitWindowPosition(100, 100);            // Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    // 8 bites R,G,B,A + dupla buffer + melyseg buffer
    
    glutCreateWindow("Grafika hazi feladat");        // Alkalmazas ablak megszuletik es megjelenik a kepernyon
    
    glMatrixMode(GL_MODELVIEW);                // A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);            // A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    
    onInitialization();                    // Az altalad irt inicializalast lefuttatjuk
    
    glutDisplayFunc(onDisplay);                // Esemenykezelok regisztralasa
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);
    
    glutMainLoop();                    // Esemenykezelo hurok
    
    return 0;
}