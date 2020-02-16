#ifndef MU_VM_H
#define MU_VM_H

#include <stack>
#include <string>
#include <unordered_map>
#include <memory>
#include <cstdint>

#define is(x, type) (dynamic_cast<type*>(x) != nullptr)

class MuObject;

using MuObjectPtr = std::unique_ptr<MuObject>;
using FieldMap = std::unordered_map<std::string, MuObjectPtr>;

enum MuObjectType {
	MuTypeObject = 0,
	MuTypeNumber,
	MuTypeString,
	MuTypeBool
};

static std::string MuObjectTypeNames[] = {
	"object",
	"number",
	"string",
	"bool"
};

class MuObject {
public:
	MuObject() = default;
	virtual ~MuObject() = default;

	inline virtual std::string toString() { return "Object"; }
	inline virtual MuObjectType type() { return MuTypeObject; }

	inline uint64_t id() const { return m_id; }

	FieldMap& fields() { return m_fields; }

	MuObjectPtr& operator [](const std::string& str) { return m_fields[str]; }

private:
	FieldMap m_fields;
	uint64_t m_id{ ID++ };

	static uint64_t ID;
};

class MuNumber : public MuObject {
public:
	double value() const { return m_value; }
	void value(double value) { m_value = value; }

	inline std::string toString() override final { return std::to_string(m_value); }
	inline MuObjectType type() override final { return MuTypeNumber; }

private:
	double m_value;
};

class MuBool : public MuObject {
public:
	bool value() const { return m_value; }
	void value(bool value) { m_value = value; }

	inline std::string toString() override final { return std::to_string(m_value); }
	inline MuObjectType type() override final { return MuTypeBool; }

private:
	bool m_value;
};

class MuString : public MuObject {
public:
	const std::string& value() const { return m_value; }
	void value(const std::string& value) { m_value = value; }

	inline std::string toString() override final { return "\"" + m_value + "\""; }
	inline MuObjectType type() override final { return MuTypeString; }

private:
	std::string m_value;
};

class MuVM {
public:
	MuVM() = default;
	~MuVM() = default;

	enum class BinOp {
		Add = 0,
		Sub,
		Mul,
		Div,
		Mod,
		Shl,
		Shr,
		BitAnd,
		BitOr,
		BitXor,
		And,
		Or,
		Greater,
		Less,
		GreaterEq,
		LessEq,
		Equals,
		NotEquals
	};

	enum class UnOp {
		Positive = 0,
		Negative,
		Not
	};

	void pushNumber(double value);
	void pushBool(bool value);

	double popNumber();
	bool popBool();

	void binaryOp(BinOp op);
	void unaryOp(UnOp op);
	void logicNot();

private:
	std::unordered_map<int, MuObjectPtr> m_objects;
	std::stack<int> m_stack;
};

#endif // MU_VM_H
