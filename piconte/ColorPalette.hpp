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

		const double hdiv;
		const double wdiv;
		const double huediv;

		void RedrawPalette()
		{
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

		void DrawSelector()
		{
			const double x = rect.x + pick.v * rect.w;
			const double y = rect.y + pick.s * rect.h;
			Circle(x, y, 6).drawArc(0, 360, 1);
			Circle(x, y, 5).drawArc(0, 360, 1, 0, Palette::Black);

			const double h = hueRect.x + pick.h * (rect.w / 360.0);
			Line(h, hueRect.y, h, hueRect.y + 16).draw(Palette::Black);
		}

	public:
		ColorPalette(const Rect& paletteSize)
			: 
			pimage(paletteSize.w, paletteSize.h), 
			himage(paletteSize.w, 16),
			pick(0, 0, 0), 
			rect(paletteSize),
			hueRect(paletteSize.x, paletteSize.y + paletteSize.h + 16, paletteSize.w, 16),
			hdiv(1.0 / (double)rect.h),
			wdiv(1.0 / (double)rect.w),
			huediv(360.0 / (double)rect.w)
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
			DrawSelector();
		}

		Color Pick() const
		{
			return pick.toColor();
		}
	};
}