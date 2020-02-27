/*!
* ofxTriangle by kikko.fr
* -> C++ Triangle warper by Piyush Kumar
* -> Point inside polygon by Theo
*/

#ifndef OFXTRIANGLE_H_INCLUDED
#define OFXTRIANGLE_H_INCLUDED

//JG added this #define to allow to be decoupled from OpenCV
//along with the ability to pass in any vector of points
//allows for use with contours from OpenTSPS
//#define USE_OPENCV

#include "ofMain.h"
#ifdef USE_OPENCV
#include "ofxOpenCv.h"
#endif

#include <del_interface.hpp>

using namespace tpp;
//using namespace std;

typedef struct
{
    ofPoint a;
    ofPoint b;
    ofPoint c;

    float area;

} ofxTriangleData;

class ofxTriangle {
    public :

	ofxTriangle(){}
	~ofxTriangle(){}
#ifdef USE_OPENCV
	// Triangulate a openCV blob
	void triangulate(ofxCvBlob &cvblob, int resolution = 50);
#endif
	//POINT - expected to have .x,.y, so it can be ofPoint, ofVec2f, glm::vec2, glm::vec3
	template<class POINT>
	void triangulate(const vector<POINT> &contour, int resolution = 50) {
		int bSize = contour.size();
		float maxi = min(resolution, bSize);

		Delaunay::Point tempP;
		vector<Delaunay::Point> v;

		for (int i = 0; i < maxi; i++) {
			//int id = (int) ( (float)i/maxi*bSize );
			int indx = ofMap(i, 0, maxi, 0, bSize);
			tempP[0] = contour[indx].x;
			tempP[1] = contour[indx].y;

			v.push_back(tempP);
		}

		delobject = new Delaunay(v);
		delobject->Triangulate();

		Delaunay::fIterator fit;
		for (fit = delobject->fbegin(); fit != delobject->fend(); ++fit) {
			int pta = delobject->Org(fit);
			int ptb = delobject->Dest(fit);
			int ptc = delobject->Apex(fit);

			int pta_id = (int)ofMap(pta, 0, maxi, 0, bSize);
			int ptb_id = (int)ofMap(ptb, 0, maxi, 0, bSize);
			int ptc_id = (int)ofMap(ptc, 0, maxi, 0, bSize);

			ofPoint tr[3];
			tr[0] = ofPoint(contour[pta_id].x, contour[pta_id].y);
			tr[1] = ofPoint(contour[ptb_id].x, contour[ptb_id].y);
			tr[2] = ofPoint(contour[ptc_id].x, contour[ptc_id].y);

			if (isPointInsidePolygon(&contour[0], contour.size(), getTriangleCenter(tr))) {
				ofxTriangleData td;
				td.a = ofPoint(tr[0].x, tr[0].y);
				td.b = ofPoint(tr[1].x, tr[1].y);
				td.c = ofPoint(tr[2].x, tr[2].y);

				td.area = delobject->area(fit);

				triangles.push_back(td);

				nTriangles++;
			}
		}

		delete delobject;
	}

	//POINT - expected to have .x,.y, so it can be ofPoint, ofVec2f, glm::vec2, glm::vec3
	template<class POINT>
	void triangulate_indices(const vector<POINT> &contour) {
		int n = contour.size();

		vector<Delaunay::Point> v(n);

		for (int i = 0; i < n; i++) {
			v[i][0] = contour[i].x;
			v[i][1] = contour[i].y;
		}

		delobject = new Delaunay(v);
		delobject->Triangulate();

		tri_indices.clear();

		Delaunay::fIterator fit;
		for (fit = delobject->fbegin(); fit != delobject->fend(); ++fit) {
			int pta = delobject->Org(fit);
			int ptb = delobject->Dest(fit);
			int ptc = delobject->Apex(fit);
			tri_indices.push_back(pta);
			tri_indices.push_back(ptb);
			tri_indices.push_back(ptc);
		}

		delete delobject;
	}


	ofPoint getTriangleCenter(ofPoint *tr);
	bool isPointInsidePolygon(const ofPoint *polygon,int N, ofPoint p);

	void draw();
	void draw(float r, float g, float b);

	void draw(float x, float y);
	void clear();


	Delaunay* delobject;

	int nTriangles;
	vector <ofxTriangleData> triangles;

	vector<int> tri_indices;	//a,b,c, a,b,c,  a,b,c... - indices

};

#endif // OFXTRIANGLE_H_INCLUDED
