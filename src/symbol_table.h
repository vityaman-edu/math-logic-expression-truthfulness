#ifndef EXPRESSION_TRUTHFULNESS_SYMBOL_TABLE_H
#define EXPRESSION_TRUTHFULNESS_SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>

typedef std::string identifier;
typedef size_t var_id;
typedef uint64_t var_values;
const size_t SYMBOL_TABLE_CAPACITY = 64;

class symbol_table;

class symbol_table_entry {
public:
    symbol_table_entry(const var_id id, symbol_table* parent);
    var_id id() const;
    bool value() const;
    identifier name() const;
private:
    const var_id _id;
    symbol_table* _parent;
};

class symbol_table {
public:
    symbol_table();
    symbol_table_entry entry_for_identifier(const identifier& identifier);
    identifier identifier_of(const var_id& id) const;
    bool value_of(const var_id& id) const;
    void set_state(const var_values& vars);
    size_t size() const;
    void debug() const;

private:
    std::unordered_map<identifier, var_id> _id_by_identifier;
    std::unordered_map<var_id, identifier> _identifier_by_id;
    var_values _value_by_id;
    var_id _next_id;
};

#endif // EXPRESSION_TRUTHFULNESS_SYMBOL_TABLE_H
