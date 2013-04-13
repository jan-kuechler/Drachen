#include "pch.h"
#include "MapEditor.h"
#include "../ResourceManager.h"

ResourceManager<Image> gImageManager;

int main(int argc, char **argv)
{
	RenderWindow window(VideoMode(800, 600), "Drachen - MapEditor");
	window.SetFramerateLimit(100);

	MapEditor editor(window);

	while (window.IsOpened()) {
		editor.Run();
	}

	return 0;
}
