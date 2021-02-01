#pragma once

class Face {
public:
	int objidx;
	int vtx1, vtx2, vtx3;
	Face(int vtx1, int vtx2, int vtx3, int objidx) : vtx1(vtx1), vtx2(vtx2), vtx3(vtx3), objidx(objidx) {}
};
