#ifndef __ZOMBYE_SCRIPTING_SYSTEM_HPP__
#define __ZOMBYE_SCRIPTING_SYSTEM_HPP__

#include <memory>

#include <angelscript.h>

namespace zombye {
	class game;
}

namespace zombye {
	class scripting_system {
	private:
		game& game_;
		std::unique_ptr<asIScriptEngine, void(*)(asIScriptEngine*)> script_engine_;

	public:
		scripting_system(game& game);
		~scripting_system() = default;

		scripting_system(const scripting_system& rhs) = delete;
		scripting_system& operator=(const scripting_system& rhs) = delete;

		scripting_system(scripting_system&& rhs) = delete;
		scripting_system& operator=(scripting_system&& rhs) = delete;
	};
}

#endif
