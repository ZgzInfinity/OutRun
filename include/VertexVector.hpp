/* =========================================================== *
 * sf-svg (c) Kamil Koczurek | koczurekk@gmail.com          *
 * GNU GPL v3 License http://www.gnu.org/licenses/gpl-3.0.html *
 * =========================================================== */

//! @file
//!
//! \brief
//! VertexVector (drawable std::vector<sf::Vertex>) implementation

#ifndef VERTEXVECTOR
#define VERTEXVECTOR

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>

namespace sfc {
	//! \brief Drawable std::vector<sf::Vertex>
	//!
	//! \tparam TPrimitive Way to draw array.
	//! <table>
	//! <tr>
	//! <th> Value </th>
	//! <th> Meaning </th>
	//! </tr>
	//! <tr>
	//! <td>Points</td>
	//! <td>List of individual points</td>
	//! </tr>
	//! <tr>
	//! <td>Lines</td>
	//! <td>List of individual lines</td>
	//! </tr>
	//! <tr>
	//! <td>LineStrip</td>
	//! <td>List of connected lines, a point uses the previous point to form a line</td>
	//! </tr>
	//! <tr>
	//! <td>Triangles</td>
	//! <td>List of individual triangles</td>
	//! </tr>
	//! <tr>
	//! <td>TriangleStrip</td>
	//! <td>List of connected triangles, a point uses the two previous points to form a triangle</td>
	//! </tr>
	//! <tr>
	//! <td>TriangleFan</td>
	//! <td>List of connected triangles, a point uses the common center and the previous point to form a triangle</td>
	//! </tr>
	//! <tr>
	//! <td>Quads</td>
	//! <td>List of individual quads (deprecated, don't work with OpenGL ES)</td>
	//! </tr>
	//! </table>
	//!
	//! Access all features of std::vector and draw it directly to sf::RenderWindow.
	template<sf::PrimitiveType TPrimitive>
	struct VertexVector
	: public std::vector<sf::Vertex>,
	  public sf::Drawable {
		using std::vector<sf::Vertex>::vector;

	private:
		virtual void draw(sf::RenderTarget& mRenderTarget, sf::RenderStates mRenderStates) const {
			mRenderTarget.draw(&this->operator [](0), this->size(), TPrimitive, mRenderStates);
		}
	};
}
#endif
