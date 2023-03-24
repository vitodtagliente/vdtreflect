/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace reflect
{
	typedef std::map<std::string, int> enum_values_t;

	template <typename T>
	struct Enum
	{
		static const char* name() { return ""; }
		static const enum_values_t& values() {
			static enum_values_t s_values;
			return s_values;
		}
	};

	class EnumFactory final
	{
	public:
		EnumFactory() = delete;

		template <typename T>
		friend struct RegisteredInEnumFactory;

		static std::string enumToString(const std::string& type, const int value)
		{
			for (const auto& pair : definition(type))
			{
				if (pair.second == value)
				{
					return pair.first;
				}
			}
			return "";
		}

		static bool stringToEnum(const std::string& type, const std::string& name, int& value)
		{
			const enum_values_t& def = definition(type);
			const auto& it = def.find(name);
			if (it != def.end())
			{
				return value = it->second, true;
			}
			return false;
		}

	private:
		static std::map<std::string, enum_values_t>& collection()
		{
			static std::map<std::string, enum_values_t> s_getters;
			return s_getters;
		}

		static const enum_values_t& definition(const std::string& name)
		{
			static enum_values_t s_empty_definition;

			const auto& it = collection().find(name);
			if (it != collection().end())
			{
				return it->second;
			}
			return s_empty_definition;
		}

		static bool insert(const std::string& name, const enum_values_t& values)
		{
			return collection().insert(std::make_pair(name, values)), true;
		}
	};

	template <typename T>
	struct RegisteredInEnumFactory
	{
		static bool value;
	};

	template <typename T>
	bool RegisteredInEnumFactory<T>::value{ EnumFactory::insert(Enum<T>::name(), Enum<T>::values()) };

	template <class T>
	std::string enumToString(const T t)
	{
		const int value = static_cast<int>(t);
		for (const auto& pair : Enum<T>::values())
		{
			if (pair.second == value)
			{
				return pair.first;
			}
		}
		return "";
	}

	template <class T>
	bool stringToEnum(const std::string& name, T& t)
	{
		const enum_values_t& definition = Enum<T>::values();
		const auto& it = definition.find(name);
		if (it != definition.end())
		{
			return t = static_cast<T>(it->second), true;
		}
		return false;
	}

	typedef std::map<std::string, std::string> meta_t;
	typedef unsigned long long member_address_t;

	struct PropertyType
	{
		enum class Type
		{
			T_unknown,

			T_bool,
			T_char,
			T_double,
			T_enum,
			T_float,
			T_int,
			T_native,
			T_string,
			T_template,
			T_type,
			T_void
		};

		enum class DecoratorType
		{
			D_raw,
			D_pointer,
			D_reference
		};

		std::string name;
		std::vector<PropertyType> children;
		DecoratorType decorator;
		std::size_t size;
		Type type;
	};

	struct Property
	{
		std::size_t offset;
		const meta_t meta;
		const std::string name;
		const PropertyType type;

		template<typename T, typename O>
		T& value(O* const object) const
		{
			return *reinterpret_cast<T*>(reinterpret_cast<member_address_t>(object) + offset);
		}
	};
	typedef std::map<std::string, Property> properties_t;

	struct IType
	{
		IType() = default;
		virtual ~IType() = default;

		virtual const char* const type_name() const = 0;
		virtual const meta_t& type_meta() const = 0;
		virtual const properties_t& type_properties() const = 0;

		virtual operator std::string() const = 0;
		virtual void from_string(const std::string& str) = 0;
		std::string to_string() const { return static_cast<std::string>(*this); }
		virtual void from_json(const std::string& json) = 0;
		virtual std::string to_json(const std::string& offset = "") const = 0;
	};

	template <typename T>
	struct Type
	{
		Type() = delete;

		static const meta_t& meta()
		{
			static meta_t s_meta;
			return s_meta;
		}
		static const char* const name() { return ""; }
		static const properties_t& properties()
		{
			static properties_t s_properties;
			return s_properties;
		}
		static std::size_t size() { return sizeof(T); }

		static void from_string(const std::string&, T&) {}
		static std::string to_string(const T&) { return ""; }
		static void from_json(const std::string&, T&) {}
		static std::string to_json(const T&, const std::string&) { return ""; }
	};

	typedef std::function<IType* ()> constructor_t;

	class TypeFactory final
	{
	public:
		TypeFactory() = delete;

		template <typename T>
		friend struct RegisteredInTypeFactory;

		static IType* const instantiate(const std::string& name)
		{
			const auto& dictionary = collection();
			const auto& it = dictionary.find(name);
			if (it != dictionary.end())
			{
				const constructor_t& constructor = std::get<1>(it->second);
				IType* const type = constructor();
				return type;
			}
			return nullptr;
		}

		template <typename T = IType>
		static T* const instantiate()
		{
			return reinterpret_cast<T*>(instantiate(Type<T>::name()));
		}

		template <typename T = IType>
		static T* const instantiate(const std::string& name)
		{
			return reinterpret_cast<T*>(instantiate(name));
		}

		static const std::map<std::string, std::tuple<meta_t, constructor_t>>& list()
		{
			return collection();
		}

		static std::map<std::string, std::tuple<meta_t, constructor_t>> list(const std::string& metaOption)
		{
			return list(metaOption, "");
		}

		static std::map<std::string, std::tuple<meta_t, constructor_t>> list(const std::string& metaOption, const std::string& metaValue)
		{
			std::map<std::string, std::tuple<meta_t, constructor_t>> result;
			for (const auto& [name, tuple] : collection())
			{
				const meta_t& meta = std::get<0>(tuple);
				const auto& it = meta.find(metaOption);
				if (it != meta.end() && (metaValue.empty() || it->second == metaValue))
				{
					result.insert(std::make_pair(name, tuple));
				}
			}
			return result;
		}

	private:
		static std::map<std::string, std::tuple<meta_t, constructor_t>>& collection()
		{
			static std::map<std::string, std::tuple<meta_t, constructor_t>> s_getters;
			return s_getters;
		}

		static const std::tuple<meta_t, constructor_t>& definition(const std::string& name)
		{
			static std::tuple<meta_t, constructor_t> s_empty_definition;

			const auto& it = collection().find(name);
			if (it != collection().end())
			{
				return it->second;
			}
			return s_empty_definition;
		}

		static bool insert(const std::string& name, const meta_t& meta, const constructor_t& constructor)
		{
			return collection().insert(std::make_pair(name, std::make_tuple(meta, constructor))), true;
		}
	};

	template <typename T>
	struct RegisteredInTypeFactory
	{
		static bool type_registered;
	};

	template <typename T>
	bool RegisteredInTypeFactory<T>::type_registered{ TypeFactory::insert(Type<T>::name(), Type<T>::meta(), []() -> IType* { return new T(); }) };

	namespace encoding
	{
		typedef std::vector<std::byte> ByteBuffer;

		struct ByteBufferFile
		{
			static bool load(ByteBuffer& buffer, const char* const path)
			{
				buffer.clear();
				buffer.resize(0);
				buffer.shrink_to_fit();

				std::ifstream file(path, std::ios::in | std::ios_base::binary);
				if (!file.is_open() || file.bad())
					return false;

				// Clear whitespace removal flag
				file.unsetf(std::ios::skipws);

				// Determine size of the file
				file.seekg(0, std::ios_base::end);
				const std::size_t file_size = file.tellg();
				file.seekg(0, std::ios_base::beg);

				buffer.resize(file_size);
				file.read(reinterpret_cast<char*>(&buffer[0]), file_size);

				return buffer.size() == file_size;
			}

			static void save(const ByteBuffer& buffer, const char* const path)
			{
				if (buffer.empty()) return;

				std::ofstream file(path, std::ios::out | std::ios_base::binary);
				file.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size());
				file.flush();
				file.close();
			}
		};

		class ByteStream
		{
		public:
			ByteStream(ByteBuffer& buffer)
				: m_buffer{ buffer }
			{}

			inline const ByteBuffer& getBuffer() const { return m_buffer; }
			inline const std::byte* getNativeBuffer() const { return m_buffer.data(); }

		protected:
			// buffer of bytes
			ByteBuffer& m_buffer;
		};

		// The OutputByteBuffer wraps the ByteBuffer.
		// It has methods for writing sequentially into the buffer.

		class OutputByteStream : public ByteStream
		{
		public:
			OutputByteStream(ByteBuffer& buffer)
				: ByteStream(buffer)
			{}

			template<typename T>
			OutputByteStream& operator<< (const T data)
			{
				static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
					"Generic write only supports primitive data type");

				const std::byte* begin = reinterpret_cast<const std::byte*>(std::addressof(data));
				const std::byte* end = begin + sizeof(T);

				m_buffer.insert(m_buffer.end(), begin, end);

				return *this;
			}

			OutputByteStream& operator<< (const std::string& data)
			{
				*this << data.size();

				std::transform(
					std::begin(data),
					std::end(data),
					std::back_inserter(m_buffer),
					[](const char c)
					{
						return std::byte(c);
					}
				);

				return *this;
			}

			OutputByteStream& operator<< (const char* str)
			{
				const std::size_t size = std::strlen(str);
				*this << size;

				const std::byte* begin = reinterpret_cast<const std::byte*>(str);
				const std::byte* end = begin + size;

				m_buffer.insert(m_buffer.end(), begin, end);

				return *this;
			}
		};

		// The InputByteStream has methods for reading sequentially from the buffer. 

		class InputByteStream : public ByteStream
		{
		public:
			InputByteStream(ByteBuffer& buffer)
				: ByteStream(buffer)
				, m_index{ 0 }
			{}

			template<typename T>
			InputByteStream& operator>> (T& data)
			{
				static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
					"Generic read only supports primitive data type");

				const std::size_t amount = std::min<std::size_t>(sizeof(T), getSize());
				if (amount > 0)
				{
					std::memcpy(&data, &m_buffer[m_index], amount);
					m_index += amount;
				}

				return *this;
			}

			InputByteStream& operator>> (std::string& data)
			{
				std::size_t characters{ 0 };
				*this >> characters;

				const std::size_t amount = std::min<std::size_t>(characters, getSize());
				if (amount > 0)
				{
					data.reserve(amount);
					std::memcpy(&data[0], &m_buffer[m_index], amount);
					m_index += amount;
				}

				return *this;
			}

			inline std::size_t getSize() const { return m_buffer.size() - m_index; }

		private:
			// reading byte index
			std::size_t m_index;
		};

		namespace json
		{
			struct Serializer final
			{
				Serializer() = delete;
				~Serializer() = delete;

				template <typename T>
				static std::string to_string(const T value)
				{
					if constexpr (std::is_base_of<IType, T>::value)
					{
						return value.to_json();
					}
					else
					{
						return std::to_string(value);
					}
				}

				static std::string to_string(const bool value)
				{
					return value ? "true" : "false";
				}

				static std::string to_string(const char* const value)
				{
					return std::string{ quote }.append(value).append(quote);
				}

				static std::string to_string(const std::string& value)
				{
					return std::string{ quote }.append(value).append(quote);
				}

				template <typename T = IType>
				static std::string to_string(const std::shared_ptr<T>& value)
				{
					if (value == nullptr)
					{
						return "null";
					}
					return value->to_json();
				}

				template <typename T = IType>
				static std::string to_string(const std::unique_ptr<T>& value)
				{
					if (value == nullptr)
					{
						return "null";
					}
					return value->to_json();
				}

				template <typename T>
				static std::string to_string(const std::list<T>& value)
				{
					std::string result{ "[" };

					for (const auto& element : value)
					{
						result.append(to_string(element)).append(comma);
					}

					result.append("]");
					return result;
				}

				template <typename T>
				static std::string to_string(const std::vector<T>& value)
				{
					std::string result{ "[" };

					for (const auto& element : value)
					{
						result.append(to_string(element)).append(comma);
					}

					result.append("]");
					return result;
				}

				template <typename K, typename V>
				static std::string to_string(const std::map<K, V>& object)
				{

					std::string next{};

					std::string result{ "{" };

					for (const auto& pair : object)
					{
						result.append(next)
							.append(quote)
							.append(to_string(pair.first))
							.append(quote_equals)
							.append(to_string(pair.second));
						next = comma;
					}

					return result.append("}");
				}

				static constexpr char* comma = ",";
				static constexpr char* equals = ":";
				static constexpr char* quote = "\"";
				static constexpr char* quote_equals = "\":";
			};

			struct Deserializer final
			{
				Deserializer() = delete;
				~Deserializer() = delete;

				template <typename T>
				static void parse(const std::string& source, T& value)
				{
					if constexpr (std::is_base_of<IType, T>::value)
					{
						value.from_json(source);
					}
					else
					{
						value = static_cast<T>(std::stod(source));
					}
				}

				static void parse(const std::string& source, bool& value)
				{
					static const std::string pattern{ "true" };

					value = to_lower(source).find(pattern) != std::string::npos;
				}

				static void parse(const std::string& source, std::string& value)
				{
					value = trim(trim(source, space), quote);
				}

				template <typename T = IType>
				static void parse(const std::string& source, std::shared_ptr<T>& value)
				{
					if (value == nullptr)
					{
						value = std::make_shared<T>();
					}
					value->from_json(source);
				}

				template <typename T = IType>
				static void parse(const std::string& source, std::unique_ptr<T>& value)
				{
					if (value == nullptr)
					{
						value = std::make_unique<T>();
					}
					value->from_json(source);
				}

				template <typename T>
				static void parse(const std::string& source, std::list<T>& list)
				{
					std::string src{ trim(source, space) };
					src = src.substr(1, src.length() - 2);

					size_t index = 0;
					std::string element_source;
					while (!src.empty() && (index = next_value(src, element_source)) != std::string::npos)
					{
						T element;
						parse(element_source, element);
						list.push_back(std::move(element));
						const size_t minIndex = src.length() < index + 1 ? src.length() : index + 1;
						src = src.substr(minIndex);
					};
				}

				template <typename T>
				static void parse(const std::string& source, std::vector<T>& list)
				{
					std::string src{ trim(source, space) };
					src = src.substr(1, src.length() - 2);

					size_t index = 0;
					std::string element_source;
					while (!src.empty() && (index = next_value(src, element_source)) != std::string::npos)
					{
						T element;
						parse(element_source, element);
						list.push_back(std::move(element));
						const size_t minIndex = src.length() < index + 1 ? src.length() : index + 1;
						src = src.substr(minIndex);
					};
				}

				template <typename K, typename V>
				static void parse(const std::string& source, std::map<K, V>& obj)
				{
					std::string src{ trim(source, space) };

					size_t index = 0;
					std::string key_source, value_source;
					while ((index = next_key(src, key_source)) != std::string::npos)
					{
						src = src.substr(index + 2);
						index = next_value(src, value_source);
						if (index != std::string::npos)
						{
							K key;
							parse(key_source, key);
							V value;
							parse(value_source, value);
							obj.insert({ std::move(key), std::move(value) });
							src = src.substr(index + 1);
						}
						else
						{
							break;
						}
					};
				}

				static bool is_array(const std::string& source)
				{
					const std::string src = trim(source, space);
					if (src.empty()) return false;
					return src.at(0) == lsquareb && src.at(src.length() - 1) == rsquareb;
				}

				static bool is_bool(const std::string& source)
				{
					const std::string src = to_lower(trim(source, space));
					return src == "true"
						|| src == "false";
				}

				static bool is_null(const std::string& source)
				{
					const std::string src = to_lower(trim(source, space));
					return src == "null";
				}

				static bool is_number(const std::string& source)
				{
					const std::string src = trim(source, space);
					return !src.empty() && src.find_first_not_of("-.0123456789") == std::string::npos;
				}

				static bool is_object(const std::string& source)
				{
					const std::string src = trim(source, space);
					if (src.empty()) return false;
					return src.at(0) == lgraphb && src.at(src.length() - 1) == rgraphb;
				}

				static bool is_string(const std::string& source)
				{
					const std::string src = trim(source, space);
					if (src.empty()) return false;
					return src.at(0) == quote && src.at(src.length() - 1) == quote;
				}

				static std::string to_lower(const std::string& str)
				{
					std::string result;
					std::transform(str.begin(), str.end(), std::back_inserter(result),
						[](const char c) -> char { return static_cast<char>(std::tolower(c)); }
					);
					return result;
				}

				static std::string ltrim(const std::string& str, const char character)
				{
					std::string result(str);
					auto it2 = std::find_if(
						result.begin(),
						result.end(),
						[character](char ch)
						{ return ch != character; }
					);
					result.erase(result.begin(), it2);
					return result;
				}

				static std::string rtrim(const std::string& str, const char character)
				{
					std::string result(str);
					auto it1 = std::find_if(
						result.rbegin(),
						result.rend(),
						[character](char ch)
						{ return ch != character; }
					);
					result.erase(it1.base(), result.end());
					return result;
				}

				static std::string trim(const std::string& str, const char character)
				{
					return ltrim(rtrim(str, character), character);
				}

				static size_t next_closure(const std::string& text, const char left, const char right)
				{
					size_t n = 0;
					for (size_t i = 1; i < text.length(); ++i)
					{
						const char c = text.at(i);
						if (c == left)
						{
							++n;
						}
						else if (c == right)
						{
							if (n == 0)
							{
								return (i + 1);
							}
							--n;
						}
					}

					return std::string::npos;
				}

				static size_t until_next(const std::string& text, const std::vector<char>& characters)
				{
					for (size_t i = 0; i < text.length(); ++i)
					{
						if (std::find(characters.begin(), characters.end(), text.at(i)) != characters.end())
						{
							return i;
						}
					}
					return text.length();
				}

				static size_t until_not(const std::string& text, const std::vector<char>& characters)
				{
					for (size_t i = 0; i < text.length(); ++i)
					{
						if (std::find(characters.begin(), characters.end(), text.at(i)) == characters.end())
						{
							return i;
						}
					}
					return text.length();
				}

				static size_t next_key(const std::string& text, std::string& key)
				{
					key.clear();
					const size_t end = text.find(quote_equals);
					if (end != std::string::npos)
					{
						const size_t begin = text.find_last_of(quote, end - 1);
						if (begin != std::string::npos)
						{
							key = text.substr(begin + 1, end - 1 - begin);
						}
					}
					return end;
				}

				static size_t next_value(const std::string& text, std::string& value)
				{
					value.clear();

					if (text.empty())
					{
						return 0;
					}

					size_t pos = 0;
					if (text.at(0) == lgraphb)
					{
						pos = next_closure(text, lgraphb, rgraphb);
					}
					else if (text.at(0) == lsquareb)
					{
						pos = next_closure(text, lsquareb, rsquareb);
					}
					else
					{
						pos = until_next(text, { comma, rsquareb, rgraphb });
					}

					value = text.substr(0, pos);
					return pos;
				}

				static constexpr char comma = ',';
				static constexpr char endline = '\n';
				static constexpr char equals = ':';
				static constexpr char lsquareb = '[';
				static constexpr char lgraphb = '{';
				static constexpr char rsquareb = ']';
				static constexpr char rgraphb = '}';
				static constexpr char space = ' ';
				static constexpr char quote = '"';
				static constexpr char* quote_equals = "\":";
				static constexpr char tab = '\t';
			};
		}
	}

#define ENUM(...)
#define CLASS(...)
#define NATIVE_CLASS(T, ...)
#define PROPERTY(...)
#define FUNCTION(...)
#define GENERATED_BODY() \
	template <typename T> \
	friend struct Type; \
	virtual const meta_t& type_meta() const override; \
	virtual const char* const type_name() const override; \
	virtual const properties_t& type_properties() const override; \
	virtual operator std::string() const override; \
	virtual void from_string(const std::string& str) override; \
	virtual void from_json(const std::string& json) override; \
	virtual std::string to_json(const std::string& offset = "") const override;
}