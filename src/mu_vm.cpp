#include "mu_vm.h"

#include <iostream>

uint64_t MuObject::ID = 0;

void MuVM::addCommand(MuCommand::MuCommandType type, const Value& data) {
	MuCommand cmd;
	cmd.type = type;
	cmd.data = data;
	m_program.push_back(cmd);
}

void MuVM::run() {
	size_t pc = 0;
#define next() (m_program[pc++])

	while (pc < m_program.size()) {
		auto& cmd = next();
		switch (cmd.type) {
			default: break;
			case MuCommand::MuPushNumber: pushNumber(std::get<double>(cmd.data)); break;
			case MuCommand::MuPushBool: pushBool(std::get<bool>(cmd.data)); break;
			case MuCommand::MuBinaryOp: { const BinOp op = BinOp(std::get<int>(cmd.data)); binaryOp(op); } break;
			case MuCommand::MuUnaryOp: { const UnOp op = UnOp(std::get<int>(cmd.data)); unaryOp(op); } break;
			case MuCommand::MuJump: pc = std::get<int>(cmd.data); break;
		}
	}
}

void MuVM::pushNumber(double value) {
	MuNumber* num = new MuNumber();
	num->value(value);
	m_objects.insert({ num->id(), std::unique_ptr<MuObject>(num) });
	m_stack.push(num->id());
}

void MuVM::pushBool(bool value) {
	MuBool* b = new MuBool();
	b->value(value);
	m_objects.insert({ b->id(), std::unique_ptr<MuObject>(b) });
	m_stack.push(b->id());
}

void MuVM::binaryOp(BinOp op) {
	int a = m_stack.top(); m_stack.pop();
	int b = m_stack.top(); m_stack.pop();
	MuObject* na = m_objects[a].get();
	MuObject* nb = m_objects[b].get();
	if (na->type() == MuTypeNumber && nb->type() == MuTypeNumber) {
		double va = ((MuNumber*) na)->value();
		double vb = ((MuNumber*) nb)->value();
		switch (op) {
			case BinOp::Add: pushNumber(va + vb); break;
			case BinOp::Sub: pushNumber(va - vb); break;
			case BinOp::Mul: pushNumber(va * vb); break;
			case BinOp::Div: pushNumber(va / vb); break;
			case BinOp::Mod: pushNumber(int64_t(va) % int64_t(vb)); break;
			case BinOp::Shr: pushNumber(int64_t(va) >> int64_t(vb)); break;
			case BinOp::Shl: pushNumber(int64_t(va) << int64_t(vb)); break;
			case BinOp::BitAnd: pushNumber(int64_t(va) & int64_t(vb)); break;
			case BinOp::BitOr: pushNumber(int64_t(va) | int64_t(vb)); break;
			case BinOp::BitXor: pushNumber(int64_t(va) ^ int64_t(vb)); break;
			case BinOp::Greater: pushBool((va) > (vb)); break;
			case BinOp::Less: pushBool((va) < (vb)); break;
			case BinOp::GreaterEq: pushBool((va) >= (vb)); break;
			case BinOp::LessEq: pushBool((va) <= (vb)); break;
			case BinOp::Equals: pushBool((va) == (vb)); break;
			case BinOp::NotEquals: pushBool((va) != (vb)); break;
			default:
				std::cerr << "Cannot perform this binary operation on numbers." << std::endl;
			break;
		}
	} else if (na->type() == MuTypeBool && nb->type() == MuTypeBool) {
		bool va = ((MuBool*) na)->value();
		bool vb = ((MuBool*) nb)->value();
		switch (op) {
			case BinOp::Add: pushBool(va + vb); break;
			case BinOp::Sub: pushBool(va - vb); break;
			case BinOp::Mul: pushBool(va * vb); break;
			case BinOp::Div: pushBool(va / vb); break;
			case BinOp::BitAnd: pushBool(int64_t(va) & int64_t(vb)); break;
			case BinOp::BitOr: pushBool(int64_t(va) | int64_t(vb)); break;
			case BinOp::BitXor: pushBool(int64_t(va) ^ int64_t(vb)); break;
			case BinOp::Equals: pushBool((va) == (vb)); break;
			case BinOp::NotEquals: pushBool((va) != (vb)); break;
			case BinOp::And: pushBool((va) && (vb)); break;
			case BinOp::Or: pushBool((va) || (vb)); break;
			default:
				std::cerr << "Cannot perform this binary operation on bool values." << std::endl;
			break;
		}
	} else {
		// TODO: Error codes
		std::cerr << "Cannot perform binary operation on " << MuObjectTypeNames[na->type()] << " and " << MuObjectTypeNames[nb->type()] << "." << std::endl;
	}

	// Don't need these anymore
	m_objects.erase(a);
	m_objects.erase(b);
}

void MuVM::unaryOp(UnOp op) {
	int a = m_stack.top(); m_stack.pop();
	MuObject* na = m_objects[a].get();
	if (na->type() == MuTypeNumber) {
		double va = ((MuNumber*) na)->value();
		switch (op) {
			case UnOp::Negative: pushNumber(-va); break;
			case UnOp::Positive: pushNumber(va < 0.0 ? -va : va); break;
			case UnOp::Not: pushNumber(~int64_t(va)); break;
			default: break;
		}
	} else {
		// TODO: Error codes
		std::cerr << "Cannot perform unary operation on a " << MuObjectTypeNames[na->type()] << "." << std::endl;
	}
	m_objects.erase(a);
}

void MuVM::logicNot() {
	int a = m_stack.top(); m_stack.pop();
	MuObject* na = m_objects[a].get();
	if (na->type() == MuTypeBool) {
		bool va = ((MuBool*) na)->value();
		pushBool(!va);
	} else {
		// TODO: Error codes
		std::cerr << "Cannot perform this operation on a " << MuObjectTypeNames[na->type()] << "." << std::endl;
	}
	m_objects.erase(a);
}

double MuVM::popNumber() {
	int a = m_stack.top(); m_stack.pop();
	MuObject* na = m_objects[a].get();
	if (na->type() == MuTypeNumber) {
		return ((MuNumber*) na)->value();
	} else {
		// TODO: Error codes
		std::cerr << "Invalid type. Expected " << MuObjectTypeNames[MuTypeNumber] << ", got " << MuObjectTypeNames[na->type()] << "." << std::endl;
	}
	m_objects.erase(a);
	return 0;
}

bool MuVM::popBool() {
	int a = m_stack.top(); m_stack.pop();
	MuObject* na = m_objects[a].get();
	if (na->type() == MuTypeBool) {
		return ((MuBool*) na)->value();
	} else {
		// TODO: Error codes
		std::cerr << "Invalid type. Expected " << MuObjectTypeNames[MuTypeBool] << ", got " << MuObjectTypeNames[na->type()] << "." << std::endl;
	}
	m_objects.erase(a);
	return 0;
}