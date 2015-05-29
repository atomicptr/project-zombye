#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <scriptstdstring/scriptstdstring.h>

#include <zombye/assets/asset.hpp>
#include <zombye/assets/asset_manager.hpp>
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

		register_function("void print(const string& in)", +[](const std::string& in) {log(in);});
		register_glm();

		begin_module("MyModule");
		load_script("scripts/test.as");
		end_module();
		exec("void main()", "MyModule");
	}

	void scripting_system::begin_module(const std::string& module_name) {
		auto result = script_builder_->StartNewModule(script_engine_.get(), module_name.c_str());
		if (result < 0) {
			throw std::runtime_error("Could not create new module " + module_name);
		}
	}

	void scripting_system::load_script(const std::string& file_name) {
		auto asset = game_.asset_manager().load(file_name);
		if (!asset) {
			throw std::runtime_error("Could not load script " + file_name);
		}
		auto content = asset->content();
		auto result = script_builder_->AddSectionFromMemory(file_name.c_str(), content.data(), content.size(), 0);
		if (result < 0) {
			throw std::runtime_error("Could not add section from memory " + file_name);
		}
	}

	void scripting_system::end_module() {
		auto result = script_builder_->BuildModule();
		if (result < 0) {
			throw std::runtime_error("Could not build module");
		}
	}

	void scripting_system::exec(const std::string& function_decl, const std::string& module_name) {
		auto mod = script_engine_->GetModule(module_name.c_str());
		if (!mod) {
			throw std::runtime_error("No module named " + module_name);
		}
		auto func = mod->GetFunctionByDecl(function_decl.c_str());
		if (!func) {
			throw std::runtime_error("No function with signature " + function_decl + " in module " + module_name);
		}

		script_context_->Prepare(func);
		auto result = script_context_->Execute();
		if (result != asEXECUTION_FINISHED) {
			if (result == asEXECUTION_EXCEPTION) {
				throw std::runtime_error(script_context_->GetExceptionString());
			}
		}
	}

	void scripting_system::register_destructor(const std::string& type_name, void(*function)(void*)) {
		auto result = script_engine_->RegisterObjectBehaviour(type_name.c_str(), asBEHAVE_DESTRUCT,
			"void f()", asFUNCTION(function), asCALL_CDECL_OBJFIRST);
		if (result < 0) {
			throw std::runtime_error("Could not register destructor at type " + type_name);
		}
	}

	void scripting_system::register_member(const std::string& type_name, const std::string& member_decl, size_t offset) {
		auto result = script_engine_->RegisterObjectProperty(type_name.c_str(), member_decl.c_str(), offset);
		if (result < 0 ) {
			throw std::runtime_error("Could not register member " + member_decl + " at type " + type_name);
		}
	}

	void scripting_system::register_glm() {
		script_engine_->SetDefaultNamespace("glm");

		register_type<glm::vec3>("vec3");

		register_constructor("vec3", "void f()",
			+[](void* memory) { *reinterpret_cast<glm::vec3*>(memory) = glm::vec3{}; });
		register_constructor("vec3", "void f(float a, float b, float c)",
			+[](void* memory, float a, float b, float c) { *reinterpret_cast<glm::vec3*>(memory) = glm::vec3(a, b, c); });

		register_destructor("vec3", +[](void* memory) {});

		register_member("vec3", "float x", asOFFSET(glm::vec3, x));
		register_member("vec3", "float y", asOFFSET(glm::vec3, y));
		register_member("vec3", "float z", asOFFSET(glm::vec3, z));

		register_member_function("vec3", "vec3& opAssign(const vec3& in)",
			+[](glm::vec3& lhs, const glm::vec3& rhs) -> glm::vec3& { return lhs = rhs; });
		register_member_function("vec3", "vec3& opAddAssign(const vec3& in)",
			+[](glm::vec3& lhs, const glm::vec3& rhs) -> glm::vec3& { return lhs += rhs; });
		register_member_function("vec3", "vec3& opSubAssign(const vec3& in)",
			+[](glm::vec3& lhs, const glm::vec3& rhs) -> glm::vec3& { return lhs -= rhs; });
		register_member_function("vec3", "vec3& opMulAssign(float)",
			+[](glm::vec3& lhs, float rhs) -> glm::vec3& { return lhs *= rhs; });
		register_member_function("vec3", "vec3& opDivAssign(float)",
			+[](glm::vec3& lhs, float rhs) -> glm::vec3& { return lhs /= rhs; });
		register_member_function("vec3", "vec3 opAdd(const vec3& in)",
			+[](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs + rhs; });
		register_member_function("vec3", "vec3 opSub(const vec3& in)",
			+[](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs - rhs; });
		register_member_function("vec3", "vec3 opMul(float)",
			+[](float lhs, const glm::vec3& rhs) { return lhs * rhs; },
			asCALL_CDECL_OBJLAST);
		register_member_function("vec3", "vec3 opMul_r(float)",
			+[](const glm::vec3& lhs, float rhs) { return lhs * rhs; });
		register_member_function("vec3", "vec3 opDiv(float)",
			+[](const glm::vec3& lhs, float rhs) { return lhs / rhs; });

		register_function("float length(const vec3& in)",
			+[](const glm::vec3& v) {return glm::length(v);});
		register_function("float dot(const vec3& in, const vec3& in)",
			+[](const glm::vec3& v1, const glm::vec3& v2) {return glm::dot(v1, v2);});
		register_function("vec3 cross(const vec3& in, const vec3& in)",
			+[](const glm::vec3& v1, const glm::vec3& v2) {return glm::cross(v1, v2);});
		register_function("vec3 normalize(const vec3& in)",
			+[](const glm::vec3& v) {return glm::normalize(v);});

		script_engine_->SetDefaultNamespace("");
		register_function("void print(const glm::vec3& in)", +[](const glm::vec3& in) {log(glm::to_string(in));});
	}
}
