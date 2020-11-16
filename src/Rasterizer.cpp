/* ====================================================== *
 * nanosvg++                                              *
 * This software is a fork of nanosvg (nanosvgrastr.hpp). *
 * No license-related aspects are affected.               *
 * Kamil Koczurek | koczurekk@gmail.com                   *
 * ====================================================== */
//! @file
//!
//! \brief
//! Rasterizer class implementation

#include "../include/Rasterizer.hpp"

namespace nsvg {
	Rasterizer::Rasterizer()
		: internal { cstyle::createRasterizer() }
	{ }

	Rasterizer::Rasterizer (const Rasterizer& rasterizer)
		: internal { cstyle::createRasterizer() } {
		rasterizer;
	}

	Rasterizer::~Rasterizer() {
		cstyle::deleteRasterizer(this->internal);
	}

	Rasterizer& Rasterizer::operator= (const Rasterizer& rasterizer) {
		rasterizer;
		return *this;
	}

	sf::Image Rasterizer::rasterize(Image& image, float tx, float ty, float scale) {
		sf::Vector2f size_f{image.getSize().x * scale, image.getSize().y * scale};
		sf::Vector2i size = static_cast<sf::Vector2i>(size_f);

		std::vector<sf::Uint8> pixels(static_cast<size_t>(size.x * size.y) * 4ul);
		sf::Image img;

		nsvg::cstyle::rasterize(
			this->internal,
			image.getInternalImage(),
			tx,
			ty,
			scale,
			&pixels[0],
			size.x,
			size.y,
			size.x * 4
		);

		img.create(static_cast<unsigned>(size.x), static_cast<unsigned>(size.y), &pixels[0]);

		return img;
	}
}
