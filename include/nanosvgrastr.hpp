/* ================================================ *
 * nanosvg++                                        *
 * This software is a fork of nanosvg (note below). *
 * No license-related aspects are affected.         *
 * Kamil Koczurek | koczurekk@gmail.com             *
 * ================================================ */
/* ============================================================================ *
 * Copyright (c) 2013-14 Mikko Mononen memon@inside.org                         *
 *                                                                              *
 * This software is provided 'as-is', without any express or implied            *
 * warranty.  In no event will the authors be held liable for any damages       *
 * arising from the use of this software.                                       *
 *                                                                              *
 * Permission is granted to anyone to use this software for any purpose,        *
 * including commercial applications, and to alter it and redistribute it       *
 * freely, subject to the following restrictions:                               *
 *                                                                              *
 * 1. The origin of this software must not be misrepresented; you must not      *
 * claim that you wrote the original software. If you use this software         *
 * in a product, an acknowledgment in the product documentation would be        *
 * appreciated but is not required.                                             *
 * 2. Altered source versions must be plainly marked as such, and must not be   *
 * misrepresented as being the original software.                               *
 * 3. This notice may not be removed or altered from any source distribution.   *
 *                                                                              *
 * The polygon rasterization is heavily based on stb_truetype rasterizer        *
 * by Sean Barrett - http://nothings.org/                                       *
 * ============================================================================ */

//! @file
//!
//! \brief
//! Nanosvg++ rasterizer declaration

#ifndef NANOSVGRASTR_HPP
#define NANOSVGRASTR_HPP

#include "nanosvg.hpp"

namespace nsvg {
namespace cstyle {
	#define SUBSAMPLES   5
	#define FIXSHIFT     10
	#define FIX	         (1 << FIXSHIFT)
	#define FIXMASK	     (FIX-1)
	#define MEMPAGE_SIZE 1024

	struct Edge {
		float x0,y0, x1,y1;
		int dir;

		Edge* next;
	};

	struct Point {
		float x, y;
		float dx, dy;
		float len;
		float dmx, dmy;
		unsigned char flags;
	};

	struct ActiveEdge {
		int x,dx;
		float ey;
		int dir;

		ActiveEdge *next;
	};

	struct MemPage {
		unsigned char mem[MEMPAGE_SIZE];
		int size;

		MemPage* next;
	};

	struct CachedPaint {
		PaintType type;
		SpreadType spread;
		float xform[6];
		unsigned int colors[256];
	};

	struct RasterizerStruct
	{
		float px, py;

		float tessTol;
		float distTol;

		Edge* edges;
		int nedges;
		int cedges;

		Point* points;
		int npoints;
		int cpoints;

		Point* points2;
		int npoints2;
		int cpoints2;

		ActiveEdge* freelist;
		MemPage* pages;
		MemPage* curpage;

		unsigned char* scanline;
		int cscanline;

		unsigned char* bitmap;
		int width, height, stride;
	};

	// Allocated rasterizer context.
	RasterizerStruct* createRasterizer();

	// Rasterizes SVG image, returns RGBA image (non-premultiplied alpha)
	//   r - pointer to rasterizer context
	//   image - pointer to image to rasterize
	//   tx,ty - image offset (applied after scaling)
	//   scale - image scale
	//   dst - pointer to destination image data, 4 bytes per pixel (RGBA)
	//   w - width of the image to render
	//   h - height of the image to render
	//   stride - number of bytes per scaleline in the destination buffer
	void rasterize(RasterizerStruct* r,
					   ImageStruct* image, float tx, float ty, float scale,
					   unsigned char* dst, int w, int h, int stride);

	// Deletes rasterizer context.
	void deleteRasterizer(RasterizerStruct*);
}
}

#endif // NANOSVGRASTR_HPP
