#include <iostream>
#include <string>

#include "../inc/ctgl.hpp"

int main() {
    std::cout << "Running the CTGL examples...\n\n";

    {

        // Example 1
        // -------------------------------------------------------------------------
        // The nodes of a CTGL graph are ctgl::Node<> types with unique ID parameters.
        using n1 = ctgl::Node<int>;
        using n2 = ctgl::Node<long int>;
        using n3 = ctgl::Node<long long int>;
        using nodes = ctgl::List<n1, n2, n3>;

        // The directed, weighted edges of a CTGL graph are ctgl::Edge<> types.
        // 1 --(4)--> 2 --(4)--> 3
        // '---------(9)---------^
        using e12 = ctgl::Edge<n1, n2, 4>;
        using e13 = ctgl::Edge<n1, n3, 9>;
        using e23 = ctgl::Edge<n2, n3, 6>;
        using edges = ctgl::List<e12, e13, e23>;

        // By definition, a Graph is a composition of Nodes and Edges.
        using graph = ctgl::Graph<nodes, edges>;

        // |distance| represents the (shortest) distance between Node 1 and Node 3.
        // In this case, the value of |distance| will be 8.
        constexpr int distance = ctgl::algorithm::findDistance(graph{}, n1{}, n3{});
        std::cout << "The distance between Node 1 and Node 3 is " << distance << ".\n";
    }{
        // Example 2
        // -------------------------------------------------------------------------
        // Nodes represent cities in the TSP instance.
        struct Dubai
        {
            std::string cityName;
        };

        struct Miami
        {
            std::string cityName;
        };

        struct Paris
        {
            std::string cityName;
        };

        struct Tokyo
        {
            std::string cityName;
        };

        using dubai = ctgl::Node<Dubai>;
        using miami = ctgl::Node<Miami>;
        using paris = ctgl::Node<Paris>;
        using tokyo = ctgl::Node<Tokyo>;
        using cities = ctgl::List<dubai, miami, paris, tokyo>;

        // Edges represent unidirectional routes between cities.
        //                  .-------------(1)-------------v
        // Dubai --(1)--> Miami --(2)--> Paris --(1)--> Tokyo
        // ^   ^-----------(3)-----------'   ^---(4)----'   |
        // '----------------------(3)-----------------------'
        using routes = ctgl::List<ctgl::Edge<dubai, miami, 1>,
                                  ctgl::Edge<miami, paris, 2>,
                                  ctgl::Edge<miami, tokyo, 1>,
                                  ctgl::Edge<paris, dubai, 3>,
                                  ctgl::Edge<paris, tokyo, 1>,
                                  ctgl::Edge<tokyo, dubai, 3>,
                                  ctgl::Edge<tokyo, paris, 4>>;

        // |circuit| represents an optimal solution to the TSP instance from Dubai.
        // The type of |circuit| will express Dubai --> Miami --> Paris --> Tokyo --> Dubai.
        using world = ctgl::Graph<cities, routes>;
        constexpr auto circuit = ctgl::algorithm::findShortestRoute(world{}, dubai{}, cities{});
        constexpr int length = ctgl::path::length(circuit);
        std::cout << "The solution to the TSP has length " << length << ".\n";

        constexpr auto shortestPath = ctgl::algorithm::findShortestPath(world{},dubai{},tokyo{});
        auto tup = std::make_tuple(Dubai{"Dubai"},Miami{"Miami"},Paris{"Paris"},Tokyo{"Tokyo"});
        auto under = edgeToTail(shortestPath);
        auto sub = getSubTuple(tup, under);

        std::cout << "The shortest path from Dubai to Tokyo has flights from: \n";
        std::apply([](const auto&... vars){
            ((std::cout<<vars.cityName<<'\n'),...);
            },sub);
    }{
        // Example 3
        // -------------------------------------------------------------------------
        // Nodes represent currencies in the foreign exchange market.
        using aud = ctgl::Node<int>;
        using cad = ctgl::Node<uint>;
        using nzd = ctgl::Node<long int>;
        using usd = ctgl::Node<char>;
        using currencies = ctgl::List<aud, cad, nzd, usd>;

        // Edges represent logarithmic exchange rates between currencies. Why? If the
        // product of the exchange rates along a cycle is less than one, then the sum
        // of the logarithmic exchanges rates along that cycle must be less than zero.
        //                v----------(-2)----------.
        // NZD --(1)--> AUD --(-3)--> CAD --(3)--> USD
        using log_rates = ctgl::List<ctgl::Edge<nzd, aud,  1>,
                                     ctgl::Edge<aud, cad, -3>,
                                     ctgl::Edge<cad, usd,  3>,
                                     ctgl::Edge<usd, aud, -2>>;

        // |arbitrage| represents the possibility of arbitrage in the currency market.
        // Here, |arbitrage| will be set to true because AUD --> CAD --> USD --> AUD
        // forms a negative cycle.
        using market = ctgl::Graph<currencies, log_rates>;
        constexpr bool arbitrage = ctgl::graph::hasNegativeCycle(market{});
        std::cout << "The foreign exchange market " << (arbitrage ? "is" : "is not") << " susceptible to arbitrage.\n";
    }

    std::cout << "\nDone.\n";
}
