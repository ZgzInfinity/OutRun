/* ====================================================== *
 * nanosvg++                                              *
 * This software is a fork of nanosvg (nanosvgrastr.hpp). *
 * No license-related aspects are affected.               *
 * Kamil Koczurek | koczurekk@gmail.com                   *
 * ====================================================== */
//! @file
//!
//! \brief
//! Enums definitions

#ifndef ENUMS_HPP
#define ENUMS_HPP

namespace nsvg {
	/*
	 *	Enums
	*/
	enum class PaintType {
		NONE = 0,
		COLOR = 1,
		LINEAR_GRADIENT = 2,
		RADIAL_GRADIENT = 3,
	};

	enum class SpreadType {
		PAD = 0,
		REFLECT = 1,
		REPEAT = 2,
	};

	enum class LineJoin {
		MITER = 0,
		ROUND = 1,
		BEVEL = 2,
	};

	enum class LineCap {
		BUTT = 0,
		ROUND = 1,
		SQUARE = 2,
	};

	enum class FillRule {
		NONZERO = 0,
		EVENODD = 1,
	};

	enum class Flags {
		NSVG_FLAGS_VISIBLE = 0x01,
		INVISIBLE = 0x00
	};

	enum class GradientUnits {
		USER_SPACE = 0,
		OBJECT_SPACE = 1,
	};

	//! \brief Available units
	enum class Units {
		//! \brief
		//! Unknown unit
		UNITS_USER,

		//! \brief
		//! Pixels (Relative to the viewing device)
		UNITS_PX,

		//! \brief
		//! Point (1/72th of an inch).
		UNITS_PT,

		//! \brief
		//! Pica (12 points).
		UNITS_PC,

		//! \brief
		//! Milimetre
		UNITS_MM,

		//! \brief
		//! Centimetre
		UNITS_CM,

		//! \brief
		//! Inch
		UNITS_IN,

		//! \brief
		//! Percent
		UNITS_PERCENT,

		//! \brief
		//! Used for fonts
		UNITS_EM,

		//! \brief
		//! X-height of the element
		UNITS_EX,
	};
}

#endif // ENUMS_HPP
