#import "OpenFileDlg.h"
@implementation FileDlg


+(bool) openFileDlg:(char*)  name
{
  	 // Create the File Open Dialog class.
	NSOpenPanel* openDlg = [NSOpenPanel openPanel];

    // Enable the selection of files in the dialog.
	[openDlg setCanChooseFiles:YES];

    // Multiple files not allowed
	[openDlg setAllowsMultipleSelection:NO];

    // Can't select a directory
	[openDlg setCanChooseDirectories:NO];
	NSString* url;
    // Display the dialog. If the OK button was pressed,
    // process the files.
	bool ok = false;
	if ( [openDlg runModal] == NSOKButton ) {
		ok = true;
        // Get an array containing the full filenames of all
        // files and directories selected.
		NSArray* urls = [openDlg URLs];

        // Loop through all the files and process them.
		for(int i = 0; i < [urls count]; i++ ) {
			url = [urls objectAtIndex:i];
			NSLog(@"Url: %@", url);
		}
	}
	char ret[512];
	[url getCString: ret maxLength:512 encoding:NSUTF8StringEncoding];
	name = ret;
	return ok;
}

+(bool) saveFileDlg:(char*) name
{
	NSSavePanel *save = [NSSavePanel savePanel];
	[save setAllowedFileTypes:[NSArray arrayWithObject:@"dat"]];
	[save setAllowsOtherFileTypes:NO];


	NSString* url;
	bool ok = false;
	if ([save runModal] == NSOKButton)
	{
		url = [[save URL] path];
		ok = true;
	}
	char ret[512];
	[url getCString: ret maxLength:512 encoding:NSUTF8StringEncoding];
	name = ret;
	return ok;
}


@end

namespace fs = boost::filesystem;

boost::optional<boost::filesystem::path> FileOpenDlg()
{
	FileDlg* filedlg;
	char name[512];
	if ([filedlg openFileDlg: name]) {
		return fs::path(name);
	}
	return boost::optional<fs::path>();
}

boost::optional<boost::filesystem::path> FileSaveDlg()
{
	FileDlg* filedlg;
	char name[512];
	if ([filedlg saveFileDlg: name]) {
		return fs::path(name);
	}
	return boost::optional<fs::path>();
}