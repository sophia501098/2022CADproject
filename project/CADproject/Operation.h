#pragma once
#include<iostream>
#include"Element.h"
using namespace std;
class Operation
{
private:
	vector<Vertex*> Vmm; //vertex memory manager;
	vector<Edge*> Emm;
	vector<HalfEdge*> HEmm;
	vector<Loop*> Lmm;
	vector<Face*> Fmm;
	vector<Solid*> Smm;
	vector<Point*> Pmm;
public:
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
			cout << "(" << vpos[0] << " " << vpos[1] << " " << vpos[2] << ") -> ";
			cout << "(" << evpos[0] << " " << evpos[1] << " " << evpos[2] << ")" << endl;
		}
		Vertex* v = he->startv;
		Vertex* ev = he->endv;
		glm::vec3 vpos = v->pointPtr->GetPos();
		glm::vec3 evpos = ev->pointPtr->GetPos();
		cout << "(" << vpos[0] << " " << vpos[1] << " " << vpos[2] << ") -> ";
		cout << "(" << evpos[0] << " " << evpos[1] << " " << evpos[2] << ")" << endl;
	}
	void PrintLoop_pre(Loop* lp)
	{
		cout << "print loop" << endl;
		HalfEdge* beginhe = lp->ledg;
		HalfEdge* he;
		for (he = beginhe; he->pre_he != beginhe; he = he->pre_he)
		{
			Vertex* v = he->startv;
			Vertex* ev = he->endv;
			glm::vec3 vpos = v->pointPtr->GetPos();
			glm::vec3 evpos = ev->pointPtr->GetPos();
			cout << "(" << vpos[0] << " " << vpos[1] << " " << vpos[2] << ") -> ";
			cout << "(" << evpos[0] << " " << evpos[1] << " " << evpos[2] << ")" << endl;
		}
		Vertex* v = he->startv;
		Vertex* ev = he->endv;
		glm::vec3 vpos = v->pointPtr->GetPos();
		glm::vec3 evpos = ev->pointPtr->GetPos();
		cout << "(" << vpos[0] << " " << vpos[1] << " " << vpos[2] << ") -> ";
		cout << "(" << evpos[0] << " " << evpos[1] << " " << evpos[2] << ")" << endl;
	}
	Solid* mvsf(Point* p, Vertex** v)
	{
		Face* f = new Face();
		Fmm.push_back(f);
		Solid* s = new Solid();
		Smm.push_back(s);
		*v = new Vertex(p);
		Vmm.push_back(*v);
		Loop* lp = new Loop();
		Lmm.push_back(lp);
		s->sface = f;
		f->fsolid = s;

		f->floop = lp;
		lp->lface = f;
		lp->isInnerLoop = false;
		lp->ledg = NULL;
		return s;
	}

	// v1是原来的点，v2是新生成的点，lp是所属的环
	HalfEdge* mev(Vertex** v1, Vertex** v2,Point* p2, Loop** lp)
	{
		//glm::vec3 v1pos = (*v1)->pointPtr->GetPos();
		//glm::vec3 v2pos = p2->GetPos();
		//cout << "make edge: (" << v1pos[0] << "," << v1pos[1] << "," << v1pos[2] << ") -> " << "(" << v2pos[0] << ", " << v2pos[1] << ", " << v2pos[2] << ")" << endl;
		HalfEdge* he1 = new HalfEdge();
		HalfEdge* he2 = new HalfEdge();
		HEmm.push_back(he1);
		HEmm.push_back(he2);
		Edge* eg = new Edge();
		Emm.push_back(eg);
		*v2 = new Vertex(p2);
		Vmm.push_back(*v2);

		he1->startv = *v1;
		he1->endv = *v2;
		he2->startv = *v2;
		he2->endv = *v1;

		he1->nxt_he = he2;
		he2->pre_he = he1;
		he1->wloop = he2->wloop = *lp;

		eg->he1 = he1;
		eg->he2 = he2;
		he1->e = eg;
		he2->e = eg;
		/*if (*lp)
		{
			Loop* newlp=new Loop();
			*lp = newlp;
		}*/
		if ((*lp) ->ledg == NULL)
		{
			he2->nxt_he = he1;
			he1->pre_he = he2;
			(*lp)->ledg = he1;
		}
		else
		{
			HalfEdge* he;
			for (he = (*lp)->ledg; he->nxt_he->startv != *v1; he = he->nxt_he)
			{
			}
			he2->nxt_he = he->nxt_he;
			he->nxt_he = he1;
			he1->pre_he = he;
			he2->nxt_he->pre_he = he2;
		}
		return he1;
	}

	//以v1，v2为端点生成新边和新面
	Loop* mef(Vertex** v1, Vertex** v2, Loop** rawlp)
	{
		Edge* e = new Edge();
		Emm.push_back(e);
		HalfEdge* he1 = new HalfEdge();
		HalfEdge* he2 = new HalfEdge();
		HEmm.push_back(he1);
		HEmm.push_back(he2);
		e->he1 = he1;
		e->he2 = he2;
		he1->e = e;
		he2->e = e;
		he1->startv = *v1;
		he1->endv = *v2;
		he2->startv = *v2;
		he2->endv = *v1;

		//新建一个面，以及新建这个面所对应的外环
		Face* newf = new Face();
		Loop* newlp = new Loop();
		Fmm.push_back(newf);
		Lmm.push_back(newlp);
		Solid* raws = (*rawlp) ->lface->fsolid;
		Face* f = raws->sface;
		while (f->nxt_f != NULL)
		{
			f = f->nxt_f;
		}
		f->nxt_f = newf;
		newf->pre_f = f;
		newf->fsolid = raws;
		newf->nxt_f = NULL;
		newf->floop = newlp;

		newlp->isInnerLoop = false;
		newlp->lface = newf;
		

		HalfEdge* he_b = (*rawlp)->ledg;
		while (he_b->endv != *v2)
		{
			he_b = he_b->nxt_he;
		}
		assert(he_b->endv == *v2);

		HalfEdge* he_a = (*rawlp)->ledg;
		while (he_a->endv != *v1)
		{
			he_a = he_a->nxt_he;
		}
		assert(he_a->endv == *v1);

		he1->nxt_he = he_b->nxt_he;
		he1->nxt_he->pre_he = he1;
		he_b->nxt_he = he2;
		he2->pre_he = he_b;

		he2->nxt_he = he_a->nxt_he;
		he2->nxt_he->pre_he = he2;
		he_a->nxt_he = he1;
		he1->pre_he = he_a;

		newlp->ledg = he1;
		(*rawlp)->ledg = he2;
		he1->wloop = newlp;
		he2->wloop = *rawlp;
		
		if ((*rawlp)->GetLength() < newlp->GetLength())
		{
			newlp->ledg = he2;
			(*rawlp)->ledg = he1;
			he1->wloop = *rawlp;
			he2->wloop = newlp;
		}
		
		return newlp;
	}

	//v1在外环上，v2在内环上
	Loop* kemr(Vertex** v1, Vertex** v2, Loop** rawlp)
	{
		Loop* newlp = new Loop();
		Lmm.push_back(newlp);
		newlp->isInnerLoop = true;
		Face* rawf = (*rawlp) ->lface;
		newlp->lface = rawf;
		rawf->hasInnerLoop = true;
		Loop* curlp = rawf->floop;
		while (curlp->nxt_l)
		{
			curlp = curlp->nxt_l;
		}
		curlp->nxt_l = newlp;
		newlp->pre_l = curlp;
		HalfEdge* he1 = (*rawlp)->ledg;
		
		//he1: v1 -> v2
		while (!(he1->startv == *v1 && he1->endv == *v2))
		{
			he1 = he1->nxt_he;
		}
		assert(he1->startv == *v1 && he1->endv == *v2);

		//he2: v2 -> v1
		HalfEdge* he2 = (*rawlp)->ledg;
		while (!(he2->startv == *v2 && he2->endv == *v1))
		{
			he2 = he2->nxt_he;
		}
		assert(he2->startv == *v2 && he2->endv == *v1);


		newlp->ledg = he1->nxt_he;
		(*rawlp)->ledg = he2->nxt_he;
		//PrintLoop_pre(newlp);
		//PrintLoop(newlp);
		//HalfEdge* testhe = he1->nxt_he->nxt_he->nxt_he->nxt_he; //->nxt_he=he2

		HalfEdge* he1_pre = he1->pre_he;
		HalfEdge* he2_pre = he2->pre_he;
		HalfEdge* he1_nxt = he1->nxt_he;
		HalfEdge* he2_nxt = he2->nxt_he;

		he1_pre->nxt_he = he2_nxt;
		he2_pre->nxt_he = he1_nxt;
		he1_nxt->pre_he = he2_pre;
		he2_nxt->pre_he = he1_pre;

		/*he2->pre_he->nxt_he = he1->nxt_he;
		he1->nxt_he->pre_he = he2->pre_he;
		he1->pre_he->nxt_he = he2->nxt_he;
		he2->nxt_he->pre_he = he1->pre_he;*/
		
		//PrintLoop(newlp);

		if ((*v1) ->vhe == he1 || (*v1)->vhe == he2)
		{
			(*v1)->vhe = he1->pre_he;
		}
		if ((*v2) ->vhe == he1 || (*v2)->vhe == he2)
		{
			(*v2)->vhe = he2->pre_he;
		}
		he1->e = NULL;
		he1 = NULL;
		he2 = NULL;
		return newlp;
	}
	
	//删除rawf2,将其变成rawf1的一个内环
	void kfmrh(Face** rawf1,Face** rawf2)
	{
		Loop* inlp = (*rawf2) ->floop;
		inlp->isInnerLoop = true;
		Loop* lp = (*rawf1)->floop;
		while (lp->nxt_l)
		{
			lp = lp->nxt_l;
		}
		lp->nxt_l = inlp;
		inlp->pre_l = lp;
		inlp->lface = *rawf1;
		(*rawf1)->hasInnerLoop = true;
		Face* pref = (*rawf2)->pre_f;
		Face* nxtf = (*rawf2)->nxt_f;
		if ((*rawf2)->fsolid->sface == (*rawf2))
			(*rawf2)->fsolid->sface = *rawf1;
		/*if ((*rawf2)->pre_f)
			(*rawf2)->pre_f->nxt_f = (*rawf2)->nxt_f;
		if ((*rawf2)->nxt_f)
			(*rawf2)->nxt_f->pre_f = (*rawf2)->pre_f;*/
		if (pref)
		{
			pref->nxt_f = nxtf;
		}
		if (nxtf)
		{
			nxtf->pre_f = pref;
		}
		*rawf2 = NULL;
	}

	Solid* sweep(Face* rawf, glm::vec3 direction, float distance)
	{
		cout << "sweepping...." << endl;
		Solid* solid = rawf->fsolid;
		Loop* lp = rawf->floop;
		HalfEdge* he;
		while (lp)
		{
			Loop* nxlp = lp->nxt_l;
			cout << "sweepping loop...." << endl;
			he = lp->ledg;
			Vertex* startv,*v;
			startv = he->startv;
			glm::vec3 rawVpos = startv->pointPtr->GetPos();
			glm::vec3 newVpos = rawVpos + distance * direction;
			Point* newp = new Point(newVpos[0], newVpos[1], newVpos[2]);
			newp->SetColor(0.0f, 1.0f, 0.0f);
			Pmm.push_back(newp);
			Vertex* newv;
			mev(&startv, &newv, newp, &lp);
			//PrintLoop(lp);
			Vertex* prev = newv;
			Vertex* firstupv = newv;
			he = he->nxt_he;
			v = he->startv;
			while (v != startv)
			{
				rawVpos = v->pointPtr->GetPos();
				newVpos = rawVpos + distance * direction;
				newp = new Point(newVpos[0], newVpos[1], newVpos[2]);
				newp->SetColor(0.0f, 1.0f, 0.0f);
				Pmm.push_back(newp);
				Vertex* newv;
				
				mev(&v, &newv, newp, &lp);
				
				mef(&prev, &newv, &lp);
				prev = newv;

				he = he->nxt_he;
				v = he->startv;
			}
			mef(&prev, &firstupv, &lp);
			lp = lp->nxt_l;
		}
		
		return solid;
	}

	void FreeAll()
	{
		for (int i = 0; i < Vmm.size(); ++i)
		{
			delete(Vmm[i]);
		}
		for (int i = 0; i < Emm.size(); ++i)
		{
			delete(Emm[i]);
		}
		for (int i = 0; i < HEmm.size(); ++i)
		{
			delete(HEmm[i]);
		}
		for (int i = 0; i < Lmm.size(); ++i)
		{
			delete(Lmm[i]);
		}
		for (int i = 0; i < Fmm.size(); ++i)
		{
			delete(Fmm[i]);
		}
		for (int i = 0; i < Smm.size(); ++i)
		{
			delete(Smm[i]);
		}
	}
};
