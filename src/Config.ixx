//
// Created by tom on 1/24/2026.
//

module;

#include <map>
#include <string>
#include <variant>

export module Mango.Config;

export import Mango.Core;

/*
 * Configs
 *
 * Why does get operate with a template and parameters?
 * Well, rule sections must be guaranteed at compile time, where keys are not, since keys can come from mod sources.
 *
 * Keys that ARE guaranteed at compile time follow the same syntax for least astonishment, but if no value exists in
 * the config, they will produce their default value. This allows systems to be able to expect SOME value.
 *
 */

namespace Mango {

	export template <typename ConfigType, FixedString... Sections> struct Config {
		using Key = std::pair<const char*, ConfigType>;

	private:
		using Section = std::map<std::string_view, ConfigType>;
		std::map<std::string_view, Section> config;

	public:
		template <FixedString S> const Section& get() const {										// Get a section
			static_assert(((S == Sections) || ...), "Config does not contain supplied section");
			return config.at(S);
		}

		// Get a concrete value.
		// Usage: auto v = config.get<SECTION>(key); type* v = std::get_if<type>(v);
		template <FixedString S> ConfigType get(const Key key) const {								// From default key
			static_assert(((S == Sections) || ...), "Config does not contain supplied section");
			if (config.at(S).contains(key.first)) return config.at(S).at(key.first);
			return key.second;
		}
		template <FixedString S> ConfigType get(const char* key) const {							// From runtime key
			static_assert(((S == Sections) || ...), "Config does not contain supplied section");
			return config.at(S).at(key);
		}
					// From default key

		// Get a concrete value (when using std::variant)
		// Since default keys will always provide a default, no if-init is needed.
		template <FixedString S, typename T> T get(const Key key) const {							// From default key
			static_assert(requires(ConfigType t) {
				{ t.valueless_by_exception() } -> std::convertible_to<bool>;
			}, "You only need to use Config's type-templated get if your Config's value type is a std::variant.");
			static_assert(((S == Sections) || ...), "Config does not contain supplied section");
			if (config.at(S).contains(key.first)) return std::get<T>(config.at(S).at(key.first));
			return std::get<T>(key.second);
		}
		// Usage: if (auto* v = config.get<SECTION, type>(key)) { /* do conditionally*/ }
		template <FixedString S, typename T> T* get(const char* key) const {						// From runtime key
			static_assert(requires(ConfigType t) {
				{ t.valueless_by_exception() } -> std::convertible_to<bool>;
			}, "You only need to use Config's type-templated get if your Config's value type is a std::variant.");
			static_assert(((S == Sections) || ...), "Config does not contain supplied section");
			return std::get_if<T>(config.at(S).at(key));
		}


		template <FixedString S> Config& set(const Key key, const ConfigType val) {					// From default key
			static_assert(((S == Sections) || ...), "Config does not contain supplied section");
			config.at(S).at(key.first) = val;
			return this;
		}
		template <FixedString S> Config& set(const char* key, const ConfigType val) {				// From runtime key
			static_assert(((S == Sections) || ...), "Config does not contain supplied section");
			config.at(S).at(key) = val;
			return this;
		}

		static Config Load() {
		    // wip
			auto c = Config();
			return c;
		}
	};
}

