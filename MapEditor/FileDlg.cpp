#include "pch.h"
#include <CommDlg.h>

namespace fs = boost::filesystem;

boost::optional<fs::path> FileOpenDlg()
{
	OPENFILENAMEA ofn;
	memset(&ofn, 0, sizeof(ofn));

	char file[1024];

	ofn.lStructSize = sizeof (ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = file;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(file);
	ofn.lpstrFilter = "All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	if (GetOpenFileNameA(&ofn)) {
		return fs::path(ofn.lpstrFile);
	}
	return boost::optional<fs::path>();
}

boost::optional<fs::path> FileSaveDlg()
{
	OPENFILENAMEA ofn;
	memset(&ofn, 0, sizeof(ofn));

	char file[1024];

	ofn.lStructSize = sizeof (ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = file;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(file);
	ofn.lpstrFilter = "All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	if (GetSaveFileNameA(&ofn)) {
		return fs::path(ofn.lpstrFile);
	}
	return boost::optional<fs::path>();
}

