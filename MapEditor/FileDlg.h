#ifndef FILE_DLG_H
#define FILE_DLG_H
#ifdef WIN32
boost::optional<boost::filesystem::path> FileOpenDlg();
boost::optional<boost::filesystem::path> FileSaveDlg();
#elif __APPLE__
//#include "OpenFileDlg.h"
boost::optional<boost::filesystem::path> FileOpenDlg();
boost::optional<boost::filesystem::path> FileSaveDlg();

#endif

#endif //FILE_DLG_H
