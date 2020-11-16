/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! @file
//!
//! \brief
//! Some debug files implementation

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <SFML/System/Vector2.hpp>
#include <iostream>

template<typename T>
std::ostream& operator<<(std::ostream& out, const sf::Vector2<T>& vec) {
	out << "[" << vec.x << ", " << vec.y << "]";
	return out;
}

#endif // DEBUG_HPP
