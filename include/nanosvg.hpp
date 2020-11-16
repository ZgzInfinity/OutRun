/* ================================================ *
 * nanosvg++                                        *
 * This software is a fork of nanosvg (note below). *
 * No license-related aspects are affected.         *
 * Kamil Koczurek | koczurekk@gmail.com             *
 * ================================================ */
/* ================================================================================= *
 * Copyright (c) 2013-14 Mikko Mononen memon@inside.org                              *
 *                                                                                   *
 * This software is provided 'as-is', without any express or implied                 *
 * warranty.  In no event will the authors be held liable for any damages            *
 * arising from the use of this software.                                            *
 *                                                                                   *
 * Permission is granted to anyone to use this software for any purpose,             *
 * including commercial applications, and to alter it and redistribute it            *
 * freely, subject to the following restrictions:                                    *
 *                                                                                   *
 * 1. The origin of this software must not be misrepresented; you must not           *
 * claim that you wrote the original software. If you use this software              *
 * in a product, an acknowledgment in the product documentation would be             *
 * appreciated but is not required.                                                  *
 * 2. Altered source versions must be plainly marked as such, and must not be        *
 * misrepresented as being the original software.                                    *
 * 3. This notice may not be removed or altered from any source distribution.        *
 *                                                                                   *
 * The SVG parser is based on Anti-Grain Geometry 2.4 SVG example                    *
 * Copyright (C) 2002-2004 Maxim Shemanarev (McSeem) (http://www.antigrain.com/)     *
 *                                                                                   *
 * Arc calculation code based on canvg (https://code.google.com/p/canvg/)            *
 *                                                                                   *
 * Bounding box calculation based on                                                 *
 * http://blog.hackers-cafe.net/2009/06/how-to-calculate-bezier-curves-bounding.html *
 * ================================================================================= */

//! @file
//!
//! \brief
//! Nanosvg++ declarations

#ifndef NANOSVG_H
#define NANOSVG_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "enums.hpp"

//! \namespace nsvg
//!
//! \brief Nanosvg++ namespace
namespace nsvg {

//! \namespace nsvg::cstyle
//!
//! \brief Completly or partially C-styled code
//!
//! All C-styled code is stored in another namespace to avoid using it non-necessarily.
//! That code will be slowly replaced with object orientated one to provide fully-functional, C++-styled SVG library.
namespace cstyle {
	/*
	 *	Definitions
	*/
	#define MAX_ATTR 128
	#define NSVG_MAX_DASHES 8

	/*
	 *	Structures
	*/
	struct GradientStop {
		unsigned int color;
		float offset;
	};

	struct Gradient {
		float xform[6];
		SpreadType spread;
		float fx, fy;
		int nstops;
		GradientStop stops[1];
	};

	struct Paint {
		PaintType type;
		union {
			unsigned int color;
			Gradient* gradient;
		};
	};

	struct PathStruct
	{
		float* pts;					// Cubic bezier points: x0,y0, [cpx1,cpx1,cpx2,cpy2,x1,y1], ...
		int npts;					// Total number of bezier points.
		char closed;				// Flag indicating if shapes should be treated as closed.
		float bounds[4];			// Tight bounding box of the shape [minx,miny,maxx,maxy].

		PathStruct* next;		// Pointer to next path, or NULL if last element.
	};

	struct ShapeStruct
	{
		char id[64];				// Optional 'id' attr of the shape or its group
		Paint fill;				// Fill paint
		Paint stroke;			// Stroke paint
		float opacity;				// Opacity of the shape.
		float strokeWidth;			// Stroke width (scaled).
		float strokeDashOffset;		// Stroke dash offset (scaled).
		float strokeDashArray[8];			// Stroke dash array (scaled).
		char strokeDashCount;				// Number of dash values in dash array.
		LineJoin strokeLineJoin;		// Stroke join type.
		LineCap strokeLineCap;			// Stroke cap type.
		FillRule fillRule;				// Fill rule, see NSVGfillRule.
		Flags flags;		// Logical or of NSVG_FLAGS_* flags
		float bounds[4];			// Tight bounding box of the shape [minx,miny,maxx,maxy].

		PathStruct* paths;			// Linked list of paths in the image.
		ShapeStruct* next;		// Pointer to next shape, or NULL if last element.
	};

	struct ImageStruct
	{
		float width;				// Width of the image.
		float height;				// Height of the image.
		ShapeStruct* shapes;			// Linked list of shapes in the image.
	};

	struct Coordinate {
		float value;
		Units units;
	};

	struct LinearData {
		Coordinate x1, y1, x2, y2;
	};

	struct RadialData {
		Coordinate cx, cy, r, fx, fy;
	};

	struct GradientData
	{
		char id[64];
		char ref[64];
		PaintType type;
		union {
			LinearData linear;
			RadialData radial;
		};
		SpreadType spread;
		GradientUnits units;
		float xform[6];
		int nstops;
		GradientStop* stops;
		struct GradientData* next;
	};

	struct Attrib
	{
		char id[64];
		float xform[6];
		unsigned int fillColor;
		unsigned int strokeColor;
		float opacity;
		float fillOpacity;
		float strokeOpacity;
		char fillGradient[64];
		char strokeGradient[64];
		float strokeWidth;
		float strokeDashOffset;
		float strokeDashArray[NSVG_MAX_DASHES];
		int strokeDashCount;
		LineJoin strokeLineJoin;
		LineCap strokeLineCap;
		FillRule fillRule;
		float fontSize;
		unsigned int stopColor;
		float stopOpacity;
		float stopOffset;
		char hasFill;
		char hasStroke;
		char visible;
	};

	struct Parser
	{
		Attrib attr[MAX_ATTR];
		int attrHead;
		float* pts;
		int npts;
		int cpts;
		PathStruct* plist;
		ImageStruct* image;
		GradientData* gradients;
		float viewMinx, viewMiny, viewWidth, viewHeight;
		int alignX, alignY, alignType;
		float dpi;
		char pathFlag;
		char defsFlag;
	};

	/*
	 *	Functions
	*/
	// Parses SVG file from a file, returns SVG image as paths.
	ImageStruct* parseFromFile(const char* filename, const char* units, float dpi);

	// Parses SVG file from a null terminated string, returns SVG image as paths.
	// Important note: changes the string.
	ImageStruct* parse(char* input, const char* units, float dpi);

	// Deletes list of paths.
	void deleteImage(ImageStruct* image);
}
}
#endif // NANOSVG_H
