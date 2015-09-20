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
 
//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
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
    float operator*(const Vector& v) {     // dot product
        return (x * v.x + y * v.y + z * v.z);
    }
    Vector operator%(const Vector& v) {     // cross product
        return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }
    float Length() { return sqrt(x * x + y * y + z * z); }
};
 
//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
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
    Color operator+(const Color& c) {
        return Color(r + c.r, g + c.g, b + c.b);
    }
};
 
const int screenWidth = 600;    // alkalmaz�s ablak felbont�sa
const int screenHeight = 600;
 
 
bool spaceWasPressed = false;
bool viewportChanged = false;
unsigned char viewPortPos = 'n';
 
Vector points[100];
int elemszam = 0;
Vector hermiteTemp;
 
float times[100];
float time = 0.0f;
 
int vonalResolution = 50.0f;
 
void timeCalculator(){
    for (int i = 0; i < elemszam-1; i++) {
        times[i]=0;
    }
    float sum = 0;
    float ut = 0;
    float ido = 0;
    float sebesseg = 1.0f;
    
    for (int i = 1; i < elemszam; i++) {
        ut = (points[i] - points[i - 1]).Length();
        ido = ut / sebesseg;
        sum = sum + ido;
        times[i] = sum;
    }
}
 
void pontColorSet(){
    glColor3f(1.0f, 1.0f, 0.0f);
}
 
void vonalColorSet(){
    glColor3f(1.0f, 1.0f, 1.0f);
}
 
Vector hermiteDerivate(Vector pi, Vector vi, Vector pi1, Vector vi1, float ti, float ti1, float t){
    Vector a1 = vi;
    Vector a2 = ((pi1 - pi) * 3 * (1 / powf((ti1 - ti), 2))) - ((vi1 + vi * 2)* (1 / (ti1 - ti)));
    Vector a3 = (((pi - pi1) * 2)*(1 / (powf(ti1 - ti, 3)))) + ((vi1 + vi)*(1 / (powf(ti1 - ti, 2))));
    
    return a3 * powf(t - ti, 2) * 3 + a2 * (t - ti) * 2 + a1;
}
 
Vector hermiteInterpolate(Vector pi, Vector vi, Vector pi1, Vector vi1, float ti, float ti1, float t){
    Vector a0 = pi;
    Vector a1 = vi;
    Vector a2 = ((pi1 - pi) * 3 * (1 / powf((ti1 - ti), 2))) - ((vi1 + vi * 2)* (1 / (ti1 - ti)));
    Vector a3 = (((pi - pi1) * 2)*(1 / (powf(ti1 - ti, 3)))) + ((vi1 + vi)*(1 / (powf(ti1 - ti, 2))));
    
    return a3 * powf(t - ti, 3) + a2 * powf(t - ti, 2) + a1 * (t - ti) + a0;
}
 
Vector rennerGaborSpline(int i){
    Vector renner = Vector(0.0f,0.0f,0.0f);
    float terimin1 = 0.0000001f, teri = 0.0000001f;
    if (i>=2) {
        terimin1 = ((points[i-1] - points[i-2]) % (points[i-1] - points[i])).Length() / 2;
    }
    if (i <= elemszam-3) {
        teri = ((points[i] - points[i+1]) % (points[i+1] - points[i+2])).Length() / 2;
    }
    if (i >= 1 && (teri + terimin1)>0) {
        renner = ((points[i]-points[i-1])*teri)*(1/(teri+terimin1));
    }
    if (i<= elemszam-2 && (terimin1 + teri)>0) {
        renner = renner + ((points[i+1]-points[i])*terimin1)*(1/(terimin1+teri));
    }
    renner = renner * (1/renner.Length()+0.0001f);
    
    return renner;
}
 
float szogToRad(float angle){
    return (3.1415926536 / 180.0f)*angle;
}
 
void drawCircle(float vizszintesPozicio, float fuggolegesPozicio, float r, float resolution){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(vizszintesPozicio, fuggolegesPozicio);
    float szog = szogToRad(360.0f / resolution);
    for (int i = 0; i < resolution + 1; i++) {
        glVertex2f(vizszintesPozicio + r * cosf(szog * i), fuggolegesPozicio + r * sinf(szog *i));
    }
    glEnd();
}
 
void xVonalKirajzol(){
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i<elemszam - 1; i++){
        for (float t = times[i]; t <= times[i + 1]; t += 1.0f / vonalResolution){
            hermiteTemp = hermiteInterpolate(points[i], rennerGaborSpline(i), points[i + 1], rennerGaborSpline(i+1), times[i], times[i + 1], t);
            glVertex2f(-hermiteTemp.z, hermiteTemp.y);
        }
    }
    glEnd();
}
 
void xPontKirajzol(){
    for (int i = 0; i < elemszam; i++) {
        drawCircle(-points[i].z, points[i].y, 0.02f, 16);
    }
}
 
void xViewKirajzol(){
    pontColorSet();
    xPontKirajzol();
    
    vonalColorSet();
    xVonalKirajzol();
}
 
void yVonalKirajzol(){
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i<elemszam - 1; i++){
        for (float t = times[i]; t <= times[i + 1]; t += 1.0f / vonalResolution){
            hermiteTemp = hermiteInterpolate(points[i], rennerGaborSpline(i), points[i + 1], rennerGaborSpline(i+1), times[i], times[i + 1], t);
            glVertex2f(hermiteTemp.x, -hermiteTemp.z);
        }
    }
    glEnd();
}
 
void yPontKirajzol(){
    for (int i = 0; i < elemszam; i++) {
        drawCircle(points[i].x, -points[i].z, 0.02f, 16);
    }
}
 
void yViewKirajzol(){
    pontColorSet();
    yPontKirajzol();
    
    vonalColorSet();
    yVonalKirajzol();
}
 
void zVonalKirajzol(){
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i<elemszam - 1; i++){
        for (float t = times[i]; t <= times[i + 1]; t += 1.0f / vonalResolution){
            hermiteTemp = hermiteInterpolate(points[i], rennerGaborSpline(i), points[i + 1], rennerGaborSpline(i+1), times[i], times[i + 1], t);
            glVertex2f(hermiteTemp.x, hermiteTemp.y);
        }
    }
    glEnd();
}
 
void zPontKirajzol(){
    for (int i = 0; i < elemszam; i++) {
        drawCircle(points[i].x, points[i].y, 0.02f, 16);
    }
}
 
void zViewKirajzol(){
    pontColorSet();
    zPontKirajzol();
    
    vonalColorSet();
    zVonalKirajzol();
}
 
Vector convertToHomogen(Vector vector){
    spaceWasPressed = false;
    Vector vectorTemp;
    vectorTemp.x = (vector.x) / (screenWidth/4) - 1;
    vectorTemp.y = (vector.y) / (screenHeight/4) - 1;
    vectorTemp.z = (vector.z) / (screenWidth/4) - 1;
    return vectorTemp;
}
 
Vector xViewPortbanVan(float xscreen, float yscreen){
    if (viewPortPos == 'n') {
        viewPortPos = 'x';
    }
    if (viewPortPos != 'x') {
        viewportChanged = true;
    }
    Vector vectorTemp;
    vectorTemp.x = screenWidth/4;
    vectorTemp.y = (screenWidth/2) - (yscreen - (screenWidth/2));
    vectorTemp.z = (screenWidth/2) - (xscreen - (screenWidth/2));
    
    return vectorTemp;
}
 
Vector yViewPortbanVan(float xscreen, float yscreen){
    if (viewPortPos == 'n') {
        viewPortPos = 'y';
    }
    if (viewPortPos != 'y') {
        viewportChanged = true;
    }
    Vector vectorTemp;
    vectorTemp.x = xscreen;
    vectorTemp.y = (screenHeight/4);
    vectorTemp.z = yscreen;
    
    return vectorTemp;
}
 
Vector zViewPortbanVan(float xscreen, float yscreen){
    if (viewPortPos == 'n') {
        viewPortPos = 'z';
    }
    if (viewPortPos != 'z') {
        viewportChanged = true;
    }
    Vector vectorTemp;
    vectorTemp.x = xscreen;
    vectorTemp.y = (screenHeight/2) - (yscreen - (screenHeight/2));
    vectorTemp.z = (screenWidth/4);
    if (viewPortPos != 'z') {
        viewportChanged = true;
    }
    return vectorTemp;
}
 
Vector convertToScreen(int x, int y){
    float xscreen = float(x);
    float yscreen = float (y);
    Vector temporaryVector;
    
    if (x <= (screenWidth / 2) && y <= (screenHeight / 2)) {
        temporaryVector = yViewPortbanVan(xscreen, yscreen);
    }
    else if (x <= (screenWidth / 2) && y > (screenHeight / 2)){
        temporaryVector = zViewPortbanVan(xscreen, yscreen);
    }
    else if (x >= (screenWidth / 2) && y >= (screenHeight / 2)){
        temporaryVector = xViewPortbanVan(xscreen, yscreen);
    }
    else return temporaryVector;
    
    return temporaryVector;
}
 
float tavolsag(float vizszintes, float fuggoleges, float vizszintesEger, float fuggolegesEger){
    return sqrtf((powf(vizszintes - vizszintesEger, 2.0f)) + (powf(fuggoleges - fuggolegesEger, 2.0f)));
}
 
void mozgatEgerhez(float vizszintesEger, float fuggolegesEger){
    Vector egerTemp;
    
    if (vizszintesEger <= (screenWidth / 2) && fuggolegesEger <= (screenHeight / 2)) {
        egerTemp = convertToHomogen(yViewPortbanVan(vizszintesEger, fuggolegesEger));
        int minIndex = 0;
        float minTavolsag = tavolsag(points[0].x, points[0].z, egerTemp.x, egerTemp.z);
        for (int i = 1; i < elemszam; i++) {
            if (tavolsag(points[i].x, points[i].z, egerTemp.x, egerTemp.z) < minTavolsag) {
                minTavolsag = tavolsag(points[i].x, points[i].z, egerTemp.x, egerTemp.z);
                minIndex = i;
            }
        }
        points[minIndex].x = egerTemp.x;
        points[minIndex].z = egerTemp.z;
    }
    else if (vizszintesEger <= (screenWidth / 2) && fuggolegesEger > (screenHeight / 2)){
        egerTemp = convertToHomogen(zViewPortbanVan(vizszintesEger, fuggolegesEger));
        int minIndex = 0;
        float minTavolsag = tavolsag(points[0].x, points[0].y, egerTemp.x, egerTemp.y);
        for (int i = 1; i < elemszam; i++) {
            if (tavolsag(points[i].x, points[i].y, egerTemp.x, egerTemp.y) < minTavolsag) {
                minTavolsag = tavolsag(points[i].x, points[i].y, egerTemp.x, egerTemp.y);
                minIndex = i;
            }
        }
        points[minIndex].x = egerTemp.x;
        points[minIndex].y = egerTemp.y;
    }
    else if (vizszintesEger >= (screenWidth / 2) && fuggolegesEger >= (screenHeight / 2)){
        egerTemp = convertToHomogen(xViewPortbanVan(vizszintesEger, fuggolegesEger));
        int minIndex = 0;
        float minTavolsag = tavolsag(points[0].z, points[0].y, egerTemp.z, egerTemp.y);
        for (int i = 1; i < elemszam; i++) {
            if (tavolsag(points[i].z, points[i].y, egerTemp.z, egerTemp.y) < minTavolsag) {
                minTavolsag = tavolsag(points[i].z, points[i].y, egerTemp.z, egerTemp.y);
                minIndex = i;
            }
        }
        points[minIndex].z = egerTemp.z;
        points[minIndex].y = egerTemp.y;
    }
}
 
Vector hovaHelyezzem(float ido){
    int where = 0;
    for (int i = 0; i < elemszam; i++) {
        if (ido > times[elemszam - 1]) {
            spaceWasPressed = false;
        }
        if (times[i] > ido) {
            where = i -1;
            break;
        }
    }
    float t = ido;
    
    return hermiteInterpolate(points[where], rennerGaborSpline(where), points[where + 1], rennerGaborSpline(where+1), times[where], times[where + 1], t);
}
 
Vector hovaDerivaljam(float ido){
    int where = 0;
    for (int i = 0; i < elemszam; i++) {
        if (ido > times[elemszam - 1]) {
            spaceWasPressed = false;
        }
        if (times[i] > ido) {
            where = i - 1;
            break;
        }
    }
    float t = ido;
    
    return hermiteDerivate(points[where], rennerGaborSpline(where), points[where + 1], rennerGaborSpline(where+1), times[where], times[where + 1], t);
}
 
void animateZ(float ido){
    glColor3f(1.0f, 0.0f, 0.0f);
    Vector hova;
    hova = hovaDerivaljam(ido);
    hova = hova*(1/hova.Length());
    hova = hova * (2.0f/3.0f);
    drawCircle(hovaHelyezzem(ido).x, hovaHelyezzem(ido).y, 0.05f, 16);
    
    glBegin(GL_LINES);
    glVertex2f(hovaHelyezzem(ido).x, hovaHelyezzem(ido).y);
    glVertex2f(hovaHelyezzem(ido).x - hova.x, hovaHelyezzem(ido).y-hova.y);
    glEnd();
}
 
void animateY(float ido){
    glColor3f(1.0f, 0.0f, 0.0f);
    Vector hova;
    hova = hovaDerivaljam(ido);
    hova = hova*(1/hova.Length());
    hova = hova * (2.0f/3.0f);
    drawCircle(hovaHelyezzem(ido).x, -hovaHelyezzem(ido).z, 0.05f, 16);
    
    glBegin(GL_LINES);
    glVertex2f(hovaHelyezzem(ido).x, -hovaHelyezzem(ido).z);
    glVertex2f(hovaHelyezzem(ido).x - hova.x, -1*(hovaHelyezzem(ido).z-hova.z));
    glEnd();
}
 
void animateX(float ido){
    glColor3f(1.0f, 0.0f, 0.0f);
    Vector hova;
    hova = hovaDerivaljam(ido);
    hova = hova*(1/hova.Length());
    hova = hova * (2.0f/3.0f);
    drawCircle(-hovaHelyezzem(ido).z, hovaHelyezzem(ido).y, 0.05f, 16);
    
    glBegin(GL_LINES);
    glVertex2f(-hovaHelyezzem(ido).z, hovaHelyezzem(ido).y);
    glVertex2f(-1*(hovaHelyezzem(ido).z - hova.z), hovaHelyezzem(ido).y-hova.y);
    glEnd();
}
 
void ablakKirajzol(){
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);        // torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles
    
    glViewport(0, 0, screenHeight, screenWidth);
    vonalColorSet();
    glBegin(GL_LINES);
    glVertex2f(-1, 0);
    glVertex2f(1, 0);
    
    glVertex2f(0, -1);
    glVertex2f(0, 1);
    glEnd();
    
    glViewport(0, 0, screenWidth/2, screenHeight/2);
    zViewKirajzol();
    
    glViewport(0, screenHeight/2, screenWidth/2, screenHeight/2);
    yViewKirajzol();
    
    glViewport(screenWidth/2, 0, screenWidth/2, screenHeight/2);
    xViewKirajzol();
}
 
 
// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
}
 
// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    ablakKirajzol();
    glViewport(0, 0, screenWidth/2, screenHeight/2);
    if (spaceWasPressed) {
        animateZ(time);
    }
    glViewport(0, screenHeight/2, screenWidth/2, screenHeight/2);
    if (spaceWasPressed) {
        animateY(time);
    }
    glViewport(screenWidth/2, 0, screenWidth/2, screenHeight/2);
    if (spaceWasPressed) {
        animateX(time);
    }
    
    glutSwapBuffers();                     // Buffercsere: rajzolas vege
    
}
 
// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
    if (key == ' '){
        time = 0;
        spaceWasPressed = true;
    }
}
 
// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {
    
}
 
// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        Vector tmp = convertToHomogen(convertToScreen(x, y));
        if (elemszam == 0) {
            points[elemszam++] = tmp;
        }else
            if ((x<(screenWidth/2) || y>(screenHeight/2)) && viewportChanged == false && !(tmp.x == points[elemszam-1].x && tmp.y == points[elemszam-1].y && tmp.z == points[elemszam-1].z))
                points[elemszam++] = tmp;
            else
                mozgatEgerhez(x, y);
    }// A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
    timeCalculator();
    glutPostRedisplay( );                          // Ilyenkor rajzold ujra a kepet
}
 
// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{
    
}
 
// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
    float deltaT = 0.01f;
    float timeDiff = (2.0f/3.0f);
    static long  old_time = glutGet(GLUT_ELAPSED_TIME);
    long new_time = glutGet(GLUT_ELAPSED_TIME);        // program inditasa ota eltelt ido
    if (spaceWasPressed && time < (times[elemszam - 1] - deltaT)) {
        float elapsedTimeInSec = (new_time - old_time) / 1000.0f;
        time += elapsedTimeInSec * timeDiff;
    }
    else{
        spaceWasPressed = false;
    }
    old_time = new_time;
    
    glutPostRedisplay();
    
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