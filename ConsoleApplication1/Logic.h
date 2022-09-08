#pragma once
#include <map>
#include <vector>
class LogicBase;
class Logic;

enum class LogicType : uint8_t
{
	None,
	Input = 1,
	Output = 2,
};
inline LogicType operator | (LogicType lhs, LogicType rhs)
{
	
	using T = std::underlying_type_t <LogicType>;
	return static_cast<LogicType>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline LogicType& operator |= (LogicType& lhs, LogicType rhs)
{
	lhs = lhs | rhs;
	return lhs;
}
inline LogicType operator & (LogicType lhs, LogicType rhs)
{

	using T = std::underlying_type_t <LogicType>;
	return static_cast<LogicType>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

inline LogicType& operator &= (LogicType& lhs, LogicType rhs)
{
	lhs = lhs & rhs;
	return lhs;
}
typedef void(LogicBase::* ExampleFn)(bool hot);

struct LogicKlass
{
	typedef void (LogicBase::*OnUpdate)();
	typedef void (LogicBase::*OnSinal)(bool value);

	LogicKlass();
	LogicKlass(OnUpdate onupdate, OnSinal onsinal, LogicType type);
	OnUpdate onUpdate;
	OnSinal onSinal;
	LogicType logicType;

};
class LogicBase
{
public:
	bool IsInput();
	bool IsOnlyInput();
	bool isOutput();
	bool IsOnlyOutput();
	bool FirstInput();
	virtual LogicKlass GetKlass();
	std::vector<int> output;
	std::vector<int> input;
	int Restant;
	bool Value;
	Logic* logicParent;
};
class And: public LogicBase
{
public:
	void OnUpdate();
	void OnSinal(bool hot);
	LogicKlass GetKlass();
private:
	static LogicKlass klass;

};

class Or : public LogicBase
{
public:
	void OnUpdate();
	void OnSinal(bool hot);
	LogicKlass GetKlass();
private:
	static LogicKlass klass;
};
class Input : public LogicBase
{
public:
	void OnSinal(bool hot);
	void OnUpdate();
	LogicKlass GetKlass();
private:
	static LogicKlass klass;
};

class Output : public LogicBase
{
public:
	void OnSinal(bool hot);
	void OnUpdate();
	LogicKlass GetKlass();
private:
	static LogicKlass klass;
};

class NAnd : public LogicBase
{
public:
	void OnUpdate();
	void OnSinal(bool hot);
	LogicKlass GetKlass();
private:
	static LogicKlass klass;

};

class Xor : public LogicBase
{
public:
	void OnUpdate();
	void OnSinal(bool hot);
	LogicKlass GetKlass();
private:
	static LogicKlass klass;

};

class Not : public LogicBase
{
public:
	void OnUpdate();
	void OnSinal(bool hot);
	LogicKlass GetKlass();
private:
	static LogicKlass klass;

};
class NOr : public LogicBase
{
public:
	void OnUpdate();
	void OnSinal(bool hot);
	LogicKlass GetKlass();
private:
	static LogicKlass klass;

};

class XNOr : public LogicBase
{
public:
	void OnUpdate();
	void OnSinal(bool hot);
	LogicKlass GetKlass();
private:
	static LogicKlass klass;

};

class Logic
{
public:
	int AddAnd();
	int AddNAnd();

	int AddOr();
	int AddXor();

	int AddInput(bool v);
	int AddOutput();

	int AddNot();
	int AddNor();
	int AddXnor();

	void AddES(int inputid, int outputid);
	LogicBase* Get(int id);
	void Run();
private:
	int currentid;
	std::map<int, LogicBase*> logics;
};

