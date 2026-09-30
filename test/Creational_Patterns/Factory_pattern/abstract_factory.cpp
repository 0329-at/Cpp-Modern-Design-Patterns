#include <print>
#include <memory>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/cat.hpp>

/*
使用泛型宏消除重复
*/

struct Lux   {};
struct Ziggs {};
struct Teemo {};

struct LuxEasy   { constexpr auto identify()  { std::println("LuxEasy"); } };
struct ZiggsEasy { constexpr auto identify()  { std::println("ZiggsEasy"); } };
struct TeemoEasy { constexpr auto identify()  { std::println("TeemoEasy"); } };

template<class... Ts>
struct AbstractFactory : Ts... {
    using Ts::operator()...;
};

template<class T, class U>
concept IsAIType = std::is_same_v<T, U>;

#define _GEN_LAMBDA_IMPL(AIType, Level, AIName) \
    []() requires IsAIType<AIType, AIName> { return std::make_unique<BOOST_PP_CAT(AIName, Level)>(); },

#define _EXPAND(x) x

#define _PP_LAMBDA(r, data, elem) \
    _EXPAND(_GEN_LAMBDA_IMPL( \
        BOOST_PP_TUPLE_ELEM(0, data), \
        BOOST_PP_TUPLE_ELEM(1, data), \
        elem \
    ))

#define AI_REGISTER(AIType, Level, ...) \
    BOOST_PP_SEQ_FOR_EACH( \
        _PP_LAMBDA, \
        (AIType, Level), \
        BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__) \
    )

template<class T>
static constexpr auto CreateAI = AbstractFactory {
    AI_REGISTER(T, Easy, Lux, Ziggs, Teemo)
};

// ===================== 使用 =====================
int main() {
    auto ziggs = CreateAI<Ziggs>();
    ziggs->identify();  // ZiggsEasy

    auto lux = CreateAI<Lux>();
    lux->identify();    // LuxEasy

    auto teemo = CreateAI<Teemo>();
    teemo->identify();  // TeemoEasy
}