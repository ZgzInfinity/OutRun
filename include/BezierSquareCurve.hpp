/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! @file
//!
//! \brief
//! BezierSquareCurve declaration

#ifndef BEZIERSQUARECURVE_H
#define BEZIERSQUARECURVE_H

#include "BezierAbstractCurve.hpp"

namespace sfc {
	//! \class BezierSquareCurve
	//!
	//! \brief Bezier square curve representation
	//!
	//! Can be drawn, inherits BezierAbstractCurve. <br />
	//! Use to draw/calculate simple curves.
	class BezierSquareCurve
	: public BezierAbstractCurve {
	private:
		sf::Vertex m_debugLines[4];
		sf::Vector2f m_controlPoint;

		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	protected:
		/* Commented in base class */
		virtual void calculateVeritces(std::vector<sf::Vector2f>&, const float, sf::Vertex*);

	public:
		//! \brief Point type enum
		//!
		//! Used to easly set/get points with one function – setPointPosition / getPointPosition.
		enum PointType {
			//! \brief Control point (handle)
			CONTROL,

			//! \brief Beginning of the curve
			BEGIN,

			//! \brief End of the curve
			END
		};

		//! \brief Default constructor
		//!
		//! Sets point count to 32 and color to white.
		BezierSquareCurve();

		//! \brief Specialized constructor
		//!
		//! \param [in] start – beginning of the curve
		//! \param [in] end – end of the curve
		//! \param [in] controlPoint – second control point
		//!
		//! Sets point count to 32 and color to white.
		//! Calls calculateCubic with proper args.
		BezierSquareCurve(const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, DrawMode = DrawMode::NORMAL);

		/* Commented in base class */
		virtual Line2<float> getHelperLine(const float) const;

		/* Commented in base class */
		virtual sf::Vector2f getPoint(std::size_t) const;

		/* Commented in base class */
		virtual void update();

		//! \brief Sets position of selected point
		//!
		//! \param [in] point – point type (PointType enum)
		//! \param [in] pos – new position
		void setPointPosition(PointType, sf::Vector2f);

		//! \brief Gets position of selected point
		//!
		//! \param [in] point – point type (PointType enum)
		//!
		//! \returns sf::Vector2f (position)
		sf::Vector2f getPointPosition(PointType);
	};
}

#endif // BEZIERSQUARECURVE_H
