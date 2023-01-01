#include "symbol_table.h"
#include <iostream>

symbol_table_entry::symbol_table_entry(const var_id id, symbol_table* parent)
        : _id(id), 
          _parent(parent) {}

var_id symbol_table_entry::id() const {
    return _id;
}

bool symbol_table_entry::value() const {
    return _parent->value_of(_id);
}

identifier symbol_table_entry::name() const {
    return _parent->identifier_of(_id);
}


symbol_table::symbol_table() 
    : _id_by_identifier(
        std::unordered_map<identifier, var_id>(SYMBOL_TABLE_CAPACITY)
      ),
      _identifier_by_id(
        std::unordered_map<var_id, identifier>(SYMBOL_TABLE_CAPACITY)
      ),
      _value_by_id(0),
      _next_id(0) {}

symbol_table_entry 
symbol_table::entry_for_identifier(const identifier& identifier) {
    auto entry = _id_by_identifier.find(identifier);
    if (entry != _id_by_identifier.end()) {
        return symbol_table_entry(entry->second, this);
    }
    var_id id = _next_id++; // TODO: check capactity overflow
    _id_by_identifier.insert({ identifier, id });
    _identifier_by_id.insert({ id, identifier });
    return symbol_table_entry(id, this);
}

identifier symbol_table::identifier_of(const var_id& id) const {
    return _identifier_by_id.at(id);
}

bool symbol_table::value_of(const var_id& id) const {
    return _value_by_id & (1 << id);
}

void symbol_table::set_state(const var_values& vars) {
    _value_by_id = vars;
}

size_t symbol_table::size() const {
    return _next_id;
}

void symbol_table::debug() const {
    std::cout << "table::next_id: " << _next_id << std::endl;
    for (auto const& entry : _identifier_by_id) {
        auto id = entry.first;
        auto identifier = entry.second;
        std::cout 
            << "(id: " << id
            << ", name: " << identifier
            << ", value: " << value_of(id)
            << ")" << std::endl;
    }
};
