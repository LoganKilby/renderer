/* date = October 18th 2021 3:26 pm */

#ifndef _VISION_DEFECT_STRUCT_H
#define _VISION_DEFECT_STRUCT_H
#pragma once

// slm: 06-23-17
#define MAX_NUM_VISION_DEFECTS		500
#define VISION_NUM_DEFECT_TYPES		(7)

typedef enum VisionDefectTypeTag
{
	VISION_DEFECT_NO_DEFECT = 0,
	VISION_DEFECT_KNOT,
	VISION_DEFECT_PITCH_POCKET,
	VISION_DEFECT_STAIN,
	VISION_DEFECT_OTHER,
	VISION_DEFECT_WANE,
	VISION_DEFECT_IGNORED //Update VISION_NUM_DEFECT_TYPES above if any new types are added
} VisionDefectType;

typedef struct DefectRectangleTag
{
	int left;
	int top;
	int right;
	int bottom;
} DefectRectangle;

typedef struct VisionDefectTag
{
	int id;
	int type;
	DefectRectangle position;	/* pixel position */
	double area;				/* square inches */
} VISION_DEFECT;

typedef struct {
	int iNumDefects;
	VISION_DEFECT sDefects[MAX_NUM_VISION_DEFECTS];
} VISION_DEFECT_STRUCT;

typedef struct {
	VISION_DEFECT_STRUCT	sTopDefectData;
	VISION_DEFECT_STRUCT	sBottomDefectData;
} VISION_DEFECT_DATA;

/* NOTE : all rectanges in the following code are encoded into a 4-element list,
which gives the x or y coordinates of the edges in the following order:
left, top, right, bottom, where left and right are given in x coords; and top,
bottom in y coords.
Note also that in Baxley terminology the top is the lesser y coordinate,
and the bottom is the greater y. */

/* enumeration of rectangle edge indices */
enum RectangleEdgeType { XLEFT = 1, YTOP, XRIGHT, YBOTTOM };
enum CoordinateAxesType { XNEG = 1, YNEG, XPOS, YPOS };

class Rect {
    public:
	int left;
	int top;
	int right;
	int bottom;
	int mask;  // bitmask for defect screening. unused for other rect types.
	Rect() :left(0), top(0), right(0), bottom(0), mask(0) {};
	Rect(const Rect& r) :left(r.left), top(r.top), right(r.right), bottom(r.bottom), mask(r.mask) {};
	Rect(int left, int top, int right, int bottom) :left(left), top(top), right(right), bottom(bottom), mask(0) {};
	Rect& operator=(const Rect& r) {
		left = r.left;
		right = r.right;
		bottom = r.bottom;
		top = r.top;
		mask = r.mask;
		return *this;
	};
    
	int area() {
		return ((right - left)*(bottom - top));
	}
	int coordinate(CoordinateAxesType direction) {
		switch (direction) {
            case XNEG: return left;
            case YNEG: return top;
            case XPOS: return right;
            case YPOS: return bottom;
            default: return -10000; // throw exception or make assertion
		}
	}
	int coordinate(RectangleEdgeType edgeType) {
		switch (edgeType) {
            case XLEFT: return left;
            case YTOP: return top;
            case XRIGHT: return right;
            case YBOTTOM: return bottom;
            default: return -10000; // throw exception or make assertion
		}
	}
};


#endif //_VISION_DEFECT_STRUCT_H
