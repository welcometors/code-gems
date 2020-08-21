// original code from
// CppCon 2017: Nicolai Josuttis “The Nightmare of Move Semantics for Trivial Classes”

#include <string>

class Cust {
private:
    std::string first;
    std::string last;
    int id;

public:
    template<typename T1, typename T2 = std::string, 
        typename = std::enable_if_t<std::is_convertible_v<T1, std::string>>>
    Cust(T1&& _first, T2&& _last = "", int _id = 0) :
        first(std::forward<T1>(_first)), 
        last(std::forward<T2>(_last)),
        id(_id) {}
};

Cust a{ "John", "Cena", 42 };
std::string v = "John";
Cust b{ v, "Cena", 42 };
const Cust c{ "friza" };
Cust d = c;
Cust e{ c };

class VIP: public Cust {
    using Cust::Cust;
};

VIP v = "boss";
VIP u = v;
Cust cv{ v };