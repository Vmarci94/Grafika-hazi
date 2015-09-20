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
// Nev    : Szabó Tamás
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
 
struct Vector {
    float x, y, z;
    
    Vector( ) {
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
        return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }
    float Length() {
        return sqrt(x * x + y * y + z * z);
    }
    Vector Norm() {
        return *this / Length();
    }
};
 
struct Color {
    float r, g, b;
    
    Color( ) {
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
        return Color(powf(r, c), powf(g, c),powf(b, c));
    }
    Color operator+(const Color& c) {
        return Color(r + c.r, g + c.g, b + c.b);
    }
    Color operator-(const Color& c) {
        return Color(r - c.r, g - c.g, b - c.b);
    }
};
 
Vector colorToVector(Color c){
    Vector temp;
    temp.x = c.r;
    temp.y = c.b;
    temp.z = c.g;
    return temp;
}
 
Color vectorToColor(Vector v){
    Color temp;
    temp.r = v.x;
    temp.b = v.y;
    temp.g = v.z;
    return temp;
}
 
float pi = 3.1415926536;
float delta = 0.001f;
int maxDepth = 3;
const int screenWidth = 600;
const int screenHeight = 600;
Vector blackHoleCenter;
Color image[screenWidth*screenHeight];
Color kep[600][600];
 
Color diffuse(Color kd, Vector pos, bool diff){
    if(!diff)
        return kd;
    else
        return (int)(pos * 10).Length() % 2 ? kd * (0.8f) : kd * (0.2f);
}
 
struct Ray{
    Vector eye;
    Vector v;
    Ray(Vector eyein, Vector dir){
        eye = eyein;
        v = dir;
    }
};
 
struct Light{
    Vector O;
    Color color;
    Light(Vector origo = Vector(), Color szin = Color(1.0f, 1.0f, 1.0f)){
        O = origo;
        color = szin;
    }
    Color gyengules(float position){
        return color / (powf(position, 2));
    }
};
 
struct Material{
    Color F0;
    Color n;
    Color ks;
    Color kd;
    Color ka;
    float shine;
    bool isReflect;
    bool isDiffuse;
    Material(Color nin, Color kin, Color ksin, Color kdin, Color kain, float shininess, bool reflect, bool isDiffuse){
        n= nin;
        ks = ksin;
        kd = kdin;
        ka = kain;
        shine = shininess;
        isReflect = reflect;
        this->isDiffuse = isDiffuse;
        F0 = (((n - Color(1.0f, 1.0f, 1.0f)) ^ 2.0f) + (kin ^ 2.0f)) / (((n + Color(1.0f, 1.0f, 1.0f)) ^ 2.0f) + (kin ^ 2.0f));
    }
    Vector reflect(Vector invec, Vector norm){
        return invec - norm * (norm * invec) * 2.0f;
    }
    Color Fresnel(Vector invec, Vector norm){
        float cosa = fabsf((norm * invec));
        return F0 + (Color(1.0f, 1.0f, 1.0f) - F0) * powf(1.0f - cosa, 5.0f);
    }
    Color shade(Vector normal, Vector viewDir, Vector lightDir, Vector inRad, Vector pos){
        Color reflRad(0.0f, 0.0f, 0.0f);
        float cosTheta = normal * lightDir;
        if(cosTheta < 0)
            return reflRad;
        reflRad = vectorToColor(inRad) * diffuse(kd, pos, isDiffuse) * cosTheta;
        Vector halfway = (viewDir + lightDir).Norm();
        float cosDelta = normal * halfway;
        if(cosDelta < 0)
            return reflRad;
        return reflRad + vectorToColor(inRad) * ks * powf(cosDelta,shine);
    }
};
 
struct Hit {
    float t;
    Vector position;
    Vector normal;
    Material* material;
    Hit() {
        t = -1;
    }
};
 
struct Intersectable{
    Material* material;
    virtual Hit ussagyon(Ray ray) = 0;
};
 
Intersectable* object[100];
int objectdb = 0;
Light fenyek[5];
int fenydb = 0;
 
Hit firstIntersect(Ray ray) {
    Hit eddigJo;
    for(int i = 0; i < objectdb; i++) {
        Hit hit = object[i]->ussagyon(ray);
        if(hit.t > 0 && (eddigJo.t == -1 || hit.t < eddigJo.t))
            eddigJo = hit;
    }
    return eddigJo;
}
 
Hit firstIntersectWithBlackHole(Ray ray){
    Ray newEye = ray;
    Hit eddigJo, temp;
    float deltaS = 0.035f;
    
    for (float lepes = 0; lepes < 7; lepes += deltaS){
        for (int i = 0; i < objectdb; i++){
            temp = object[i]->ussagyon(newEye);
            if(temp.t != -1 && (eddigJo.t == -1 || eddigJo.t > temp.t)){
                eddigJo = temp;
            }
        }
        if ((newEye.eye - blackHoleCenter).Length() <= 0.16f){
            return eddigJo;
        }
        if ((eddigJo.position - newEye.eye).Length() < deltaS){
            return eddigJo;
        }
        float r = (blackHoleCenter - newEye.eye).Length();
        Vector dVesszo = newEye.v * deltaS + ((blackHoleCenter - newEye.eye) / (r)) * 1 / 2 * 2 * 0.16f * (powf(deltaS, 2) / (powf(r, 2)));
        newEye.v = dVesszo.Norm();
        newEye.eye = newEye.eye + newEye.v * deltaS;
    }
    return Hit();
}
 
struct Triangle: public Intersectable{
    Vector a,b,c,n;
    Triangle(){
    };
    Triangle(Vector r1, Vector r2, Vector r3, Material* mat){
        a = r1;
        b = r2;
        c = r3;
        material = mat;
        n = ((b - a) % (c - a)).Norm();
    }
    bool haromszog_e(Triangle t, Vector p){
        return (((t.b - t.a) % (p - t.a) * t.n) >= 0 && ((t.c - t.b) % (p - t.b) * t.n) >= 0 && ((t.a - t.c) % (p - t.c) * t.n) >= 0);
    }
    Hit ussagyon(Ray ray){
        float t = ((a - ray.eye) * n) / (ray.v * n);
        Vector p = ray.eye+(ray.v*t);
        if (t < 0){
            return Hit();
        }
        if(!haromszog_e(*this, p)){
            return Hit();
        }
        Hit temp;
        temp.material = material;
        temp.t = t;
        temp.normal = n;
        temp.position = p;
        return temp;
    }
};
 
struct Square: public Intersectable{
    Triangle minitri[2];
    Square(Vector p1, Vector p2, Vector p3, Vector p4, Material* mat){
        minitri[0] = Triangle(p1, p3, p2, mat);
        minitri[1] = Triangle(p1, p4, p3, mat);
    }
    Hit ussagyon(Ray ray){
        Hit eddigJo;
        for (int i = 0; i < 2; i++) {
            Hit h = minitri[i].ussagyon(ray);
            if(h.t != -1 && (eddigJo.t == -1 || eddigJo.t > h.t))
                eddigJo = h;
        }
        return eddigJo;
    }
};
 
struct Sphere: public Intersectable{
    Vector O;
    float R;
    Sphere(Vector center, float radius, Material* mat, bool isBlackHole){
        O = center;
        R = radius;
        material = mat;
        if (isBlackHole) {
            blackHoleCenter = center;
        }
    }
    Hit ussagyon(Ray ray){
        Hit eddigJo;
        float a = ray.v ^ 2;
        float b = 2 * ((ray.eye - O) * ray.v);
        float c = ((ray.eye - O) ^ 2) - (R * R);
        float D = powf(b, 2) - (4 * a * c);
        if (D < 0)
            return eddigJo;
        float x1 = (b * (-1.0f) + sqrtf(D)) / (2 * a);
        float x2 = (-1.0f * b - sqrtf(D)) / (2 * a);
        float t;
        if (x1 < (0 + delta))
            t = x2;
        else if (x2 < (0 + delta))
            t = x1;
        else
            t = x1 < x2 ? x1 : x2;
        if (t > (0 - delta)){
            eddigJo.t = t;
            eddigJo.position = ray.eye + ray.v * t;
            eddigJo.normal = (eddigJo.position - O).Norm();
            eddigJo.material = material;
            return eddigJo;
        }
        return eddigJo;
    }
};
 
struct Torus: public Intersectable{
    float R, r;
    Vector O;
    Triangle minitri[100];
    int tridb;
    float u, ui, vi, v;
    int felbontas, masikfelbontas;
    Vector miket(float uin, float vin){
        Vector temp;
        temp.x = (R + r * cosf(uin)) * cosf(vin) + O.x;
        temp.y = (R + r * cosf(uin)) * sinf(vin) + O.y;
        temp.z = r * sinf(uin) + O.z;
        return temp;
    }
    Torus(Vector origo,float Rin, float rin, int resolution, int masikresolution, Material* mat){
        O = origo;
        tridb = 0;
        R = Rin;
        r = rin;
        felbontas = resolution;
        masikfelbontas = masikresolution;
        u = (2.0f * pi)/(float)masikfelbontas;
        v = (2.0f * pi)/(float)felbontas;
        for (int i = 0; i < masikfelbontas; i++){
            ui = ((float)i / (float)masikfelbontas) * (2 * pi);
            for (int j = 0; j < felbontas; j++){
                vi = ((float)j / (float)felbontas) * (2 * pi);
                Vector p1 = miket(ui, vi);
                Vector p2 = miket(ui + u, vi);
                Vector p3 = miket(ui, vi + v);
                Vector p4 = miket(ui + u, vi + v);
                minitri[tridb++] = Triangle(p3, p2, p1, mat);
                minitri[tridb++] = Triangle(p3, p4, p2, mat);
            }
        }
    }
    Hit ussagyon(Ray ray){
        Hit eddigJo;
        for (int i = 0; i < tridb; i++) {
            Hit temp = minitri[i].ussagyon(ray);
            if(temp.t != -1 && (eddigJo.t == -1 || eddigJo.t > temp.t))
                eddigJo = temp;
        }
        return eddigJo;
    }
};
 
Color trace(Ray ray, int depth) {
    Color outRadiance(0.0f, 0.0f, 0.0f);
    if(depth > maxDepth)
        return outRadiance;
    Hit hit = firstIntersectWithBlackHole(ray);
    if(hit.t < 0)
        return outRadiance;
    for(int i = 0; i < fenydb; i++){
        Ray shadowRay(hit.position + hit.normal * delta, fenyek[i].O - hit.position);
        Hit shadowHit = firstIntersect(shadowRay);
        if(shadowHit.t < 0 || shadowHit.t > (fenyek[i].O-hit.position).Length())
            outRadiance = outRadiance + hit.material->shade(hit.normal, (ray.v)*(-1), fenyek[i].O - hit.position, colorToVector(fenyek[i].gyengules(shadowHit.t)), hit.position);
    }
    if(hit.material->isReflect){
        Vector reflectionDir = hit.material->reflect(ray.v, hit.normal);
        Ray reflectedRay(hit.position + hit.normal * delta, reflectionDir);
        outRadiance = outRadiance + trace(reflectedRay,depth + 1)*hit.material->Fresnel(ray.v, hit.normal);
    }
    return outRadiance;
}
 
 
void onInitialization( ) {
    glViewport(0, 0, screenWidth, screenHeight);
    Vector eye = Vector(0.0f, 0.0f, -2.0f);
    Vector v;
    
    fenyek[fenydb++] = Light(Vector(0.4f, -0.7f, -1.0f), Color(0.5f, 0.5f, 0.5f));
    fenyek[fenydb++] = Light(Vector(-0.7f, 0.6f, -1.0f), Color(0.35f, 0.35f, 0.35f));
    
    Material gold(Color(0.17f, 0.35f, 1.5f), Color(3.1f, 2.7f, 1.9f), Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), 30.0f, true, false);
    Material piros(Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f), Color(1.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), 30.0f, false, true);
    Material feher(Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 0.0f), 30.0f, false, true);
    Material zold(Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f), Color(0.0f, 1.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), 30.0f, false, true);
    Material kek(Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f), Color(0.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 0.0f), 30.0f, false, true);
    Material pink(Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f), Color(1.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 0.0f), 30.0f, false, true);
    Material black(Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f), 0.0f, false, false);
    
    object[objectdb++] = new Square(Vector(-1.0f, 1.0f, 1.0f), Vector(1.0f, 1.0f, 1.0f), Vector(1.0f, 1.0f, -1.0f), Vector(-1.0f, 1.0f, -1.0f), &piros);
    object[objectdb++] = new Square(Vector(-1.0f, -1.0f, -1.0f), Vector(1.0f, -1.0f, -1.0f), Vector(1.0f, -1.0f, 1.0f), Vector(-1.0f, -1.0f, 1.0f), &pink);
    object[objectdb++] = new Square(Vector(-1.0f, -1.0f, -1.0f), Vector(-1.0f, -1.0f, 1.0f), Vector(-1.0f, 1.0f, 1.0f), Vector(-1.0f, 1.0f, -1.0f), &zold);
    object[objectdb++] = new Square(Vector(1.0f, -1.0f, 1.0f), Vector(1.0f, -1.0f, -1.0f), Vector(1.0f, 1.0f, -1.0f), Vector(1.0f, 1.0f, 1.0f), &feher);
    object[objectdb++] = new Square(Vector(-1.0f,-1.0f,1.0f), Vector(1.0f,-1.0f,1.0f), Vector(1.0f,1.0f,1.0f), Vector(-1.0f,1.0f,1.0f), &kek);
    object[objectdb++] = new Torus(Vector(0.4f, -0.2f, 0.8f), 0.3f, 0.15f, 3, 3, &gold);
    object[objectdb++] = new Sphere(Vector(-0.2f, -0.5f, 0.5f), 0.16f, &black, true);
    
    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            kep[i][j] = trace(Ray(eye, (Vector((i / 300.0f - 1.0f), (j / 300.0f - 1.0f), 0.0f) - eye).Norm()), 0);
        }
    }
    int db = 0;
    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenWidth; j++) {
            image[db++] = kep[j][i];
        }
    }
}
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);
    glutSwapBuffers();
}
 
void onKeyboard(unsigned char key, int x, int y) {
    
}
 
void onKeyboardUp(unsigned char key, int x, int y) {
    
}
 
void onMouse(int button, int state, int x, int y) {
    
}
 
void onMouseMotion(int x, int y)
{
    
}
 
void onIdle( ) {
    
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
