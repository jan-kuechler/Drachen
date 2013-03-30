#ifndef JSEX_H
#define JSEX_H

namespace jsex
{
	namespace js = json_spirit;

	template <typename T>
	static T get(const js::mValue& v);

	template <>
	static bool get(const js::mValue& v)
	{
		return v.get_bool();
	}

	template <>
	static int get(const js::mValue& v)
	{
		return v.get_int();
	}

	template <>
	static float get(const js::mValue& v)
	{
		return static_cast<float>(v.get_real());
	}

	template <>
	static size_t get(const js::mValue& v)
	{
		return v.get_int();
	}

	template <>
	static Vector2f get(const js::mValue& v)
	{
		const js::mArray& arr = v.get_array();
		return Vector2f(get<float>(arr[0]), get<float>(arr[1]));
	}

	template <>
	static FloatRect get(const js::mValue& v)
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
	static std::vector<T> read_vector(const js::mArray& arr)
	{
		std::vector<T> vec;
		vec.reserve(arr.size());
		for (size_t i=0; i < arr.size(); ++i) {
			vec.push_back(get<T>(arr[i]));
		}
		return vec;
	}

}

#endif //JSEX_H
