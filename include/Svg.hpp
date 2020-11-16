/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! \file
//!
//! \brief
//! SVGImage class declaration

#ifndef SVG_HPP
#define SVG_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "nsvgpp.hpp"

#include "BezierCubicCurve.hpp"

namespace sfc {
	//! \class SVGImage
	//!
	//! \brief SVG-XML image support for SFML
	//!
	//! Class uses Cubic Bezier Curves to draw SVG graphics, can be drawn as set of curves or rasterized to create normal sf::Image.
	class SVGImage
	: public sf::Drawable {
		std::vector<std::unique_ptr<BezierCubicCurve>> m_curves;
		DrawMode m_mode = DrawMode::NORMAL;
		nsvg::Image m_image;

		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		void update();

	public:
		//! \brief Default Constructor
		//!
		//! Initializes internal variables
		SVGImage();

		//! \brief Constructor loading from file
		//!
		//! \param [in] file – path to .svg file
		//! \param [in] dpi – dots per inch/passed to nanosvg
		//!
		//! Calls loadFromFile with given params
		SVGImage(const std::string& file, const float dpi = 96.f);

		//! \brief Set curve drawing mode
		//!
		//! \param [in] mode – Drawing mode (normal / debug)
		//!
		//! If DrawMode::DEBUG parameter is given, non-rasterized image shows points used to create curves.
		//! Remember, new mode is applied only after re-loading SVG image!
		void setMode(const DrawMode m_mode);

		//! \brief Opens .svg file
		//!
		//! \param [in] file – path to .svg file
		//! \param [in] dpi – dots per inch/passed to nanosvg
		//!
		//! \returns True if ok, False otherwise
		//!
		//! Method that parses .svg file and creates proper Bezier Curves.
		bool loadFromFile(const std::string& file, const float dpi = 96.f);

		//! \brief Loads SVG image from memory
		//!
		//! \param [in] data – pointer to null-terminated data
		//! \param [in] size – data count
		//! \param [in] dpi – dots per inch/passed to nanosvg
		//!
		//! \return True if ok, False otherwise
		//!
		//! Method that parses SVG-XML image from memory and creates proper Bezier Curves.
		bool loadFromMemory(const void* data, size_t size, const float dpi = 96.f);

		//! \brief Loads SVG image from stream
		//!
		//! \param [in] stream – SFML input stream instance
		//! \param [in] dpi – dots per inch/passed to nanosvg
		//!
		//! \return True if ok, False otherwise
		//!
		//! Method that parses SVG-XML image from SFML input stream and creates proper Bezier Curves.
		bool loadFromStream(sf::InputStream& stream, const float dpi = 96.f);

		//! \brief Rasterizes SVG
		//!
		//! \param [in] scale – factor to scale the image by/passed to nanosvg
		//!
		//! \returns Rasterized sf::Image
		sf::Image rasterize(const float scale = 1.f);

		//! \brief Translate image
		//!
		//! \param [in] vec – vector by which image will be translated
		void move(sf::Vector2f vec);

		//! \brief Scale image
		//!
		//! \param [in] factor – scale to apply
		void scale(const float factor);

		//! \brief Returns size of image
		//!
		//! \returns sf::Vector2f (size)
		sf::Vector2f getSize() const;
	};
}

#endif // SVG_HPP
