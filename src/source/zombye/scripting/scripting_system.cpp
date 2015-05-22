#include <zombye/core/game.hpp>
#include <zombye/scripting/scripting_system.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
	scripting_system::scripting_system(game& game)
	: game_{game}, script_engine_{nullptr, +[](asIScriptEngine*){}} {
		script_engine_ = std::unique_ptr<asIScriptEngine, void(*)(asIScriptEngine*)>(
			asCreateScriptEngine(ANGELSCRIPT_VERSION),
			+[](asIScriptEngine* se) { se->ShutDownAndRelease(); }
		);

		script_engine_->SetMessageCallback(
			asFUNCTION(+[](const asSMessageInfo* msg, void* param) {
				auto message = std::string{msg->message} + " in section " + msg->section + ":"
					+ std::to_string(msg->row) + ":" + std::to_string(msg->col);
				switch(msg->type) {
					case asEMsgType::asMSGTYPE_INFORMATION:
						log(LOG_INFO, message);
						break;
					case asEMsgType::asMSGTYPE_WARNING:
						log(LOG_WARNING, message);
						break;
					case asEMsgType::asMSGTYPE_ERROR:
						log(LOG_ERROR, message);
						break;
				}
			}),
			nullptr,
			asCALL_CDECL
		);
	}
}
