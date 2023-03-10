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
    std::cin >> str;
//    str = "11100010";
    BooleanFunction bf;
    try {
        bf = str;
    } catch (const runtime_error &re) {
        cout << re.what() << endl;
        return 1;
    }
    cout << "str = " + str + ", bf = " << bf.to_string() << ", bf.n = " << bf.getN() << endl;
    return 0;
}
