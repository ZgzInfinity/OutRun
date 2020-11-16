/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com             *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */
//! @file
//!
//! \brief
//! Base header used by (probably) all sf-svg classes.

#ifndef SFAS_BASE_HPP
#define SFAS_BASE_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

//! \brief Namespace that contains all sf-svg method, classes, enums etc.
namespace sfc {

	//! \brief Way to draw curves.
	enum class DrawMode {
		//! \brief Just draw the curve
		NORMAL,

		//! \brief Draw also helper lines etc.
		DEBUG,

		//! \brief Don't draw curve
		NONE
	};

	//! \class Line2
	//!
	//! \brief 2D line
	//!
	//! was written to be used in sf-svg back-end, but actually can be used in any project that uses SFML.
	//! Line is defined as a pair of point and a vector (offset). Because of this, such code:
	//!
	//! \code{.cpp}
	//! Line2<float> line({50, 50}, {60, 0});
	//! // ...
	//! window.draw(line);
	//! \endcode
	//!
	//! Will draw a line between [50, 50] and [110, 50].
	template<typename T>
	struct Line2
	: public sf::Drawable {

		//! \brief Beginning of the line
		sf::Vector2<T> point;

		//! \brief Vector (offset)
		//!
		//! End of the line is point + vector.
		sf::Vector2<T> vector;

		//! \brief Color of the line
		sf::Color color;

		//! \brief Default constructor
		//!
		//! Sets variables to proper values (white line between [0, 0] and [0, 0]).
		Line2() {
			point = {0, 0};
			vector = {0, 0};
			color = {255, 255, 255};
		}

		//! \brief Specialized constructor.
		//!
		//! \param [in] point – beginning
		//! \param [in] vector – offset
		//! \param [in] color – color of the line, white by default
		//!
		//! Creates line using given parameters.
		Line2(const sf::Vector2<T> point, const sf::Vector2<T> vector, const sf::Color color = sf::Color::White) {
			this->point = point;
			this->vector = vector;
			this->color = color;
		}

		//! \brief Calculate length
		//!
		//! \returns Length of the line
		T length() {
			return sqrt(vector.x * vector.x + vector.y * vector.y);
		}

	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
			sf::Vertex tab[2] =
			{{point, this->color},
			{point + vector, this->color}};

			target.draw(tab, 2, sf::Lines, states);
		}
	};
}

#endif // SYSTEM_HPP
