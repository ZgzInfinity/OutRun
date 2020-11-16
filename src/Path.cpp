/* ====================================================== *
 * nanosvg++                                              *
 * This software is a fork of nanosvg (nanosvgrastr.hpp). *
 * No license-related aspects are affected.               *
 * Kamil Koczurek | koczurekk@gmail.com                   *
 * ====================================================== */
//! @file
//!
//! \brief
//! Path class implementation

#include "../include/Path.hpp"


namespace nsvg {
	Path::Path(const cstyle::PathStruct* ptr)
		: internal { ptr }
	{ }

	std::vector<CubicPointSet> Path::getPointsSets() const {
		std::vector<CubicPointSet> result;

		for(auto i = 0; i < this->internal->npts - 1; i += 3) {
			float* p = &this->internal->pts[i * 2];

			sf::Vector2f begin = { p[0], p[1] };
			sf::Vector2f ctrl1 = { p[2], p[3] };
			sf::Vector2f ctrl2 = { p[4], p[5] };
			sf::Vector2f end   = { p[6], p[7] };

			result.push_back({begin, end, ctrl1, ctrl2});
		}

		return result;
	}

	Path Path::getNextPath() const{
		return Path(this->internal->next);
	}

	bool Path::good() const  {
		return internal;
	}

	Path& Path::operator ++() {
		return (*this = this->getNextPath());
	}

	Path::operator bool() const {
		return this->good();
	}
}
