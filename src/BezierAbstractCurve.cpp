/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! @file
//!
//! \brief
//! BezierAbstractCurve implementation

#include "../include/BezierAbstractCurve.hpp"


namespace sfc {
	void BezierAbstractCurve::setColor(const sf::Color& color) {
		this->m_color = color;
	}
	sf::Color BezierAbstractCurve::getColor() {
		return this->m_color;
	}

	void BezierAbstractCurve::setNormalizedLengthLimit(const float limit) {
		this->m_limitCurve = limit;
	}
	float BezierAbstractCurve::getNormalizedLengthLimit() {
		return this->m_limitCurve;
	}

	void BezierAbstractCurve::setPointCount(const std::size_t count) {
		this->m_pointCount = count;
	}
	std::size_t BezierAbstractCurve::getPointCount() const {
		return this->m_pointCount;
	}

	void BezierAbstractCurve::move(sf::Vector2f diff) {
		for(auto& verticle : m_vertices)
			verticle.position += diff;
	}

	void BezierAbstractCurve::scale(const sf::Vector2f &factor) {
		for(auto& vertex: m_vertices) {
			vertex.position.x *= factor.x;
			vertex.position.y *= factor.y;
		}
	}
}
