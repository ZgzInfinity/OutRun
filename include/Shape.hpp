/* ====================================================== *
 * nanosvg++                                              *
 * This software is a fork of nanosvg (nanosvgrastr.hpp). *
 * No license-related aspects are affected.               *
 * Kamil Koczurek | koczurekk@gmail.com                   *
 * ====================================================== */
//! @file
//!
//! \brief
//! Shape class declaration

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <string>
#include <vector>

#include "Path.hpp"
#include "nanosvg.hpp"

namespace nsvg {
	//! \class Shape
	//!
	//! \brief C++-styled cstyle::ShapeStruct wrapper
	class Shape {
		const cstyle::ShapeStruct* internal;

	public:
		//! \brief Constructor
		//!
		//! \param [in] ptr – const pointer to ShapeStruct
		//!
		//! It's the only constructor of nsvg::Shape class, which wraps given cstyle::ShapeStruct into object orientated interface.
		Shape(const cstyle::ShapeStruct *ptr);

		//! \brief Get ID
		//!
		//! \return Shape's ID
		std::string getId() const;

		//! \brief Get fill paint
		//!
		//! \return cstyle::Paint structure
		cstyle::Paint getFill() const;

		//! \brief Get stroke
		//!
		//! \return Stroke
		cstyle::Paint getStroke() const;

		//! \brief Get opacity
		//!
		//! \return Floating-point opacity
		float getOpacity() const;

		//! \brief Get stroke width
		//!
		//! \return Stroke width
		float getStrokeWidth() const;

		//! \brief Get stroke dash offset
		//!
		//! \return Stroke dash offset
		float getStrokeDashOffset() const;

		//! \brief Get stroke dash array
		//!
		//! \return Const float pointer to stroke dash array
		const float* getStrokeDashArray() const;

		//! \brief Get stroke dash count
		//!
		//! \return Unsigned stroke dash count
		size_t getStrokeDashCount() const;

		//! \brief Get stroke line join
		//!
		//! \return LineJoin enum
		LineJoin getStrokeLineJoin() const;

		//! \brief Get stroke line cap
		//!
		//! \return LineCap enum
		LineCap getStrokeLineCap() const;

		//! \brief get Flags
		//!
		//! \return Flags enum
		Flags getFlags() const;

		//! \brief Get bounds
		//!
		//! \return Const float pointer to bounds array
		const float* getBounds() const;

		//! \brief Get paths
		//!
		//! \return PathVector containing all the paths
		PathVector getPaths() const;

		//! \brief Get next shape
		//!
		//! \return Instance of shape wrapping this->internal->next.
		Shape getNextShape() const;

		//! \brief Is ok?
		//!
		//! \return True if instance wraps proper pointer (not nullptr), False otherwise.
		bool good() const;

		//! \brief Switches itself to next shape
		//!
		//! \return Reference to *this
		//!
		//! Equivalent to:
		//! \code{.cpp}
		//! sampleShape = sampleShape.getNextShape();
		//! \endcode
		Shape& operator ++();

		//! \brief Cast nsvg::Shape → bool
		//!
		//! Implicitly cast *this to bool, equivalent to nsvg::Shape::good() method.
		operator bool() const;
	};

	//! \brief Vector of shapes
	//!
	//! Basically normal vector of shapes, but constructor takes cstyle::ShapeStruct pointer to initialize
	class ShapeVector
		: public std::vector<Shape> {
	public:
		//! \brief Constructor
		//!
		//! \param [in] shape_ptr – pointer to first cstyle::ShapeStruct
		ShapeVector(cstyle::ShapeStruct* shape_ptr) {
			while(shape_ptr) {
				emplace_back(shape_ptr);
				shape_ptr = shape_ptr->next;
			}
		}
	};
}

#endif // SHAPE_HPP
