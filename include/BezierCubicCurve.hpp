/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! @file
//!
//! \brief
//! BezierCubicCurve declaration

#ifndef BEZIERCUBICCURVE_HPP
#define BEZIERCUBICCURVE_HPP
#include "BezierAbstractCurve.hpp"

namespace sfc {
	//! \class BezierCubicCurve
	//!
	//! \brief Bezier cubic curve representation
	//!
	//! Can be drawn, inherits BezierAbstractCurve. <br />
	//! Use to draw/calculate more complex curves, like entities paths.
	class BezierCubicCurve
	: public BezierAbstractCurve {
	private:
		sf::Vertex debugLines[6];

		sf::Vector2f m_beginControl;
		sf::Vector2f m_endControl;

		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	protected:
		/* Commented in base class */
		virtual void calculateVeritces(std::vector<sf::Vector2f>&, const float, sf::Vertex*);

	public:
		//! \brief Point type enum
		//!
		//! Used to easly set/get points with one function – setPointPosition / getPointPosition.
		enum PointType {
			//! \brief First control point
			BEGIN_CONTROL,

			//! \brief Second control point
			END_CONTROL,

			//! \brief Beginning of the curve
			BEGIN,

			//! \brief End of the curve
			END
		};

		//! \brief Default constructor
		//!
		//! Sets point count to 32 and color to white.
		BezierCubicCurve();

		//! \brief Specialized constructor
		//!
		//! \param [in] start – beginning of the curve
		//! \param [in] end – end of the curve
		//! \param [in] startControl – first control point
		//! \param [in] endControl – second control point
		//!
		//! Sets point count to 32 and color to white.
		//! Calls calculateCubic with proper args.
		BezierCubicCurve(const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, const sf::Vector2f&, DrawMode = DrawMode::NORMAL);

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

		//! \brief Gets 1st helper line used to draw curve
		//!
		//! \param [in] normalized – Normalized ([0, 1]) progress
		Line2<float> getCubicHelperLine1(const float);

		//! \brief Gets 2nd helper line used to draw curve
		//!
		//! \param [in] normalized – Normalized ([0, 1]) progress
		Line2<float> getCubicHelperLine2(const float);
	};
}

#endif // BEZIERCUBICCURVE_HPP
