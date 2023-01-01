#include "expression.h"
#include "symbol_table.h"
#include "parse/gen/expression.tab.hpp"
#include "parse/gen/expression.lexer.hpp"
#include <iostream>

extern cached_expression* result;
extern symbol_table* table;

void yyerror(const char *error) {
    std::cerr << error;
}

int yywrap() {
    return 1;
}

int main() {
    std::string input;
    std::cin >> input;
    yy_scan_string(input.c_str());
    yyparse();

    // std::cout 
    //    << result->as_string()
    //    << std::endl;

    size_t true_cases = 0;
    size_t false_cases = 0;
    const var_values end = (1 << table->size());
    for (var_values vals = 0; vals < end; ++vals) {
        table->set_state(vals);
        if (result->calculate(vals)) {
            ++true_cases;
        } else {
            ++false_cases;
        }
    }

    if (true_cases == 0) {
        std::cout << "Unsatisfiable" << std::endl;
    } else if (false_cases == 0) {
        std::cout << "Valid" << std::endl;
    } else {
        std::cout 
            << "Satisfiable and invalid, " 
            << true_cases << " true and " 
            << false_cases << " false cases"
            << std::endl;
    }

    return 0;
}