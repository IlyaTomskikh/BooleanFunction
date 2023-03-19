#include "BooleanFunction.h"

using namespace std;

int main() {
//    srand(time(nullptr));
//    BooleanFunction tmp(3, 3);
//    std::cout << tmp.to_string() + " w = " + std::to_string(tmp.w()) << std::endl;
//    for (u_int i = 2; i < 32; ++i) {
//        auto bf = BooleanFunction(i, 1);
//        std::cout << float(bf.w()) / u_int(1 << bf.getN()) << std::endl;
//    }
    string str;
//    std::cin >> str;
//    str = "11100010";
    BooleanFunction bf;
//    try {
//        bf = str;
//    } catch (const runtime_error &re) {
//        cout << re.what() << endl;
//        return 1;
//    }
//    cout << "str = " + str + ", bf = " << bf.to_string() << ", bf.n = " << bf.getN() << endl;
//    cout << "mobius(bf) = " << bf.mobius().to_string() << " mobius(mobius(bf)) = " << bf.mobius().mobius().to_string() << " ANF = " << bf.anf() << endl;
    int n = 31;
    bf = BooleanFunction(n, 2);
    auto mob = bf.mobius();
    auto mobmob = mob.mobius();
    cout /*<< "bf(n=" + to_string(n) + ") = " << bf*/
//    << "\n anf = " << bf.anf() << endl;
//    << "\nmobius(bf) = " << mob
//    << "\nmobius(mobius(bf)) = " << mobmob
    << "\nmobius(mobius) == bf - ? = " << boolalpha << (bf == mobmob) << endl;
    return 0;
}
