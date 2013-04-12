#ifndef UI_HELPER_H
#define UI_HELPER_H

static inline void InitButton(Button& btn, std::string prefix, int idx = -1)
{
	namespace fs = boost::filesystem;

	if (gTheme.KeyExists(prefix + "/image", idx)) {
		auto fn = gTheme.GetFileName(prefix + "/image", idx);
		btn.SetImage(gImageManager.getResource(fn));

		if (fn.find("-normal") != fn.npos) {
			auto fnHighlight = boost::replace_last_copy(fn, "-normal", "-highlight");
			if (fs::exists(fnHighlight))
				btn.SetHighlightImage(gImageManager.getResource(fnHighlight));

			auto fnDown = boost::replace_last_copy(fn, "-normal", "-down");
			if (fs::exists(fnHighlight))
				btn.SetDownImage(gImageManager.getResource(fnDown));
		}
	}

	if (gTheme.KeyExists(prefix + "/position", idx))
		btn.SetPosition(gTheme.GetPosition(prefix + "/position", idx));
}

static inline void InitText(String& txt, std::string prefix, int idx = -1)
{
	txt.SetFont(gTheme.GetMainFont());
	if (gTheme.KeyExists(prefix + "/position", idx))
		txt.SetPosition(gTheme.GetPosition(prefix + "/position", idx));
	if (gTheme.KeyExists(prefix + "/font-size", idx))
		txt.SetSize(gTheme.GetFloat(prefix + "/font-size", idx));
	if (gTheme.KeyExists(prefix + "/color", idx))
		txt.SetColor(gTheme.GetColor(prefix + "/color", idx));
}

static inline void CenterText(String& txt)
{
	auto pos = txt.GetPosition();
	auto bbox = txt.GetRect();
	txt.SetPosition(pos - Vector2f(bbox.GetWidth() / 2.f, bbox.GetHeight() / 2.f));
}

static inline void InitImage(Sprite& img, std::string prefix, int idx = -1)
{
	if (gTheme.KeyExists(prefix + "/position", idx))
		img.SetPosition(gTheme.GetPosition(prefix + "/position", idx));
	if (gTheme.KeyExists(prefix + "/image", idx))
		img.SetImage(gImageManager.getResource(gTheme.GetFileName(prefix + "/image", idx)));
}

#endif //UI_HELPER_H
