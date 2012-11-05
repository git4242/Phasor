#pragma  once

#include <map>
#include <vector>
#include <list>

typedef unsigned long DWORD;

// Forward declare relevant classes
namespace Lua
{
	class State;
}

namespace Common
{
	class Object;
}

typedef Lua::State ScriptState;	

// Namespace: Manager
// Provides an interface for managing scripts of different types.
// Currently only Lua is supported, but others can be integrated easily.
namespace Manager
{		
	class Result;			

	// --------------------------------------------------------------------

	// Opens the script specified, relative to the scripts directory
	// May throw an exception <todo: add specific info>
	ScriptState* OpenScript(const char* file);

	// Closes the specified script
	void CloseScript(ScriptState* state);

	// --------------------------------------------------------------------
	// Class: Result
	// Provides an interface for retrieving values from scripts
	class Result
	{
	protected:
		std::vector<Common::Object*> result;

		void SetData(const Result& other);
		void Free();

		// Constructs the result and takes ownership of the memory.
		Result(const std::vector<Common::Object*>& result);

	public:
		Result();
		Result(const Result& other);
		Result& operator=(const Result& rhs);
		~Result();

		size_t size() const;

		const Common::Object* operator [] (size_t index);

		friend class Caller;
	};

	// --------------------------------------------------------------------
	// Class: Caller
	// Provides an interface for passing parameters to scripts.
	class Caller
	{
	protected:
		std::list<Common::Object*> args;

		void SetData(const Caller& other);			

	public:
		Caller();
		Caller(const Caller& other);
		~Caller();
		Caller& operator=(const Caller& rhs);

		// Adds an argument to the list, which is passed to the next function called
		void AddArg(bool b);
		void AddArg(const char* str);
		void AddArg(int value);
		void AddArg(DWORD value);
		void AddArg(float value);
		void AddArg(double value);	
		void AddArg(const std::map<std::string, std::string>& table);	

		// Clears the current argument list
		void Clear();

		Result Call(ScriptState* state, const char* function, bool* found, int timeout);
		Result Call(ScriptState* state, const char* function, int timeout);
	};
}
