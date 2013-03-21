#ifndef UI_HELPER_H
#define UI_HELPER_H

static inline void InitButton(Button& btn, std::string prefix, int idx = -1)
{
	btn.SetImage(gImageManager.getResource(gTheme.GetFileName(prefix + "/image", idx)));
	btn.SetPosition(gTheme.GetPosition(prefix + "/position", idx));
}

static inline void InitText(String& txt, std::string prefix, int idx = -1)
{
	txt.SetFont(gTheme.GetMainFont());
	txt.SetPosition(gTheme.GetPosition(prefix + "/position", idx));
	txt.SetSize(gTheme.GetFloat(prefix + "/font-size", idx));
}

static inline void CenterText(String& txt)
{
	auto pos = txt.GetPosition();
	auto bbox = txt.GetRect();
	txt.SetPosition(pos - Vector2f(bbox.GetWidth() / 2.f, bbox.GetWidth() / 2.f));
}

static inline void InitImage(Sprite& img, std::string prefix, int idx = -1)
{
	img.SetPosition(gTheme.GetPosition(prefix + "/position", idx));
	img.SetImage(gImageManager.getResource(gTheme.GetFileName(prefix + "/image", idx)));
}

#endif //UI_HELPER_H
