#pragma once
#include <Siv3D.hpp>
#include <SivPentablet.hpp>

namespace piconte
{
	class Canvas
	{
		Rect bounds;
		Image image;
		DynamicTexture canvas;

		Point prevPos;

		void DrawCanvas(const Color& color)
		{
			auto pos = Mouse::Pos() - bounds.pos;
			pos.x = Clamp(pos.x, 0, bounds.w - 1);
			pos.y = Clamp(pos.y, 0, bounds.h - 1);

			Line(prevPos, pos).overwrite(image, color);
			canvas.fill(image);
		}

	public:
		Canvas(const Rect& bounds) : 
			image(bounds.w, bounds.h, Palette::White),
			bounds(bounds) 
		{
			canvas = DynamicTexture(image);
		}

		void Update(const Color& color)
		{
			if (Input::MouseL.pressed && bounds.mouseOver)
			{
				DrawCanvas(color);
			}
			canvas.draw(bounds.pos);
			prevPos = Mouse::Pos() - bounds.pos;
		}
	};
}