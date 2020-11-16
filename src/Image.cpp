/* ====================================================== *
 * nanosvg++                                              *
 * This software is a fork of nanosvg (nanosvgrastr.hpp). *
 * No license-related aspects are affected.               *
 * Kamil Koczurek | koczurekk@gmail.com                   *
 * ====================================================== */
//! @file
//!
//! \brief
//! Image class implementation

#include "../include/Image.hpp"

namespace nsvg {
	Image::Image()
	{ }

	Image::Image(const Image& image) {
		image;
	}

	Image& Image::operator =(const Image& image) {
		image;
		return *this;
	}

	bool Image::loadFromFile(const std::string& filename, const std::string& units, const float dpi) {
		if(internal)
			cstyle::deleteImage(internal);

		internal = cstyle::parseFromFile(filename.c_str(), units.c_str(), dpi);

		if(!this->internal) {
			std::cout << "Couldn't parse SVG image!" << std::endl;
			this->internal = nullptr;
			return false;
		}

		return true;
	}

	bool Image::loadFromMemory(std::string input, const std::string& units, const float dpi) {
		if(internal)
			cstyle::deleteImage(internal);

		internal = cstyle::parse(&input[0], units.c_str(), dpi);

		if(!this->internal) {
			std::cout << "Couldn't parse SVG image!" << std::endl;
			this->internal = nullptr;
			return false;
		}

		return true;
	}

	cstyle::ImageStruct* Image::getInternalImage() {
		return this->internal;
	}

	ShapeVector Image::getShapes() {
		return ShapeVector(this->internal->shapes);
	}

	sf::Vector2f Image::getSize() const {
		return {
			this->internal->width,
			this->internal->height
		};
	}

	Image::~Image() {
		if(internal)
			cstyle::deleteImage(internal);
	}
}
