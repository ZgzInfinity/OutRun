/* ====================================================== *
 * nanosvg++                                              *
 * This software is a fork of nanosvg (nanosvgrastr.hpp). *
 * No license-related aspects are affected.               *
 * Kamil Koczurek | koczurekk@gmail.com                   *
 * ====================================================== */
//! @file
//!
//! \brief
//! Path class declaration

#ifndef PATH_HPP
#define PATH_HPP

#include <SFML/System/Vector2.hpp>
#include <vector>

#include "nanosvg.hpp"

namespace nsvg {
	//! \brief Point set of Bezier Cubic Curve
	//!
	//! Can store all four points of any Bezier Cubic Curve.
	struct CubicPointSet {
		//! \brief
		//! Beginning of the curve
		sf::Vector2f begin;

		//! \brief
		//! End of the curve
		sf::Vector2f end;

		//! \brief
		//! First handle of the curve
		sf::Vector2f control1;

		//! \brief
		//! Second handle of the curve
		sf::Vector2f control2;
	};

	//! \class Path
	//!
	//! \brief C++-styled cstyle::PathStruct wrapper
	class Path {
	public:
		const cstyle::PathStruct* internal;

	public:
		//! \brief Constructor
		//!
		//! \param [in] ptr – const pointer to PathStruct
		//!
		//! It's the only constructor of nsvg::Path class, which wraps given cstyle::PathStruct into object orientated interface.
		Path(const cstyle::PathStruct* ptr);

		//! \brief Get points sets
		//!
		//! \return Set of points required to create Bezier curve (begin, end, 2 handles)
		//!
		//! Sample code:
		//! \code{.cpp}
		//! for(const auto& points: path.getPointsSets()){
		//! std::cout << "begin: [" << points.begin.x << ", " << points.begin.y << "]" << std::endl
		//! std::cout << "end: [" << points.end.x << ", " << points.end.y << "]" << std::endl
		//! std::cout << "handle 1: [" << points.control1.x << ", " << points.control1.y << "]" << std::endl
		//! std::cout << "handle 2: [" << points.control2.x << ", " << points.control2.y << "]" << std::endl
		//! }
		std::vector<CubicPointSet> getPointsSets() const;

		//! \brief Get next path
		//!
		//! \return Instance of shape wrapping this->internal->path.
		Path getNextPath() const;

		//! \brief Is ok?
		//!
		//! \return True if instance wraps proper pointer (not nullptr), False otherwise.
		bool good() const;

		//! \brief Switches itself to next path
		//!
		//! \return Reference to *this
		//!
		//! Equivalent to:
		//! \code{.cpp}
		//! samplePath = samplePath.getNextPath();
		//! \endcode
		Path& operator ++();

		//! \brief Cast nsvg::Path → bool
		//!
		//! Implicitly cast *this to bool, equivalent to nsvg::Path::good() method.
		operator bool() const;
	};

	//! \brief Vector of paths
	//!
	//! Basically normal vector of paths, but constructor takes cstyle::PathStruct pointer to initialize
	class PathVector
		: public std::vector<Path> {
	public:
		//! \brief Constructor
		//!
		//! \param [in] path_ptr – pointer to first cstyle::PathStruct
		PathVector(cstyle::PathStruct* path_ptr) {
			while(path_ptr) {
				emplace_back(path_ptr);
				path_ptr = path_ptr->next;
			}
		}
	};
}

#endif // PATH_HPP
