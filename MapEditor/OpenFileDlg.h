#ifndef OPEN_FILE_DLG
#define OPEN_FILE_DLG
#import <Cocoa/Cocoa.h>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <iostream>

boost::optional<boost::filesystem::path> FileOpenDlg();
boost::optional<boost::filesystem::path> FileSaveDlg();

@interface FileDlg : NSObject <NSApplicationDelegate>
+(char*) openFileDlg;
+(char*) saveFileDlg;



@end

#endif
