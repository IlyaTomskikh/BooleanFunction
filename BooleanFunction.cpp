//
// Created by Ilya on 15.02.2023.
//

#include "BooleanFunction.h"
#include <bitset>

BooleanFunction::BooleanFunction(u_int v_n, unsigned short type): n(v_n) {
    if (n < 5) len = ((1 << n) + 31) >> 5;
    else len = 1 << (n - 5);
    f = new u_int[len];
    switch (type) {
        case 0:
            std::fill(f, f + len, u_int(0));
            break;
        case 1:
            std::fill(f, f + len, u_int(~0));
            if (n < 5) {
                auto k = 32 - (1 << n);
                f[0] = f[0] << k >> k;
            }
            break;
        default:
            std::mt19937 gen(std::random_device{}());
            for(auto i = 0; i < len; ++i) f[i] = gen();
            if (n < 5) {
                auto k = 32 - (1 << n);
                f[0] = f[0] << k >> k;
            }
            break;
    }
}

BooleanFunction::BooleanFunction() {
    n = len = 0;
    f = nullptr;
}

BooleanFunction::~BooleanFunction() {
    if (f) {
        std::fill(f, f + len, 0);
        delete f;
    }
    n = len = 0;
}

BooleanFunction::BooleanFunction(const std::string& bf) {
    if (bf.find_first_not_of("01") != std::string::npos) throw std::runtime_error("string must contain only {0,1}*");
    if (bf.length() != 0 && (bf.length() & (bf.length() - 1)) != 0) throw std::runtime_error("string's length != 2^n");
    len = bf.length() >> 5;
    if ((bf.length() & 31) != 0) ++len;
    f = new u_int[len];
    std::fill(f, f + len, 0);
    if (len == 1) for (int j = bf.length() - 1; j >= 0; --j) {
        f[0] |= bf[j] - '0';
        if (j != 0) f[0] <<= 1;
    }
    else for (int i = 0; i < len; ++i) {
        auto tmp = bf.substr(i << 5, (i << 5) + 32);
        for (int j = 31; j >= 0; --j) {
            f[i] |= tmp[j] - '0';
            if (j != 0) f[i] <<= 1;
        }
    }
    n = 1;
    u_int x = bf.length();
    if ((x << 16) == 0) {
        n += 16;
        x >>= 16;
    }
    if ((x << 24) == 0) {
        n += 8;
        x >>= 8;
    }
    if ((x << 28) == 0) {
        n += 4;
        x >>= 4;
    }
    if ((x << 30) == 0) {
        n += 2;
        x >>= 2;
    }
    n -= x & 1;
}

std::string BooleanFunction::to_string() const {
    if (f == nullptr) return "nullptr";
    std::string res = "";
    for (auto i = 0; i < len; ++i) res += bits_el(i);
    return res;
}

u_int BooleanFunction::w() {
    u_int w = 0;
    for (int i = 0; i < len; ++i) w += w_el(i);
    return w;
}

BooleanFunction::BooleanFunction(const BooleanFunction &bf): n(bf.n), len(bf.len) {
    f = new u_int[len];
    std::copy(bf.f, bf.f + len, f);
}

u_int BooleanFunction::w_el(u_int ix) {
    if (f == nullptr) throw std::runtime_error("bf is null");
    auto x = f[ix];
    u_int w = 0;
    while (x) {
        x &= (x - 1);
        ++w;
    }
    return w;
}

u_int *BooleanFunction::getF() const {
    auto copy = new u_int[len];
    std::copy(f, f + len, copy);
    return copy;
}

u_int BooleanFunction::getN() const {
    return n;
}

u_int BooleanFunction::getLen() const {
    return len;
}

std::string BooleanFunction::bits_el(u_int ix) const {
    std::string res = "";
    auto val = f[ix];
    auto min = (n < 5) ? (1 << n) : 32;
//    std::cout << "______________________" << '\n';
//    std::cout << std::to_string(ix) + " <- ix | f[ix] ->" << std::bitset<sizeof(f[0])*8>(f[ix]) << std::endl;
//    for (int i = 0; i < min; ++i) std::cout << std::to_string((val & (1 << i)) >> i);
//    std::cout << "\n______________________" << '\n';
    for (int i = 0; i < min; ++i) res += std::to_string(((val & (1 << i)) >> i));
    return res;
}

bool BooleanFunction::operator==(const BooleanFunction &rhs) const {
    if (n != rhs.n || len != rhs.len || f == nullptr) return false;
    for (auto i = 0; i < len; ++i) if (f[i] != rhs.f[i]) return false;
    return true;
}

bool BooleanFunction::operator!=(const BooleanFunction &rhs) const {
    return !(rhs == *this);
}

BooleanFunction &BooleanFunction::operator=(const BooleanFunction &bf) {
    if (this == &bf) return *this;
    len = bf.len;
    n = bf.n;
    f = new u_int[len];
    for (auto i = 0; i < len; ++i) f[i] = bf.f[i];
    return *this;
}

BooleanFunction& BooleanFunction::operator=(const std::string &bf_str) {
    *this = BooleanFunction(bf_str);
    return *this;
}

BooleanFunction BooleanFunction::mobius() {
    BooleanFunction g(*this);
    int i = 0;
    for (; i < n; ++i) {
        u_int sh = 1 << i;
        switch (i) {
            case 0:
                for (int j = 0; j < len; ++j)
                    g.f[j] ^= (g.f[j] << sh) & 0xaaaaaaaa;
                break;
            case 1:
                for (int j = 0; j < len; ++j)
                    g.f[j] ^= (g.f[j] << sh) & 0xcccccccc;
                break;
            case 2:
                for (int j = 0; j < len; ++j)
                    g.f[j] ^= (g.f[j] << sh) & 0xf0f0f0f0;
                break;
            case 3:
                for (int j = 0; j < len; ++j)
                    g.f[j] ^= (g.f[j] << sh) & 0xff00ff00; //  1111 1111 0000 0000 1111 1111 0000 0000
                break;
            case 4:
                for (int j = 0; j < len; ++j)
                    g.f[j] ^= (g.f[j] << sh) & 0xffff0000; // 1111 1111 1111 1111 0000 0000 0000 0000
                break;
            default:
//                i from {5, 6, ...}
//                i = 5: 1111 1111 1111 1111 1111 1111 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000
//                std::string v = "";
//                while(v.length() < (1 << g.n)) v += std::string(sh, '1') + std::string(sh, '0');
//                std::cout << "\n\nv = " << v << " " << v.length() << std::endl << std::endl;
//                g ^= (g << sh) & BooleanFunction(v);
                auto l = i - 4;  // length of the block
                for (int j = 0; j < len - 1; j += l) {
                    for (int k = j + l + 1; k < j + l + 1; ++k) g.f[k] ^= g.f[k - j];
                }
                break;
        }
    }
    return g;
}

u_int BooleanFunction::operator[](int i) {
    if (!f) throw std::runtime_error("NullPointer Exception");
    return (i < len) ? f[i] : throw std::invalid_argument("IndexOutOfBounds Exception");
}

BooleanFunction BooleanFunction::operator^(const BooleanFunction &bf) {
    BooleanFunction res(*this);
    res ^= bf;
    return res;
}

BooleanFunction &BooleanFunction::operator^=(const BooleanFunction &bf) {
    if (!f) throw std::runtime_error("NullPointer Exception");
    auto min = (len <= bf.len) ? len : bf.len;
    auto max = (len > bf.len) ? len : bf.len;
    auto tmp = new u_int[max];
    int i = 0;
    for (; i < min; ++i) tmp[i] = f[i] ^ bf.f[i];
    for (; i < max; ++i) {
        if (max == len) tmp[i] = f[i];
        else tmp[i] = bf.f[i];
    }
    f = tmp;
    return *this;
}

BooleanFunction BooleanFunction::operator&(const BooleanFunction &bf) {
    BooleanFunction res(*this);
    if (res.len != bf.len) throw std::invalid_argument("BF lens must be equal");
    for (int i = 0; i < res.len; ++i) res.f[i] &= bf.f[i];
    return res;
}

BooleanFunction &BooleanFunction::operator&=(const BooleanFunction &bf) {
    *this = *this & bf;
    return *this;
}

BooleanFunction BooleanFunction::operator<<(u_int k) {
    if (k < 0) throw std::invalid_argument("Shifting by negative");
    BooleanFunction res(*this);
    if (k == 0) return res;
    u_int sh = k;
    while (sh >= 32) {
        sh -= 32;
        for (int i = 1; i < len; ++i) res.f[i - 1] = res[i];
        res.f[len - 1] = 0;
    }
    if (sh == 0) return res;
    u_int mask = (~0) << (32 - k);
    for (int i = 1; i < len; ++i) res.f[i - 1] = res[i] & mask;
    res.f[len - 1] = res[len - 1] & ((~0) << k);
    return res;
}

BooleanFunction &BooleanFunction::operator<<=(u_int k) {
    *this = *this << k;
    return *this;
}

std::vector<int> BooleanFunction::walsh_adamar_transformation() {
    return green(*this);
}

std::vector<int> BooleanFunction::green(const BooleanFunction &bf) {
    return std::vector<int>();
}

std::ostream &operator<<(std::ostream &os, const BooleanFunction &bf) {
    os << bf.to_string();
    return os;
}

std::string BooleanFunction::anf() {
    std::string anf("");
    BooleanFunction mob(mobius());
    BooleanFunction t(mob.n, 0);
    if (mob == t) return "0";
    t.f[0]++;
    if (mob == t) return "1";
    for (int i = 0; i < mob.len; ++i) {
        int k = 0;
        for (u_int mask = 1; mask != 0; mask <<= 1) {
//            std::cout << mask << '\n';
            if ((mask & mob[i]) != 0) {
//                std::cout << (mask & mob[i]) << " met 1 \n";
                for (int j = 0; j < mob.n; ++j) {
                    auto tmp = (k & (1 << j));
                    if (tmp != 0) anf += 'x' + std::to_string(n - j);
                }
                anf += " (+) ";
            };
            ++k;
        }
    }
    anf = anf.substr(0, anf.length() - 5);
    return anf;
}
