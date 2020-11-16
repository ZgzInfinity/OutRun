/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! @file
//!
//! \brief
//! BezierCubicCurve implementation
#include "../include/BezierCubicCurve.hpp"
#include <iostream>

namespace sfc {
	/*
		Private
	*/
	void BezierCubicCurve::draw(sf::RenderTarget &target, sf::RenderStates states) const {
		if(this->m_mode == DrawMode::NONE)
			return;

		if(this->m_mode == DrawMode::DEBUG)
			target.draw(this->debugLines, 6, sf::Lines, states);

		target.draw(this->m_vertices, states);
	}

	/*
		Protected
	*/
	void BezierCubicCurve::calculateVeritces(
	std::vector<sf::Vector2f> &vector,
	const float limitCurve,
	sf::Vertex* debugLines) {
		vector.clear();

		for(auto i = 0u; i < getPointCount() && float(i) / getPointCount() <= limitCurve; ++i)
			vector.push_back(this->getPoint(i));

		if(debugLines) {
			debugLines[0] = {this->m_begin, sf::Color(0, 255, 0)};
			debugLines[1] = {this->m_beginControl, sf::Color(0, 255, 0)};
			debugLines[2] = {this->m_beginControl, sf::Color(0, 255, 0)};
			debugLines[3] = {this->m_endControl, sf::Color(0, 255, 0)};
			debugLines[4] = {this->m_endControl, sf::Color(0, 255, 0)};
			debugLines[5] = {this->m_end, sf::Color(0, 255, 0)};
		}
	}

	/*
		Public
	*/
	BezierCubicCurve::BezierCubicCurve() {
		this->m_pointCount = 32;
		this->m_color = {255, 255, 255};
	}

	BezierCubicCurve::BezierCubicCurve(const sf::Vector2f &start,
	const sf::Vector2f &end,
	const sf::Vector2f &startControl,
	const sf::Vector2f &endControl,
	DrawMode mode) {
		this->m_color = {255, 255, 255};
		this->m_pointCount = 32;
		this->m_mode = mode;

		this->m_begin = start;
		this->m_end = end;

		this->m_beginControl = startControl;
		this->m_endControl = endControl;
	}

	Line2<float> BezierCubicCurve::getHelperLine(const float normalized) const {
		auto loopedLine1Point = this->m_begin + (this->m_beginControl - this->m_begin) * normalized;
		auto loopedLine1Vector = (this->m_beginControl + (this->m_endControl - this->m_beginControl) * normalized) - loopedLine1Point;
		auto loopedLine2Point = this->m_beginControl + (this->m_endControl - this->m_beginControl) * normalized;
		auto loopedLine2Vector = (this->m_endControl + (this->m_end - this->m_endControl) * normalized) - loopedLine2Point;

		auto tmpLinePoint1 = loopedLine1Point + loopedLine1Vector * normalized;
		auto tmpLinePoint2 = loopedLine2Point + loopedLine2Vector * normalized;
		auto tmpLineVector = tmpLinePoint2 - tmpLinePoint1;

		return Line2<float>(tmpLinePoint1, tmpLineVector, sf::Color(255, 0, 0));
	}

	sf::Vector2f BezierCubicCurve::getPoint(std::size_t index) const {
		auto normalized = static_cast<float>(index) / this->m_pointCount;

		auto line = getHelperLine(normalized);
		return (line.point + line.vector * normalized);
	}

	void BezierCubicCurve::update() {
		std::vector<sf::Vector2f> points;
		calculateVeritces(
		points,
		this->m_limitCurve,
		this->m_mode == DrawMode::DEBUG ? debugLines : nullptr);

		this->m_vertices.clear();
		for (auto& point: points)
			this->m_vertices.push_back({point, this->m_color});
	}

	void BezierCubicCurve::setPointPosition(BezierCubicCurve::PointType point, sf::Vector2f pos) {
		switch (point) {
		case BEGIN:
			this->m_begin = pos;
			break;
		case END:
			this->m_end = pos;
			break;
		case BEGIN_CONTROL:
			this->m_beginControl = pos;
			break;
		case END_CONTROL:
			this->m_endControl = pos;
			break;
		}
	}
	sf::Vector2f BezierCubicCurve::getPointPosition(BezierCubicCurve::PointType point) {
		switch (point) {
		case BEGIN:
			return this->m_begin;
		case END:
			return this->m_end;
		case BEGIN_CONTROL:
			return this->m_beginControl;
		case END_CONTROL:
			return this->m_endControl;
		default:
			return {0.f, 0.f};
		}
	}

	Line2<float> BezierCubicCurve::getCubicHelperLine1(const float normalized) {
		auto point = this->m_begin + (this->m_beginControl - this->m_begin) * normalized;
		auto vector = (this->m_beginControl + (this->m_endControl - this->m_beginControl) * normalized) - point;

		return Line2<float>(point, vector, sf::Color(255, 255, 0));
	}

	Line2<float> BezierCubicCurve::getCubicHelperLine2(const float normalized) {
		auto point = this->m_beginControl + (this->m_endControl - this->m_beginControl) * normalized;
		auto vector = (this->m_endControl + (this->m_end - this->m_endControl) * normalized) - point;

		return Line2<float>(point, vector, sf::Color(255, 255, 0));
	}
}
