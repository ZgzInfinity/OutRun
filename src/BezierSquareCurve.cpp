/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! @file
//!
//! \brief
//! BezierSquareCurve implementation
#include "../include/BezierSquareCurve.hpp"
#include <iostream>

namespace sfc {
	/*
		Private
	*/
	void BezierSquareCurve::draw(sf::RenderTarget &target, sf::RenderStates states) const {
		if(this->m_mode == DrawMode::NONE)
			return;

		if(this->m_mode == DrawMode::DEBUG)
			target.draw(this->m_debugLines, 4, sf::LinesStrip);

		target.draw(this->m_vertices, states);
	}

	/*
		Protected
	*/
	void BezierSquareCurve::calculateVeritces(
	std::vector<sf::Vector2f> &vector,
	const float limitCurve,
	sf::Vertex *debugLines) {
		vector.clear();

		for(auto i = 0u; i < getPointCount() && float(i) / getPointCount() <= limitCurve; ++i)
			vector.push_back(this->getPoint(i));

		if(debugLines) {
			debugLines[0] = {this->m_begin, sf::Color(0, 255, 0)};
			debugLines[1] = {this->m_controlPoint, sf::Color(0, 255, 0)};
			debugLines[2] = {this->m_controlPoint, sf::Color(0, 255, 0)};
			debugLines[3] = {this->m_end, sf::Color(0, 255, 0)};
		}
	}

	/*
		Public
	*/
	BezierSquareCurve::BezierSquareCurve() {
		this->m_color = {255, 255, 255};
		this->m_limitCurve = 1.f;
		this->m_pointCount = 32;
	}

	BezierSquareCurve::BezierSquareCurve(const sf::Vector2f &start,
	const sf::Vector2f &end,
	const sf::Vector2f &controlPoint,
	DrawMode mode) {
		this->m_color = {255, 255, 255};
		this->m_limitCurve = 1.f;
		this->m_pointCount = 32;

		this->m_mode = mode;

		this->m_controlPoint = controlPoint;
		this->m_begin = start;
		this->m_end = end;
	}

	Line2<float> BezierSquareCurve::getHelperLine(const float normalized) const {
		auto tmpLinePoint1 = this->m_begin + (this->m_controlPoint - this->m_begin) * normalized;
		auto tmpLinePoint2 = this->m_controlPoint + (this->m_end - this->m_controlPoint) * normalized;
		auto tmpLineVector = tmpLinePoint2 - tmpLinePoint1;

		return Line2<float>(tmpLinePoint1, tmpLineVector);
	}
	sf::Vector2f BezierSquareCurve::getPoint(std::size_t index) const {
		auto normalized = static_cast<float>(index) / this->m_pointCount;

		auto line = getHelperLine(normalized);
		return (line.point + line.vector * normalized);
	}

	void BezierSquareCurve::setPointPosition(BezierSquareCurve::PointType point, sf::Vector2f pos) {
		switch (point) {
		case CONTROL:
			this->m_controlPoint = pos;
			break;
		case BEGIN:
			this->m_begin = pos;
			break;
		case END:
			this->m_end = pos;
			break;
		}
	}
	sf::Vector2f BezierSquareCurve::getPointPosition(BezierSquareCurve::PointType point) {
		switch (point) {
		case CONTROL:
			return this->m_controlPoint;
		case BEGIN:
			return this->m_begin;
		case END:
			return this->m_end;
		default:
			return {0.f, 0.f};
		}
	}

	void BezierSquareCurve::update() {
		std::vector<sf::Vector2f> points;
		calculateVeritces(
		points,
		this->m_limitCurve,
		this->m_mode == DrawMode::NORMAL ? nullptr : this->m_debugLines);

		this->m_vertices.clear();
		for (const auto point: points)
			this->m_vertices.push_back({point, this->m_color});
	}
}
