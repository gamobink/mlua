#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class  Closure;
class String;
class Function;
struct Value
{
// 	union  {
// 		double num_;
// 		bool bvalue_;
// 		String* str_;
// 		Closure *closure_;
// 		Function* fun_;
// 	};

	enum ValueT
	{
// 		ValueT_Nil,
// 		ValueT_Bool,
// 		ValueT_Number,
// 		ValueT_Obj,
// 		ValueT_String,
// 		ValueT_Closure,
// 		ValueT_Upvalue,
// 		ValueT_Table,
// 		ValueT_UserData,
// 		ValueT_CFunction,

		TYPE_NIL,
		TYPE_BOOL,
		TYPE_NUMBER,
		TYPE_STRING,
		TYPE_TABLE,
		TYPE_FUNCTION,
		TYPE_CLOSURE,
		TYPE_NATIVE_FUNCTION,
	};

	virtual int Type() const = 0;
	virtual std::string Name() const = 0;
	virtual bool IsEqual(const Value *other) const = 0;
	virtual std::size_t GetHash() const = 0;


	struct ValueHasher : public std::unary_function<Value *, std::size_t>  {
		std::size_t operator() (const Value *value) const  {
			return value->GetHash();
		}
	};

	struct ValueEqualer : public std::binary_function<Value *, Value *, bool>  {
		bool operator() (const Value *left, const Value *right) const  {
			return left->IsEqual(right);
		}
	};

	ValueT type_;
};


class Nil : public Value
{
public:
	Nil(){}
	virtual std::string Name() const { return "nil"; }
	virtual int Type() const { return TYPE_NIL; }
	virtual bool IsEqual(const Value *other) const;
	virtual std::size_t GetHash() const { return 0; }
};



class String : public Value
{
public:
	explicit String(const std::string& v) : _value(v) {}

	virtual std::string Name() const {	return "string";}
	virtual int Type() const { return TYPE_STRING; }
	virtual bool IsEqual(const Value *other) const;
	std::string Get() { return _value; }
	const std::string& Get() const  { return _value; }

	std::size_t GetHash() const  { return std::hash<std::string>()(_value); }

private:
	std::string _value;
};


class Number : public Value
{
public:
	explicit Number(double v) : _value(v) {}

	virtual std::string Name() const { return "number"; }
	virtual int Type() const { return TYPE_NUMBER; }
	virtual bool IsEqual(const Value *other) const;
	const double Get() const  { return _value; }

	std::size_t GetHash() const  { return std::hash<double>()(_value); }

	int GetInteger() const  { return static_cast<int>(floor(_value)); }
	bool IsInteger() const { return floor(_value) == _value; }

private:
	double _value;
};








class TableValue : public Value
{
public:
	explicit TableValue(Value *value) : _value(value){}


	virtual int Type() const { return _value->Type(); }
	virtual bool IsEqual(const Value *other) const { return _value->IsEqual(other); };
	virtual std::string Name() const { return _value->Name(); }

	virtual std::size_t GetHash() const  { return _value->GetHash(); }

// 	virtual bool IsEqual(const Value *other) const
// 	{
// 		return value_->IsEqual(other);
// 	}

	Value * GetValue()  { return _value; }
	void SetValue(Value *value)  { _value = value; }

private:
	Value *_value;
};

class Table : public Value
{
public:
	Table();
	virtual std::string Name() const { return "table"; }
	virtual int Type() const { return TYPE_TABLE; }
	bool IsEqual(const Value *other) const  { return this == other; }
	std::size_t GetHash() const  { return std::hash<const Table*>()(this); }

	std::size_t GetArraySize() const;
	bool HaveKey(const Value *key) const;

	Value * GetValue(const Value *key);
	TableValue * GetTableValue(const Value *key);

	void ArrayAssign(std::size_t array_index, Value *value);
	void ArrayAssign(std::size_t array_index, TableValue *table_value);
	void Assign(const Value *key, Value *value);
	void Assign(const Value *key, TableValue *table_value);

private:
	typedef std::vector<TableValue *> ArrayType;
	typedef std::unordered_map<const Value *, TableValue *, ValueHasher, ValueEqualer> HashTableType;

	bool HashTableHasKey(const Value *key) const;
	bool ArrayHasKey(const Value *key) const;
	void MarkArray();
	void MarkHashTable();

	ArrayType* _array;
	HashTableType* _hash_table;
};
