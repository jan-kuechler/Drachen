#ifndef JSEX_H
#define JSEX_H

namespace jsex
{
	namespace js = json_spirit;

	template <typename T>
	T get(const js::mValue& v);

	template <>
	bool get(const js::mValue& v)
	{
		return v.get_bool();
	}

	template <>
	int get(const js::mValue& v)
	{
		return v.get_int();
	}

	template <>
	float get(const js::mValue& v)
	{
		return static_cast<float>(v.get_real());
	}

	template <>
	size_t get(const js::mValue& v)
	{
		return v.get_int();
	}

	template <>
	Vector2f get(const js::mValue& v)
	{
		const js::mArray& arr = v.get_array();
		return Vector2f(get<float>(arr[0]), get<float>(arr[1]));
	}

	template <>
	FloatRect get(const js::mValue& v)
	{
		const js::mObject& obj = v.get_obj();

		auto tl = get<Vector2f>(obj.at("top-left"));
		auto w = get<float>(obj.at("width"));
		auto h = get<float>(obj.at("height"));

		return FloatRect(tl.x, tl.y, tl.x + w, tl.y + h);
	}

	template <typename T>
	std::vector<T> read_vector(const js::mArray& arr)
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
