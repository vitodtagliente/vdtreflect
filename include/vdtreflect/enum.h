#include <map>
#include <regex>
#include <string>

namespace reflect
{
	template <typename T>
	struct EnumReflectBase
	{
		static const std::map<std::string, int>& translate(const std::string& str)
		{
			static std::map<std::string, int> s_translation;
			if (s_translation.empty() == false)
			{
				return s_translation;
			}

			// Should be called once per each enumeration.
			std::string senumsInfo(str);
			std::regex re("^([a-zA-Z_][a-zA-Z0-9_]+) *=? *([^,]*)(,|$) *");     // C++ identifier to optional " = <value>"
			std::smatch sm;
			int value = 0;

			for (; regex_search(senumsInfo, sm, re); senumsInfo = sm.suffix(), value++)
			{
				std::string name = sm[1].str();
				std::string enumValue = sm[2].str();

				if (enumValue.length() != 0)
					value = atoi(enumValue.c_str());

				s_translation.insert(std::make_pair(name, value));
			}

			return s_translation;
		}
	};

	template <typename T>
	struct EnumReflect
	{
		static const char* describe() { return ""; }
	};

#define REFLECT_ENUM(T, ...) \
    enum class T : int { __VA_ARGS__ }; \
    template <> \
    struct EnumReflect<T> : public EnumReflectBase<T> \
    { \
        static const char* describe() { return #__VA_ARGS__; } \
    };

	template <class T>
	std::string enumToString(const T t)
	{
		const int value = static_cast<int>(t);
		for (const auto& pair : EnumReflect<T>::translate(EnumReflect<T>::describe()))
		{
			if (pair.second == value)
			{
				return pair.first;
			}
		}
		return "";
	}

	template <class T>
	bool stringToEnum(const char* name, T& t)
	{
		for (const auto& pair : EnumReflect<T>::translate(EnumReflect<T>::describe()))
		{
			if (pair.first == name)
			{
				return t = static_cast<T>(pair.second), true;
			}
		}
		return false;
	}

	template <class T>
	bool stringToEnum(const std::string& name, T& t)
	{
		for (const auto& pair : EnumReflect<T>::translate(EnumReflect<T>::describe()))
		{
			if (pair.first == name)
			{
				return t = static_cast<T>(pair.second), true;
			}
		}
		return false;
	}
}