#ifndef ERROR_H
#define ERROR_H

struct tag_string_info {};
struct tag_filename_info {};

namespace ErrorInfo 
{
	namespace tag {
		struct string_info {};
		struct filename_info {};
	};

	typedef boost::error_info<tag::string_info, std::string> String;
	typedef boost::error_info<tag::filename_info, std::string> FileName;
};

struct GameError : virtual boost::exception, virtual std::exception
{
};

#endif //ERROR_H
