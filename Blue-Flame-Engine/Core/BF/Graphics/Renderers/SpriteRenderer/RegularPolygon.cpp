#include "RegularPolygon.h"

namespace BF
{
	namespace Graphics
	{
		namespace Renderers
		{
			using namespace BF::Math;

			RegularPolygon::RegularPolygon()
			{
			}

			RegularPolygon::RegularPolygon(const Rectangle& rectangle, unsigned int zLayer, const Color& color) :
				Renderable(Vector2((float)rectangle.x, (float)rectangle.y), rectangle, zLayer, color, Type::RegularPolygon)
			{
			}

			RegularPolygon::~RegularPolygon()
			{
			}
		}
	}
}