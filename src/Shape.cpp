/* ====================================================== *
 * nanosvg++                                              *
 * This software is a fork of nanosvg (nanosvgrastr.hpp). *
 * No license-related aspects are affected.               *
 * Kamil Koczurek | koczurekk@gmail.com                   *
 * ====================================================== */
//! @file
//!
//! \brief
//! Shape class implementation

#include "../include/Shape.hpp"

namespace nsvg {
	Shape::Shape(const cstyle::ShapeStruct *ptr)
		: internal { ptr }
	{ }

	std::string Shape::getId() const {
		return std::string(this->internal->id);
	}

	cstyle::Paint Shape::getFill() const {
		return this->internal->fill;
	}

	cstyle::Paint Shape::getStroke() const {
		return this->internal->stroke;
	}

	float Shape::getOpacity() const {
		return this->internal->opacity;
	}

	float Shape::getStrokeWidth() const {
		return this->internal->strokeWidth;
	}

	float Shape::getStrokeDashOffset() const {
		return this->internal->strokeDashOffset;
	}

	const float* Shape::getStrokeDashArray() const {
		return this->internal->strokeDashArray;
	}

	size_t Shape::getStrokeDashCount() const {
		return this->internal->strokeDashCount;
	}

	LineJoin Shape::getStrokeLineJoin() const {
		return this->internal->strokeLineJoin;
	}

	LineCap Shape::getStrokeLineCap() const {
		return this->internal->strokeLineCap;
	}

	Flags Shape::getFlags() const {
		return this->internal->flags;
	}

	const float* Shape::getBounds() const {
		return this->internal->bounds;
	}

	PathVector Shape::getPaths() const {
		return PathVector(this->internal->paths);
	}

	Shape Shape::getNextShape() const {
		return Shape(this->internal->next);
	}

	bool Shape::good() const  {
		return internal;
	}

	Shape& Shape::operator++() {
		return (*this = this->getNextShape());
	}

	Shape::operator bool() const {
		return this->good();
	}
}
