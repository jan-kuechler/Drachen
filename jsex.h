#ifndef JSEX_H
#define JSEX_H

// Compiler workarround:
// MSVC needs a static before each spezialization, for GCC it
// has to be only before the unspezialized function.

#ifdef _MSC_VER
#define STATIC static
#else
#define STATIC
#endif

namespace jsex
{
	namespace js = json_spirit;

	template <typename T>
	static T get(const js::mValue& v);

	template <>
	STATIC bool get(const js::mValue& v)
	{
		return v.get_bool();
	}

	template <>
	STATIC int get(const js::mValue& v)
	{
		return v.get_int();
	}

	template <>
	STATIC float get(const js::mValue& v)
	{
		return static_cast<float>(v.get_real());
	}

	template <>
	STATIC size_t get(const js::mValue& v)
	{
		return v.get_int();
	}

	template <>
	STATIC std::string get(const js::mValue& v)
	{
		return v.get_str();
	}

	template <>
	STATIC js::mArray get(const js::mValue& v)
	{
		return v.get_array();
	}

	template <>
	STATIC js::mObject get(const js::mValue& v)
	{
		return v.get_obj();
	}

	template <>
	STATIC Vector2f get(const js::mValue& v)
	{
		const js::mArray& arr = v.get_array();
		return Vector2f(get<float>(arr[0]), get<float>(arr[1]));
	}

	template <>
	STATIC FloatRect get(const js::mValue& v)
	{
		const js::mObject& obj = v.get_obj();

		auto tl = get<Vector2f>(obj.at("top-left"));
		auto w = get<float>(obj.at("width"));
		auto h = get<float>(obj.at("height"));

		return FloatRect(tl.x, tl.y, tl.x + w, tl.y + h);
	}

	template <typename T>
	static T get_opt(const js::mObject& obj, const std::string& idx, const T& def)
	{
		if (obj.count(idx))
			return get<T>(obj.at(idx));
		return def;
	}

	template <typename T>
	static T get_opt(const js::mObject& obj, const std::string& idx)
	{
		if (obj.count(idx))
			return get<T>(obj.at(idx));
		return T();
	}

	template <typename T>
	static void get_opt(T& var, const js::mObject& obj, const std::string& idx)
	{
		if (obj.count(idx))
			var = get<T>(obj.at(idx));
	}

	template <typename T>
	static std::vector<T> read_vector(const js::mArray& arr)
	{
		std::vector<T> vec;
		vec.reserve(arr.size());
		for (size_t i=0; i < arr.size(); ++i) {
			vec.push_back(get<T>(arr[i]));
		}
		return vec;
	}

	template <typename T>
	static std::set<T> read_set(const js::mArray& arr)
	{
		std::set<T> set;
		for (size_t i=0; i < arr.size(); ++i) {
			set.insert(get<T>(arr[i]));
		}
		return set;
	}

	template <typename T>
	static js::mArray write_set(const std::set<T>& set)
	{
		js::mArray arr;
		for (const auto& item: set) {
			arr.push_back(js::mValue(item));
		}
		return arr;
	}

}

#undef STATIC

#endif //JSEX_H
