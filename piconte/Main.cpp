#include <Siv3D.hpp>
#include "Canvas.hpp"
#include "Coloring.hpp"

void Main()
{
	const Font font(30);

	const Vec2 windowSize(1280, 960);

	Graphics::SetBackground(piconte::Coloring::Base);
	Window::Resize((int32)windowSize.x, (int32)windowSize.y);

	piconte::Canvas canvas(Rect((Window::Width() - 800) >> 1, 32 + 16, 800, 600));

	while (System::Update())
	{
		canvas.Update(Palette::Black);
	}
}