/* ====================================================== *
 * nanosvg++                                              *
 * This software is a fork of nanosvg (nanosvgrastr.hpp). *
 * No license-related aspects are affected.               *
 * Kamil Koczurek | koczurekk@gmail.com                   *
 * ====================================================== */
//! @file
//!
//! \brief
//! Nanosvg++ Image class declaration

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>

#include "nanosvg.hpp"
#include "Shape.hpp"

namespace nsvg {
	//! \class Image
	//!
	//! \brief C++-styled cstyle::ImageStruct wrapper
	//!
	//! Provides object orientated interface for C-style nsvg::cstyle::ImageStruct.
	class Image {
		cstyle::ImageStruct* internal = nullptr;

	public:
		//! \brief Default constructor
		//!
		//! Does nothing;
		Image();

		//! \brief Copy constructor
		//!
		//! Protects from double-free error afrer assigning.
		Image(const Image& image);

		//! \brief Assignment operator
		//!
		//! Protects from double-free error afrer assigning.
		Image& operator =(const Image& image);

		//! \brief Parses SVG from file
		//!
		//! \param [in] filename – path to file
		//! \param [in] units – passed to nanosvg
		//! \param [in] dpi – passed to nanosvg
		//!
		//! \return True if ok, False otherwise
		bool loadFromFile(const std::string& filename, const std::string& units, const float dpi = 96.f);

		//! \brief Parses SVG from memory
		//!
		//! \param [in] input – null-terminated data
		//! \param [in] units – passed to nanosvg
		//! \param [in] dpi – passed to nanosvg
		//!
		//! \return True if ok, False otherwise
		bool loadFromMemory(std::string input, const std::string& units, const float dpi = 96.f);

		//! \brief Get internal image
		//!
		//! \return C-styled nsvg::ImageStruct
		cstyle::ImageStruct* getInternalImage();

		//! \brief Get shape vector
		//!
		//! \return ShapeVector instance
		ShapeVector getShapes();

		//! \brief Get size
		//!
		//! \return Image's size in sf::Vector2f
		sf::Vector2f getSize() const;

		//! \brief Destructor
		//!
		//! Releases resources if any were allocated
		~Image();
	};
}

#endif // IMAGE_HPP
