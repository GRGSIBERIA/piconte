#pragma once
#include <Siv3D.hpp>

namespace piconte
{
	class ColorPalette
	{
		Rect rect;
		Rect hueRect;

		DynamicTexture palette;
		Image pimage;

		DynamicTexture hue;
		Image himage;

		HSV pick;

		void RedrawPalette()
		{
			const double hdiv = 1.0 / (double)rect.h;
			const double wdiv = 1.0 / (double)rect.w;

#pragma omp parallel for
			for (int i = 0; i < rect.h; ++i)
			{
				for (int j = 0; j < rect.w; ++j)
				{
					pimage[i][j] = HSV(pick.h, hdiv * i, wdiv * j).toColor();
				}
			}
			palette.fill(pimage);
		}

		void InitializeHue()
		{
			const double huediv = 360.0 / (double)rect.w;
			for (int i = 0; i < rect.w; ++i)
			{
				Line(i, 0, i, 16).overwrite(himage, HSV(huediv * i, 1.0, 1.0).toColor());
			}
			hue.fill(himage);
		}

		Vec2 Relative(const Rect& r) const
		{
			return Mouse::Pos() - Vec2(r.x, r.y);
		}

		void TestMouseClick()
		{
			const double huediv = 360.0 / (double)rect.w;
			const double hdiv = 1.0 / (double)rect.h;
			const double wdiv = 1.0 / (double)rect.w;

			if (rect.mouseOver && Input::MouseL.pressed)
			{
				auto pos = Relative(rect);
				pick.s = hdiv * pos.y;
				pick.v = wdiv * pos.x;
			}
			if (hueRect.mouseOver && Input::MouseL.pressed)
			{
				auto pos = Relative(hueRect);
				pick.h = huediv * pos.x;
				RedrawPalette();
			}
		}

	public:
		ColorPalette(const Rect& paletteSize)
			: 
			pimage(paletteSize.w, paletteSize.h), 
			himage(paletteSize.w, 16),
			pick(0, 0, 0), 
			rect(paletteSize),
			hueRect(paletteSize.x, paletteSize.y + paletteSize.h + 16, paletteSize.w, 16)
		{
			palette = DynamicTexture(pimage);
			hue = DynamicTexture(himage);
			RedrawPalette();
			InitializeHue();
		}

		void Update()
		{
			palette.draw(rect.x, rect.y);
			hue.draw(hueRect.x, hueRect.y);
			TestMouseClick();
		}

		const Color Pick() const
		{
			return pick.toColor();
		}
	};
}