#ifndef EXTENDED_DRAWABLES_H
#define EXTENDED_DRAWABLES_H

namespace sfex
{
	template <typename Base>
	class ExtendedDrawable : public Base
	{
	protected:
		bool visible;

	public:
		ExtendedDrawable()
		: visible(true)
		{ }

		void Show()
		{
			visible = true;
		}

		void Hide()
		{
			visible = false;
		}

		void SetVisible(bool vis)
		{
			visible = vis;
		}

		bool GetVisible() const
		{
			return visible;
		}

	protected:
		void Render(sf::RenderTarget& target) const
		{
			if (visible)
				Base::Render(target);
		}
	};

	typedef ExtendedDrawable<sf::String> String;
};

#endif //EXTENDED_DRAWABLES_H
