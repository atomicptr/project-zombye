#include <scriptstdstring/scriptstdstring.h>

#include <zombye/core/game.hpp>
#include <zombye/scripting/scripting_system.hpp>
#include <zombye/utils/logger.hpp>

namespace zombye {
	scripting_system::scripting_system(game& game)
	: game_{game}, script_engine_{nullptr, +[](asIScriptEngine*){}}, script_context_{nullptr, +[](asIScriptContext*){}} {
		script_engine_ = std::unique_ptr<asIScriptEngine, void(*)(asIScriptEngine*)>(
			asCreateScriptEngine(ANGELSCRIPT_VERSION),
			+[](asIScriptEngine* se) { se->ShutDownAndRelease(); }
		);

		auto result = script_engine_->SetMessageCallback(
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
		if (result < 0) {
			log(LOG_ERROR, "Could not register message callback for script system");
		}

		script_context_ = std::unique_ptr<asIScriptContext, void(*)(asIScriptContext*)>{
			script_engine_->CreateContext(),
			+[](asIScriptContext* context) {context->Release();}
		};

		RegisterStdString(script_engine_.get());

		script_builder_ = std::make_unique<CScriptBuilder>();
	}
}
