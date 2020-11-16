/* ====================================================== *
 * nanosvg++                                              *
 * This software is a fork of nanosvg (nanosvgrastr.hpp). *
 * No license-related aspects are affected.               *
 * Kamil Koczurek | koczurekk@gmail.com                   *
 * ====================================================== */
//! @file
//!
//! \brief
//! Rasterizer class declaration

#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include <SFML/Graphics/Image.hpp>
#include <vector>

#include "nanosvgrastr.hpp"
#include "nanosvg.hpp"
#include "Image.hpp"

namespace nsvg {
	//! \class Rasterizer
	//!
	//! \brief C++-styled cstyle::RasterizerStruct wrapper
	class Rasterizer {
		cstyle::RasterizerStruct* internal;

	public:
		//! \brief Default constructor
		//!
		//! Initializes internal cstyle::RasterizerStruct pointer.
		Rasterizer();

		//! \brief Copy constructor
		//!
		//! Doesn't really copy anything, does exactly what default constructor;
		Rasterizer (const Rasterizer& rasterizer);

		//! \brief Destructor
		//!
		//! Deletes internal cstyle::RasterizerStruct pointer.
		~Rasterizer();

		//! \brief Copy-assignment operator
		//!
		//! Doesn't really copy anything yet, exists to avoid assigning 'internal' pointer.
		Rasterizer& operator= (const Rasterizer& rasterizer);

		//! \brief Rasterizes given Image.
		//!
		//! \param [in] image – Reference to SVG image
		//! \param [in] tx – X offset
		//! \param [in] ty – Y offset
		//! \param [in] scale – Raster scale
		//!
		//! \return Rasterized image
		sf::Image rasterize(Image& image, float tx = .0f, float ty = .0f, float scale = 1.f);
	};
}

#endif // RASTERIZER_HPP
