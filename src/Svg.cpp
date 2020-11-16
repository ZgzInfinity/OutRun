/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! @file
//!
//! \brief
//! SVGImage implementation

#include "../include/Svg.hpp"

namespace sfc {
	/*
		Private
	*/
	void SVGImage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if(this->m_mode == DrawMode::NONE)
			return;

		for(const auto& curve: this->m_curves) {
			target.draw(*curve, states);
		}
	}

	void SVGImage::update() {
		this->m_curves.clear();

		if(this->m_mode == DrawMode::NONE)
			return;

		for(const nsvg::Shape& shape: this->m_image.getShapes()) {
			for(const nsvg::Path& path: shape.getPaths()) {
				for(const auto& points: path.getPointsSets()) {
					this->m_curves.emplace_back(new BezierCubicCurve(
						points.begin,
						points.end,
						points.control1,
						points.control2,
						this->m_mode
					));

					(*this->m_curves.back()).setNormalizedLengthLimit(1.f);
					(*this->m_curves.back()).setPointCount(1024);
					(*this->m_curves.back()).update();
				}
			}
		}
	}

	/*
		Public
	*/
	SVGImage::SVGImage()
	{ }

	SVGImage::SVGImage(const std::string &file, const float dpi) {
		this->loadFromFile(file, dpi);
	}

	void SVGImage::setMode(const DrawMode mode) {
		this->m_mode = mode;
	}

	bool SVGImage::loadFromFile(const std::string &file, const float dpi) {
		if(!this->m_image.loadFromFile(file.c_str(), "px", dpi))
			return false;

		this->update();
		return true;
	}

	bool SVGImage::loadFromMemory(const void *data, size_t size, const float dpi) {
		if(static_cast<const sf::Uint8*>(data)[size - 1] != '\0') {
			std::cout << "SVG file in memory has to be null-terminated!" << std::endl;
			return false;
		}

		std::string copy = static_cast<const char*>(data);
		if(!this->m_image.loadFromMemory(copy, "px", dpi))
			return false;

		this->update();
		return true;
	}

	bool SVGImage::loadFromStream(sf::InputStream& stream, const float dpi) {
		std::vector<char> copy(stream.getSize() + 1);
		stream.read(copy.data(), stream.getSize());

		if(copy[stream.getSize() - 1] != '\0') {
			copy[stream.getSize()] = '\0';
		}

		bool ok = this->m_image.loadFromMemory(copy.data(), "px", dpi);

		if(!ok)
			return false;

		this->update();
		return true;
	}

	sf::Image SVGImage::rasterize(const float scale) {
		nsvg::Rasterizer rasterizer;
		auto img = rasterizer.rasterize(this->m_image, 0, 0, scale);
		return img;
	}

	void SVGImage::move(sf::Vector2f vec) {
		for(auto& curve: this->m_curves) {
			(*curve).move(vec);
		}
	}

	void SVGImage::scale(const float factor) {
		for(auto& curve: this->m_curves) {
			(*curve).scale({factor, factor});
		}
	}

	sf::Vector2f SVGImage::getSize() const {
		return this->m_image.getSize();
	}
}
