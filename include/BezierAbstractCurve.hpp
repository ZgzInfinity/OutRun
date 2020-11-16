/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! @file
//!
//! \brief
//! BezierAbstractCurve declaration and partial implementation.


/*! \mainpage sf-svg
 *
 * \section About
 * SFML-based library to draw .svg files and Bézier Curves.
 *
 * \section Installation
 * \subsection Compilation
 * \code{.sh}
 * $ git clone https://github.com/KoczurekK/sf-svg.git
 * $ cd sf-svg
 * $ qmake curves-shared.pro
 * $ make
 * \endcode
 * Now copy SFC headers to /usr/include/SFC or to any other path supported on your OS. Then copy .so/.dll files wherever you want to.
 *
 * \subsubsection Binary builds
 * Download leatest binary build from releases on github.
 *
 * \section Usage
 * Include useful files & remember to pass linker option -lsfsvg.
 *
 * \section Features
 * - SVG images drawing;
 * - Loading SVG from files / memory / stream;
 * - SVG rasterization;
 * - Implements Square- and Cubic Bézier curves;
 * - Curves inherit sf::Shape;
 * - Provides interface for Bézier curves;
 * - Provides debug-friendly features like "helper lines" etc.
 */

#ifndef BEZIERABSTRACTCURVE_H
#define BEZIERABSTRACTCURVE_H

#include <SFML/Graphics/Shape.hpp>
#include "VertexVector.hpp"
#include "Base.hpp"

namespace sfc {
	//! \class BezierAbstractCurve
	//!
	//! \brief Abstract Bezier curve class
	//!
	//! Do not define methods! <br />
	//! Inherit it to create class compatibile with Brezier Curve API.<br />
	//! Remember, it's not an interface, this class contains variables
	//! and some definied methods – getters & setters (mostly).
	class BezierAbstractCurve:
	public sf::Shape {
	protected:
		//! \brief Vertices used to draw curve
		VertexVector<sf::LinesStrip> m_vertices;

		//! \brief Stores points count
		std::size_t m_pointCount;

		//! \brief Curve's color
		sf::Color m_color;

		//! \brief First point of a curve
		sf::Vector2f m_begin;

		//! \brief Last point of a curve
		sf::Vector2f m_end;

		//! \brief Part of curve to be drawn
		//!
		//! Real number in range [0, 1], if set to .5 only 50% of curve will be drawn etc.
		float m_limitCurve;

		//! \brief Drawing mode
		//!
		//! NORMAL – Just draw curve <br />
		//! DEBUG – Draw also helper lines etc.
		DrawMode m_mode;

	public:
		//! \brief Get last line used to draw a curve
		//!
		//! \param [in] normalized – progress of drawing curve in range [0, 1]
		//!
		//! Other helper lines are ignored, this function returns the one, on which "pencil" is placed.
		virtual Line2<float> getHelperLine(const float) const = 0;

		//! \brief Calculates vertices
		//!
		//! Changes are applied only when this function is called.
		//! \code{.cpp}
		//! sfc::BezierCubicCurve curve(arg1, arg2, arg3, arg4);
		//! curve.update();
		//! //...
		//! curve.setColor({68, 69, 70}); //Ignored
		//! window.draw(curve);
		//! \endcode
		virtual void update() = 0;

		//! \brief Returns point count
		virtual std::size_t getPointCount() const;

		//! \brief Get n-th point of curve
		//!
		//! \param [in] index – point index
		virtual sf::Vector2f getPoint(std::size_t) const = 0;

		//! \brief Virtual destructor
		//!
		//! Does nothing expect of ensuring that destructors of inherited classes will be called.
		virtual ~BezierAbstractCurve()
		{ }

		//! \brief Calculate points on cuve
		//!
		//! \param [out] vector – vector to store vertices
		//! \param [in]  limitCurve – part of curve to be drawn, range [0, 1]
		//! \param [out] debugLines – vertex array, used to store and draw helper lines
		//!
		//! Calculates all curve-related things like vertices positions / helper lines etc.
		//! Not a big deal, use only if you really want to inherit BezierCubicCurve.
		virtual void calculateVeritces(std::vector<sf::Vector2f>& vector, const float limitCurve, sf::Vertex* debugLines) = 0;

		//! \brief Sets color
		//!
		//! \param [in] color – New color
		//!
		//! Use to set color of curve's vertices.
		//! \code{.cpp}
		//! curve.setColor({255, 20, 20}); //Red
		//! \endcode
		void setColor(const sf::Color&);

		//! \brief Returns color of curve
		sf::Color getColor();

		//! \brief Set part of curve to be drawn
		//!
		//! \param [in] normalized – part of curve to be drawn in range [0, 1]
		//!
		//! \code{.cpp}
		//! curve.setNormalizedLengthLimit(.5f); //Draw 50% of the curve
		//! \endcode
		void setNormalizedLengthLimit(const float);

		//! \brief Get part of curve to be drawn
		//!
		//! Take a look at setNormalizedLengthLimit() and m_limitCurve.
		float getNormalizedLengthLimit();

		//! \brief Set amount of points to draw curve
		//!
		//! \param [in] count – Amount of points to use
		//!
		//! \code{.cpp}
		//! curve1.setPointCount(6); //Ugly one
		//! curve12.setPointCount(200); //OK in most cases
		//! curve2.setPointCount(6000); //Sooo good (actually too much)
		//! \endcode
		void setPointCount(const std::size_t);

		//! \brief Translate curve
		void move(sf::Vector2f);

		//! \brief Scale curve
		void scale(const sf::Vector2f &factor);
	};
}
#endif // BEZIERABSTRACTCURVE_H
