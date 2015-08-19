#pragma once

#include <Siv3D.hpp>

namespace piconte
{
	class Coloring
	{
	public:
		static const Color Base;
		static const Color Blue;
		static const Color Red;
	};

	const Color Coloring::Base = Color(42, 41, 41);
	const Color Coloring::Blue = Color(0, 87, 118);
	const Color Coloring::Red = Color(118, 20, 41);
}