//
// Created by Ilya on 15.02.2023.
//

#ifndef BF_BOOLEANFUNCTION_H
#define BF_BOOLEANFUNCTION_H

#include <random>
#include <chrono>
#include <iostream>

typedef unsigned int u_int;

class BooleanFunction {

private:
    /// 000, 001, 010, ...
    u_int *f{};
    u_int n{};
    u_int len{};
    std::string bits_el(u_int ix);
    u_int w_el(u_int ix);

public:
    u_int *getF() const;

    u_int getN() const;

    u_int getLen() const;

    explicit BooleanFunction(u_int v_n, unsigned short type = 0);
    BooleanFunction();
    BooleanFunction(BooleanFunction const &bf);
    BooleanFunction(const std::string& bf);

    std::string to_string();
    u_int w();

    virtual ~BooleanFunction();

    bool operator==(const BooleanFunction &rhs) const;

    bool operator!=(const BooleanFunction &rhs) const;

};


#endif //BF_BOOLEANFUNCTION_H
