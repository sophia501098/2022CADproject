#pragma once
#include<iostream>
#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/GL/glut.h"
#include"Element.h"
using namespace std;
using std::stringstream;
stringstream ss;
const char* getPrimitiveType(GLenum type)
{
    switch (type)
    {
    case 0x0000:
        return "GL_POINTS";
        break;
    case 0x0001:
        return "GL_LINES";
        break;
    case 0x0002:
        return "GL_LINE_LOOP";
        break;
    case 0x0003:
        return "GL_LINE_STRIP";
        break;
    case 0x0004:
        return "GL_TRIANGLES";
        break;
    case 0x0005:
        return "GL_TRIANGLE_STRIP";
        break;
    case 0x0006:
        return "GL_TRIANGLE_FAN";
        break;
    case 0x0007:
        return "GL_QUADS";
        break;
    case 0x0008:
        return "GL_QUAD_STRIP";
        break;
    case 0x0009:
        return "GL_POLYGON";
        break;
    }
}
void CALLBACK tessBeginCB(GLenum which)
{
    glBegin(which);

    // DEBUG //
    ss << "glBegin(" << getPrimitiveType(which) << ");\n";
}
void CALLBACK tessEndCB()
{
    glEnd();

    // DEBUG //
    ss << "glEnd();\n";
}
void CALLBACK tessVertexCB(const GLvoid* data)
{
    // cast back to double type
    const GLdouble* ptr = (const GLdouble*)data;

    glVertex3dv(ptr);

    // DEBUG //
    ss << "  glVertex3d(" << *ptr << ", " << *(ptr + 1) << ", " << *(ptr + 2) << ");\n";
}
void CALLBACK tessErrorCB(GLenum errorCode)
{
    const GLubyte* errorStr;

    errorStr = gluErrorString(errorCode);
    cerr << "[ERROR]: " << errorStr << endl;
}


class Draw {
public:
	void DrawFace(Face* face) {
        //cout << "///////////// Draw Face //////////////" << endl;
		Loop* curlp = face->floop;
		if (curlp != NULL && curlp->isInnerLoop == false)
		{
			
			glBegin(GL_POLYGON);
			HalfEdge* beginhe = curlp->ledg;
			HalfEdge* he;
			for (he = beginhe; he->nxt_he != beginhe; he = he->nxt_he)
			{
				Vertex* v = he->startv;
				glm::vec3 vpos = v->pointPtr->GetPos();
				glm::vec3 vcolor = v->pointPtr->GetColor();
				glColor3f(vcolor[0], vcolor[1], vcolor[2]);
				glVertex3f(vpos[0], vpos[1], vpos[2]);
				//cout << vpos[0] << " " << vpos[1] << " " << vpos[2] << endl;
			}
			Vertex* v = he->startv;
			glm::vec3 vpos = v->pointPtr->GetPos();
			glm::vec3 vcolor = v->pointPtr->GetColor();
			glColor3f(vcolor[0], vcolor[1], vcolor[2]);
			glVertex3f(vpos[0], vpos[1], vpos[2]);
			//cout << vpos[0] << " " << vpos[1] << " " << vpos[2] << endl;
			glEnd();
		}
		
	}
	void DrawSolid(Solid* solid) {
        //cout << "##################### Draw Solid ######################" << endl;
		Face* curFace = solid->sface;
		while (curFace) {
            if (curFace->hasInnerLoop) DrawFaceWithHole(curFace);
            else DrawFace(curFace);
			curFace = curFace->nxt_f;
		}
	}
    GLuint DrawFaceWithHole(Face* face)
    {
        //cout << "///////////// Draw Face with Hole //////////////" << endl;
        Loop* curlp = face->floop;
        GLuint id = glGenLists(1);  // create a display list
        if (!id) return id;          // failed to create a list, return 0

        GLUtesselator* tess = gluNewTess(); // create a tessellator
        if (!tess) return 0;         // failed to create tessellation object, return 0
        
        gluTessCallback(tess, GLU_TESS_BEGIN, (void(__stdcall*)(void))tessBeginCB);
        gluTessCallback(tess, GLU_TESS_END, (void(__stdcall*)(void))tessEndCB);
        gluTessCallback(tess, GLU_TESS_ERROR, (void(__stdcall*)(void))tessErrorCB);
        gluTessCallback(tess, GLU_TESS_VERTEX, (void(__stdcall*)())tessVertexCB);

        // tessellate and compile a concave quad into display list
        glNewList(id, GL_COMPILE);
        glColor3f(1, 1, 1);
        gluTessBeginPolygon(tess, 0);                       // with NULL data
        
        GLdouble* nvp;
        vector<GLdouble*> nvps;
        while (curlp)
        {
            //cout << "Begin loop" << endl;
            gluTessBeginContour(tess);
            HalfEdge* beginhe = curlp->ledg;
            HalfEdge* he;
            for (he = beginhe; he->nxt_he != beginhe; he = he->nxt_he)
            {
                Vertex* v = he->startv;
                glm::vec3 vpos = v->pointPtr->GetPos();
                glm::vec3 vcolor = v->pointPtr->GetColor();
                nvp = new GLdouble[3];
                nvp[0] = vpos[0];
                nvp[1] = vpos[1];
                nvp[2] = vpos[2];
                glColor3f(vcolor[0], vcolor[1], vcolor[2]);
                gluTessVertex(tess, nvp, nvp);
                //cout << nvp[0] << " " << nvp[1] << " " << nvp[2] << endl;
                nvps.push_back(nvp);
            }
            Vertex* v = he->startv;
            glm::vec3 vpos = v->pointPtr->GetPos();
            glm::vec3 vcolor = v->pointPtr->GetColor();
            nvp = new GLdouble[3];
            nvps.push_back(nvp);
            nvp[0] = vpos[0];
            nvp[1] = vpos[1];
            nvp[2] = vpos[2];
            glColor3f(vcolor[0], vcolor[1], vcolor[2]);
            gluTessVertex(tess, nvp, nvp);
            //cout << nvp[0] << " " << nvp[1] << " " << nvp[2] << endl;
            //cout << "end loop" << endl;
            gluTessEndContour(tess);
            curlp = curlp->nxt_l;
            
        }

        gluTessEndPolygon(tess);
        glEndList();

        gluDeleteTess(tess);        // delete after tessellation
        glCallList(id);

        //free memory
        for (int i = 0; i < nvps.size(); ++i)
        {
            delete[] nvps[i];
        }
        return id;      // return handle ID of a display list
        
    }
};