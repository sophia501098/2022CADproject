#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/GL/glut.h"
#include "Element.h"
#include "Draw.h"
#include "Operation.h"
using namespace std;
using namespace glm;

#define PI 3.14159265358979323846

const glm::ivec2 SCREEN_SIZE(1024, 1024);
const float FAR_PLANE = 100.0f;
const float NEAR_PLANE = 1.0f;
static float yAngle, zAngle;
static unsigned int Count;
static bool keys[1024];
glm::mat4 view;
glm::vec3 viewPosition;
glm::vec3 viewDirection;
std::vector<Point*> Pmm;
static void Movement(float deltaTime)
{
    deltaTime *= 50;
    // Camera controls
    if (keys[GLFW_KEY_W])
        yAngle += 1.0f * deltaTime;
    if (keys[GLFW_KEY_S])
        yAngle -= 1.0f * deltaTime;
    if (keys[GLFW_KEY_A])
        zAngle += 1.0f * deltaTime;
    if (keys[GLFW_KEY_D])
        zAngle -= 1.0f * deltaTime;
}
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        Count++;
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void PrintLoop(Loop* lp)
{
    cout << "print loop" << endl;
    HalfEdge* beginhe = lp->ledg;
    HalfEdge* he;
    for (he = beginhe; he->nxt_he != beginhe; he = he->nxt_he)
    {
        Vertex* v = he->startv;
        Vertex* ev = he->endv;
        glm::vec3 vpos = v->pointPtr->GetPos();
        glm::vec3 evpos = ev->pointPtr->GetPos();
        cout <<"("<< vpos[0] << " " << vpos[1] << " " << vpos[2]<<") -> ";
        cout << "(" << evpos[0] << " " << evpos[1] << " " << evpos[2] << ")"<<endl;
    }
    Vertex* v = he->startv;
    Vertex* ev = he->endv;
    glm::vec3 vpos = v->pointPtr->GetPos();
    glm::vec3 evpos = ev->pointPtr->GetPos();
    cout << "(" << vpos[0] << " " << vpos[1] << " " << vpos[2] << ") -> ";
    cout << "(" << evpos[0] << " " << evpos[1] << " " << evpos[2] << ")" << endl;
}
Solid* BuildSolid(Operation& m_op)
{
    //外环
    Point* p1 = new Point(-1.0f, 0.5f, 2.0f);
    Point* p2 = new Point(1.0, 0.5, 2.0);
    Point* p11 = new Point(2.0, 0.5, 0);
    Point* p3 = new Point(1.0, 0.5, -2.0);
    Point* p4 = new Point(-1.0, 0.5, -2.0);
    Point* p9 = new Point(-2.0, 0.5, 0);

    //内环1
    Point* p5 = new Point(-0.8f, 0.5f, 1.3f);
    Point* p6 = new Point(-0.2, 0.5, 1.3);
    Point* p7 = new Point(-0.2, 0.5, -1.3);
    Point* p8 = new Point(-0.8, 0.5, -1.3);
    Point* p10 = new Point(-1.5, 0.5, 0);

    //内环2
    Point* p12 = new Point(0.8f, 0.5f, 1.3f);
    Point* p13 = new Point(0.2, 0.5, 1.3);
    Point* p14 = new Point(0.2, 0.5, -1.3);
    Point* p15 = new Point(0.8, 0.5, -1.3);
    Point* p16 = new Point(1.5, 0.5, 0);

    Pmm.push_back(p1);
    Pmm.push_back(p2);
    Pmm.push_back(p3);
    Pmm.push_back(p4);
    Pmm.push_back(p5);
    Pmm.push_back(p6);
    Pmm.push_back(p7);
    Pmm.push_back(p8);
    Pmm.push_back(p9);
    Pmm.push_back(p10);
    Pmm.push_back(p11);
    Pmm.push_back(p12);
    Pmm.push_back(p13);
    Pmm.push_back(p14);
    Pmm.push_back(p15);
    Pmm.push_back(p16);

    p1->SetColor(1.0f, 0.0f, 0.0f);
    p2->SetColor(1.0f, 0.0f, 0.0f);
    p11->SetColor(1.0f, 0.0f, 0.0f);
    p3->SetColor(1.0f, 0.0f, 0.0f);
    p4->SetColor(1.0f, 0.0f, 0.0f);
    p9->SetColor(1.0f, 0.0f, 0.0f);

    p5->SetColor(1.0f, 0.0f, 0.0f);
    p6->SetColor(1.0f, 0.0f, 0.0f);
    p7->SetColor(1.0f, 0.0f, 0.0f);
    p8->SetColor(1.0f, 0.0f, 0.0f);
    p10->SetColor(1.0f, 0.0f, 0.0f);

    p12->SetColor(1.0f, 0.0f, 0.0f);
    p13->SetColor(1.0f, 0.0f, 0.0f);
    p14->SetColor(1.0f, 0.0f, 0.0f);
    p15->SetColor(1.0f, 0.0f, 0.0f);
    p16->SetColor(1.0f, 0.0f, 0.0f);

    Vertex* v1 = NULL;
    Vertex* v2 = NULL;
    Vertex* v3 = NULL;
    Vertex* v4 = NULL;
    Vertex* v5 = NULL;
    Vertex* v6 = NULL;
    Vertex* v7 = NULL;
    Vertex* v8 = NULL;
    Vertex* v9 = NULL;
    Vertex* v10 = NULL;
    Vertex* v11 = NULL;
    Vertex* v12 = NULL;
    Vertex* v13 = NULL;
    Vertex* v14 = NULL;
    Vertex* v15 = NULL;
    Vertex* v16 = NULL;

    //out loop
    Solid* solid = m_op.mvsf(p1, &v1);
    m_op.mev(&v1, &v2, p2, &(solid->sface->floop));
    m_op.mev(&v2, &v11, p11, &(solid->sface->floop));
    m_op.mev(&v11, &v3, p3, &(solid->sface->floop));
    m_op.mev(&v3, &v4, p4, &(solid->sface->floop));
    m_op.mev(&v4, &v9, p9, &(solid->sface->floop));
    Loop* l = m_op.mef(&v9, &v1, &(solid->sface->floop));

    //inner loop 1
    m_op.mev(&v1, &v5, p5, &(solid->sface->floop));
    m_op.mev(&v5, &v6, p6, &(solid->sface->floop));
    m_op.mev(&v6, &v7, p7, &(solid->sface->floop));
    m_op.mev(&v7, &v8, p8, &(solid->sface->floop));
    m_op.mev(&v8, &v10, p10, &(solid->sface->floop));
    m_op.mef(&v10, &v5, &(solid->sface->floop));
    m_op.kemr(&v1, &v5, &(solid->sface->floop));
    m_op.kfmrh(&solid->sface->nxt_f, &solid->sface->nxt_f->nxt_f);

    //inner loop 2
    m_op.mev(&v2, &v12, p12, &(solid->sface->floop));
    m_op.mev(&v12, &v13, p13, &(solid->sface->floop));
    m_op.mev(&v13, &v14, p14, &(solid->sface->floop));
    m_op.mev(&v14, &v15, p15, &(solid->sface->floop));
    m_op.mev(&v15, &v16, p16, &(solid->sface->floop));
    m_op.mef(&v16, &v12, &(solid->sface->floop));
    m_op.kemr(&v2, &v12, &(solid->sface->floop));
    m_op.kfmrh(&solid->sface->nxt_f, &solid->sface->nxt_f->nxt_f);

    //sweep
    solid = m_op.sweep(solid->sface, glm::vec3(0, -1, 0), 1);
    return solid;
}
int main()
{
    // Init GLFW
    if (!glfwInit())
    {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Create window
    GLFWwindow* window = glfwCreateWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Euler Operation", nullptr, nullptr);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    // Print out some info about the graphics drivers
    std::cout << "--------------------------------------" << endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable settings
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Init FPS Var
    std::cout << "--------------------------------------" << endl;
    std::cout << "Initialize Done." << endl;


    GLfloat deltaTime = 0, currentTime = 0;
    GLfloat startTime = glfwGetTime();
    GLfloat lastTime = glfwGetTime();

    Draw m_draw;
    Operation m_op;
    Solid* solid = BuildSolid(m_op);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        GLfloat newTime = glfwGetTime();
        Movement(newTime - lastTime);
        lastTime = newTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
        glViewport(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y);

        // Perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(45.0f, SCREEN_SIZE.x / SCREEN_SIZE.y, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); // Reset the model-view matrix
        glTranslatef(0.0f, 0.0f, -10.0f);

        glRotatef(zAngle, 0.0f, 1.0f, 0.0f);
        glRotatef(yAngle, 1.0f, 0.0f, 1.0f);

        glRotatef(180.0, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0, 1.0f, 0.0f, 0.0f);

        m_draw.DrawSolid(solid);
        
        glfwSwapBuffers(window);
    }
    m_op.FreeAll();
    for (int i = 0; i < Pmm.size(); ++i)
    {
        delete Pmm[i];
    }
    glfwTerminate();
    return 0;
}