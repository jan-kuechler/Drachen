#ifndef ERROR_H
#define ERROR_H

struct tag_string_info {};
struct tag_filename_info {};

namespace ErrorInfo 
{
	namespace tag {
		struct desc {};
		struct note {};
		struct string {};
		struct loading {};
	};

	typedef boost::error_info<tag::desc, std::string> Desc;
	typedef boost::error_info<tag::note, std::string> Note;
	typedef boost::error_info<tag::string, std::string> String;
	typedef boost::error_info<tag::loading, bool> Loading;
};

struct GameError : virtual boost::exception, virtual std::exception
{
};

#endif //ERROR_H
