#include <Siv3D.hpp>
#include "Canvas.hpp"
#include "Coloring.hpp"
#include "ColorPalette.hpp"

void Main()
{
	const Font font(30);

	const Vec2 windowSize(1280, 960);

	Graphics::SetBackground(piconte::Coloring::Base);
	Window::Resize((int32)windowSize.x, (int32)windowSize.y);

	piconte::Canvas canvas(Rect((Window::Width() - 800) >> 1, 32 + 16, 800, 600));

	piconte::ColorPalette palette(Rect(Window::Width() - 200 - 16, 32 + 16, 200, 200));

	while (System::Update())
	{
		palette.Update();
		canvas.Update(palette.Pick());
		
	}
}