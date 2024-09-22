#include "algorithm.hpp"
#include "graph.hpp"
#include "list.hpp"
#include "path.hpp"

#include <tuple>

template <int ID>
class Identifier
{
    static constexpr int id = ID;
};

template<typename... ListTypes>
constexpr auto nodeToUnderlying([[maybe_unused]]ctgl::list::List<ctgl::Node<ListTypes>...> L)
{
    return ctgl::list::List<ListTypes...>{};
}

template<typename... ListTypes, typename... L2, int... I>
constexpr auto edgeToTail([[maybe_unused]]ctgl::list::List<ctgl::graph::Edge<ctgl::Node<ListTypes>,ctgl::Node<L2>,I>...> L)
{
    return ctgl::list::List<ListTypes...>{};
}



template<typename TupleType, typename F, typename... ListTypes>
constexpr auto getSubTuple( TupleType T, ctgl::list::List<F,ListTypes...> L)
{
    if constexpr(ctgl::list::size(L)==1)
    {
        return std::tuple<F>(std::get<F>(T));
    }
    else
    {
        using LT = ctgl::list::List<ListTypes...>;
        return std::tuple_cat(std::tuple<F>(std::get<F>(T)), getSubTuple(T, LT{}));
    }
}
