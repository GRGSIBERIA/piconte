#pragma once
// 以下のサンプルより借用
// http://play-siv3d.hateblo.jp/entry/sample/GUIColorPalette

#include <Siv3D.hpp>

namespace piconte
{
	class GUIColorPalette : public IWidget
	{
	private:

		static const int PaletteHeight = 240;
		static const int SVPaletteWidth = 160;
		static const int PaletteMarginWidth = 15;
		static const int HuePaletteWidth = 20;

		Rect getSVBox(const Point& offset) const
		{
			return{ getItemBox(offset).pos + getAlignOffset(), SVPaletteWidth, PaletteHeight };
		}

		Rect getHueBox(const Point& offset) const
		{
			return{ getItemBox(offset).pos + getAlignOffset() + Point(SVPaletteWidth + PaletteMarginWidth, 0), HuePaletteWidth, PaletteHeight };
		}

		Size getPaletteRegion() const
		{
			return Size(SVPaletteWidth + PaletteMarginWidth + HuePaletteWidth, PaletteHeight);
		}

		Size getItemSize() const
		{
			Size region = getPaletteRegion();
			region.x = m_style.width.value_or(region.x);
			return region;
		}

		Rect getMarginBox(const Point& offset) const
		{
			const Size item = getItemSize();
			Rect rect;
			rect.x = 0;
			rect.y = 0;
			rect.w = m_style.margin.left + Max(m_style.padding.left, 0) + item.x + Max(m_style.padding.right, 0) + m_style.margin.right;
			rect.h = m_style.margin.top + Max(m_style.padding.top, 0) + item.y + Max(m_style.padding.bottom, 0) + m_style.margin.bottom;
			return rect.moveBy(offset);
		}

		Rect getPaddingBox(const Point& offset) const
		{
			const Size item = getItemSize();
			Rect rect;
			rect.x = m_style.margin.left;
			rect.y = m_style.margin.top;
			rect.w = Max(m_style.padding.left, 0) + item.x + Max(m_style.padding.right, 0);
			rect.h = Max(m_style.padding.top, 0) + item.y + Max(m_style.padding.bottom, 0);
			return rect.moveBy(offset);
		}

		Rect getItemBox(const Point& offset) const
		{
			const Size item = getItemSize();
			Rect rect;
			rect.x = m_style.margin.left + Max(m_style.padding.left, 0);
			rect.y = m_style.margin.top + Max(m_style.padding.top, 0);
			rect.w = item.x;
			rect.h = item.y;
			return rect.moveBy(offset);
		}

		Point getAlignOffset() const
		{
			const Size itemBoxSize = getItemBox(Point::Zero).size;
			const Size paletteRegion = getPaletteRegion();
			const Align align = m_style.align.value_or(Align::Left);
			Point alignOffset = Point::Zero;

			if (align == Align::Center)
			{
				alignOffset.x = (itemBoxSize.x - paletteRegion.x) / 2;
			}
			else if (align == Align::Right)
			{
				alignOffset.x = itemBoxSize.x - paletteRegion.x;
			}

			return alignOffset;
		}

		void updateSVPalette()
		{
			Image image(m_textureSV.size);

			for (auto p : step(image.size))
			{
				image[p.y][p.x] = HSV(getHue(), p.x / double(image.width), 1.0 - (p.y / double(image.height)));
			}

			m_textureSV.fill(image);
		}

		void updateHuePalette()
		{
			Image image(m_textureHue.size);

			for (auto p : step(image.size))
			{
				image[p.y][p.x] = HSV(p.y / double(image.height) * 360.0);
			}

			m_textureHue.fill(image);
		}

		double getHue() const
		{
			return m_posHue / double(PaletteHeight) * 360;
		}

		double getS() const
		{
			return m_posSV.x / double(SVPaletteWidth);
		}

		double getV() const
		{
			return 1.0 - m_posSV.y / double(PaletteHeight);
		}

		DynamicTexture m_textureSV, m_textureHue;

		Point m_posSV = Point::Zero;

		int m_posHue = 0;

		bool m_enabled = true;

		bool m_svPressed = false;

		bool m_huePressed = false;

	public:

		static const String& Name()
		{
			static const String name = L"ColorPalette";
			return name;
		}

		static std::shared_ptr<IWidget> Create(const HSV& color, bool enabled = true, const WidgetStyle& style = WidgetStyle())
		{
			return std::make_shared<GUIColorPalette>(color, enabled, style);
		}

		GUIColorPalette() = default;

		GUIColorPalette(const HSV& color, bool enabled, const WidgetStyle& style)
			: IWidget(style)
			, m_enabled(enabled)
			, m_textureSV(SVPaletteWidth, PaletteHeight)
			, m_textureHue(HuePaletteWidth, PaletteHeight)
		{
			updateSVPalette();
			updateHuePalette();
			setHSV(color);
		}

		const String& getWidgetName() const
		{
			return Name();
		}

		Size getSize() const override
		{
			return getMarginBox(Point::Zero).size;
		}

		bool forceNewLineBefore() const override { return false; }

		bool forceNewLineAfter() const override { return false; }

		void update(const WidgetState& state) override
		{
			const Rect SVBox = getSVBox(state.currentPos);
			const Rect HueBox = getHueBox(state.currentPos);

			if (!m_enabled || Input::MouseL.released)
			{
				m_svPressed = m_huePressed = false;
			}

			if (!m_enabled)
			{
				return;
			}

			if (SVBox.leftClicked)
			{
				m_svPressed = true;
			}
			else if (HueBox.stretched(0, 0, 0, 10).leftClicked)
			{
				m_huePressed = true;
			}

			if (m_svPressed)
			{
				m_posSV.x = Clamp(Mouse::Pos().x - SVBox.x, 0, SVBox.w);
				m_posSV.y = Clamp(Mouse::Pos().y - SVBox.y, 0, SVBox.h);
			}
			else if (m_huePressed)
			{
				const int posHue = Clamp(Mouse::Pos().y - HueBox.y, 0, HueBox.h);

				if (posHue != m_posHue)
				{
					m_posHue = posHue;
					updateSVPalette();
				}
			}
		}

		void draw(const WidgetState& state) const override
		{
			const Rect SVBox = getSVBox(state.currentPos);
			const Rect HueBox = getHueBox(state.currentPos);

			m_textureSV.draw(SVBox.pos).drawFrame(0, 1, Color(100));
			m_textureHue.draw(HueBox.pos).drawFrame(0, 1, Color(100));

			if (!m_enabled)
			{
				SVBox.draw(Color(255, 160));
				HueBox.draw(Color(255, 160));
			}

			Circle(SVBox.pos + m_posSV, 6).drawFrame(0.5, 0.5, Color(getHSV().toColor().grayscaleF > 0.7 ? 60 : 255));
			Triangle(HueBox.x - 7, HueBox.y + m_posHue, 10, HalfPi).draw(Color(m_enabled ? 80 : 160));

			if (state.showBoxModel)
			{
				getMarginBox(state.currentPos).drawFrame(1, 0, Color(255, 0, 0, 255));
				getPaddingBox(state.currentPos).drawFrame(1, 0, Color(0, 127, 0, 255));
				getItemBox(state.currentPos).drawFrame(1, 0, Color(0, 0, 127, 255));
			}
		}

		HSV getHSV() const
		{
			return{ getHue(), getS(), getV() };
		}

		void setHSV(const HSV& hsv)
		{
			m_posHue = static_cast<int>(Clamp(hsv.h, 0.0, 360.0) / 360.0 * PaletteHeight);
			m_posSV.x = static_cast<int>(Clamp(hsv.s, 0.0, 1.0) * SVPaletteWidth);
			m_posSV.y = static_cast<int>((1.0 - Clamp(hsv.v, 0.0, 1.0)) * PaletteHeight);
			updateSVPalette();
		}

		bool isPressed() const
		{
			return m_svPressed || m_huePressed;
		}

		bool& getEnabled()
		{
			return m_enabled;
		}
	};

	class GUIColorPaletteWrapper
	{
	public:

		using WidgetType = GUIColorPalette;

	private:

		friend class GUI;

		std::shared_ptr<WidgetType> m_widget;

		GUIColorPaletteWrapper(const std::shared_ptr<WidgetType>& widget)
			: m_widget(widget ? widget : std::make_shared<WidgetType>())
			, enabled(m_widget->getEnabled())
			, style(m_widget->m_style) {}

		GUIColorPaletteWrapper& operator = (const GUIColorPaletteWrapper&) = delete;

		static const String& WidgetTypeName()
		{
			return WidgetType::Name();
		}

	public:

		bool& enabled;
		WidgetStyle& style;

		void set(const Color& color) { m_widget->setHSV(color); }
		void set(const ColorF& color) { m_widget->setHSV(color); }
		void set(const HSV& color) { m_widget->setHSV(color); }

		Property_Get(HSV, hsv) { return m_widget->getHSV(); }
		Property_Get(Color, color) { return hsv; }
		Property_Get(ColorF, colorF) { return hsv; }

		Property_Get(bool, pressed) { return m_widget->isPressed(); }
	};

	//void Main()
	//{
	//	GUI gui(GUIStyle::Default);
	//	gui.setTitle(L"パレット");
	//	gui.addln(L"palette", GUIColorPalette::Create(Palette::Orange));
	//	gui.addln(L"color", GUIText::Create(L"(255,255,255)"));
	//	gui.setPos(400, 20);

	//	while (System::Update())
	//	{
	//		const Color color = gui.get<GUIColorPaletteWrapper>(L"palette").color;

	//		gui.text(L"color").text = Format(PyFmt, L"({},{},{})", color.r, color.g, color.b);

	//		Graphics::SetBackground(color);

	//		if (Input::MouseR.clicked)
	//		{
	//			gui.get<GUIColorPaletteWrapper>(L"palette").enabled = !gui.get<GUIColorPaletteWrapper>(L"palette").enabled;
	//		}
	//	}
	//}
}