#include "ofxTriangle.h"

#ifdef USE_OPENCV
void ofxTriangle::triangulate(ofxCvBlob &cvblob, int resolution)
{		
	triangulate(cvblob.pts, resolution);
}
#endif

void ofxTriangle::clear(){
    triangles.clear();
    nTriangles = 0;
}

ofPoint ofxTriangle::getTriangleCenter(ofPoint *tr){
    float c_x = (tr[0].x + tr[1].x + tr[2].x) / 3;
    float c_y = (tr[0].y + tr[1].y + tr[2].y) / 3;

    return ofPoint(c_x, c_y);
}

bool ofxTriangle::isPointInsidePolygon(const ofPoint *polygon,int N, ofPoint p)
{
    int counter = 0;
    int i;
    double xinters;
    ofPoint p1,p2;

    p1 = polygon[0];

    for (i=1;i<=N;i++)
    {
        p2 = polygon[i % N];
        if (p.y > MIN(p1.y,p2.y)) {
            if (p.y <= MAX(p1.y,p2.y)) {
                if (p.x <= MAX(p1.x,p2.x)) {
                    if (p1.y != p2.y) {
                        xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
                        if (p1.x == p2.x || p.x <= xinters){
                            counter++;
						}
                    }
                }
            }
        }
        p1 = p2;
    }

	return counter % 2 != 0;
}

void ofxTriangle::draw(float x, float y) {
    ofPushMatrix();
    ofTranslate(x, y, 0);
	draw();
    ofPopMatrix();
}

void ofxTriangle::draw() {
	draw(ofRandom(0, 255),ofRandom(0, 255),ofRandom(0, 255));
}

void ofxTriangle::draw(float r, float g, float b) {
    ofFill();
	
    for (int i=0; i<nTriangles; i++){
        ofSetColor(r,g,b);
        ofTriangle( triangles[i].a.x, triangles[i].a.y,
				   triangles[i].b.x, triangles[i].b.y,
				   triangles[i].c.x, triangles[i].c.y);
    }
}
