#ifndef EXPRESSION_TRUTHFULNESS_EXPRESSION_H
#define EXPRESSION_TRUTHFULNESS_EXPRESSION_H

#include <string>
#include <bitset>
#include <functional>
#include "symbol_table.h"

class expression {
public:
    virtual std::string as_string() = 0;
    virtual ~expression() {};
};

typedef std::bitset<SYMBOL_TABLE_CAPACITY> var_mask;

class cached_expression : public expression {
public:
    cached_expression(
        const var_mask& vars
    ) : _var_mask(vars),
        _cache_exists(false) {}

    bool calculate(const var_mask& state) {
        if (!was_updated(state) && _cache_exists) {
            return _cached_value;
        }
        _cached_state = state;
        _cache_exists = true;
        _cached_value = calculate_no_cache(state);
        return _cached_value;
    }

    virtual bool calculate_no_cache(const var_mask& state) = 0;

    var_mask vars() const {
        return _var_mask;
    }

private:
    bool was_updated(const var_mask& state) {
        var_mask changed_vars = (state & _var_mask) ^ _cached_state;
        return changed_vars.any();
    }

    bool _cache_exists;
    bool _cached_value;
    var_mask _cached_state;
    const var_mask _var_mask;
};

class binary_operation : public cached_expression {
public:
    binary_operation(
        const std::string& symbol,
        std::function<bool(bool, bool)> action,
        cached_expression* left,
        cached_expression* right
    ) : cached_expression(left->vars() | right->vars()),
        _symbol(symbol), 
        _action(action),
        _left(left),
        _right(right) {}

    std::string as_string() override {
        return "(" 
            + _symbol + "," 
            + _left->as_string() + "," 
            + _right->as_string() 
            + ")";
    }

    bool calculate_no_cache(const var_mask& state) override {
        return _action(
            _left->calculate(state), 
            _right->calculate(state)
        );
    }

private:
    std::string _symbol;
    std::function<bool(bool, bool)> _action;
    cached_expression* _left;
    cached_expression* _right;
};

class implication : public binary_operation {
public:
    implication(
        cached_expression* left,
        cached_expression* right
    ) : binary_operation(
            "->", 
            [](auto a, auto b) { return !a || b; },
            left, 
            right
        ) {}
};

class disjunction : public binary_operation {
public:
    disjunction(
        cached_expression* left,
        cached_expression* right
    ) : binary_operation(
            "|", 
            [](auto a, auto b) { return a || b; },
            left, 
            right
        ) {}
};

class conjunction : public binary_operation {
public:
    conjunction(
        cached_expression* left,
        cached_expression* right
    ) : binary_operation(
            "&", 
            [](auto a, auto b) { return a && b; },
            left, 
            right
        ) {}
};

class unary_operation : public cached_expression {
public:
    unary_operation(
        std::string const & symbol,
        std::function<bool(bool)> action,
        cached_expression* child
    ) : cached_expression(child->vars()),
        _symbol(symbol),
        _action(action),
        _child(child) {}

    inline std::string as_string() override {
        return "(" 
            + _symbol 
            + _child->as_string() 
            + ")";
    }

    bool calculate_no_cache(const var_mask& state) override {
        return _action(
            _child->calculate(state)
        );
    }

private:
    std::string _symbol;
    std::function<bool(bool)> _action;
    cached_expression* _child;
};

class negation : public unary_operation {
public:
    negation(
        cached_expression* child
    ) : unary_operation(
            "!", 
            [](bool a) { return !a; },
            child
        ) {}
};

class variable : public cached_expression {
public:
    variable(const symbol_table_entry& table_entry)
        : cached_expression(var_mask(1 << table_entry.id())),
          _table_entry(table_entry) {}

    inline std::string as_string() override {
        return _table_entry.name();
    }

    bool calculate_no_cache(const var_mask& state) override {
        return _table_entry.value();
    }

private:
    symbol_table_entry _table_entry;
};

#endif //CPP_SOLUTION_EXPRESSION_H
