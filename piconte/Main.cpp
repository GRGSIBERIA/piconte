#include <Siv3D.hpp>
#include "Canvas.hpp"

void Main()
{
	const Font font(30);

	Window::Resize(1024, 768);

	piconte::Canvas canvas(Rect(Window::Width() - 800, 0, 800, 600));

	while (System::Update())
	{
		canvas.Update(Palette::Black);
	}
}
