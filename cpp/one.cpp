#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <variant>
#include <any>

void create_line();

auto main() -> int
{
    // ---------------------------------------------
    // std:extent test
    typedef int array[5][24][60];
    char text[] = {'M', 'a', 't', 'e', 'u', 's', 'z'};

    auto result1 = std::extent<array, 0>::value;
    auto result2 = std::extent<array, 1>::value;
    auto result3 = std::extent<array, 2>::value;
    auto result4 = std::extent<array, 3>::value;
    auto resultT = std::extent<decltype(text)>::value;

    std::cout << "For array:\n" << result1 << " " << result2 << " " << result3
       << " " << result4 << "\n";
    std::cout << "For text:\n" << resultT << "\n";

    create_line();

    // ---------------------------------------------
    // std::vector<>::iterator test
    std::vector<int> ve = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11};
    std::vector<int>::iterator first = ve.begin();
    auto last = ve.end();

    int index_first = std::distance(ve.begin(), first); // index = 0 - 0 = 0
    int index_last = std::distance(ve.begin(), last); // index = 10 - 0 = 10

    std::cout << "Index: ";
    std::cout << index_first << " " << index_last << "\n";
    // in C++ array starts from 0 so ve[index_last = 10] will return garbage 
    // because of that we have to remember about decrementing it by 1
    std::cout << "ve[index]: ";
    std::cout << ve[index_first] << " " << ve[index_last - 1] << "\n";

    std::advance(first, 2); // moves iterator by 2 positions forward
    index_first = std::distance(ve.begin(), first); // index = 2 - 0
    std::advance(last, -2); // moves iterator by 2 positions back
    index_last = std::distance(ve.begin(), last); // index = 8 - 0

    std::cout << "Index: ";
    std::cout << index_first << " " << index_last << "\n" << "ve[index]: "
     << ve[index_first] << " " << ve[index_last] <<"\n";

    std::vector<int>::iterator next = std::next(first, 2); // moves iterator by 2 positions forward
    index_first = std::distance(ve.begin(), next); // index = 4 - 0
    auto prev = std::prev(last, 2); // moves iterator by 2 positions back
    index_last = std::distance(ve.begin(), prev); // index = 6 - 0

    std::cout << "Index: ";
    std::cout << index_first << " " << index_last << "\n" << "ve[index]: "
     << ve[index_first] << " " << ve[index_last] <<"\n";

    create_line();

    // ---------------------------------------------
    // std::tuple std::tie, examples with lambdas!
    std::tuple<int, int, int> element = std::make_tuple(10, 15, 97);

    auto multiply_tuple = [&element](std::string done)
    { 
            int result = std::get<0>(element) * std::get<1>(element);
            std::cout << done << std::endl;
            return result;
    };
    std::cout << multiply_tuple("Done") << "\n";

    int A;
    std::string B;
    char C;
    auto three_results = [&element](std::tuple<std::string, int> tuple)
    {
            int number = std::get<1>(tuple) + std::get<1>(element);
            std::string text = "Mati " + std::get<0>(tuple) + " awesome!";
            char character = static_cast<char>(std::get<2>(element));

            return std::make_tuple(number, text, character);
    };
    std::tie(A, B, C) = three_results(std::make_tuple("is", 5));
    std::cout << A << "\n" << B << "\n" << C << "\n";
    std::tie(A, std::ignore, C) = three_results(std::make_tuple("IGNORE IT", 10));
    // text doesn't change! It is ignored!
    std::cout << A << "\n" << B << "\n" << C << "\n";

    create_line();
    // ---------------------------------------------
    // std::pair, std::map
    std::pair<int, int> pair = std::make_pair(10, 25);
    auto multiply_pair = [&pair](int x)
    {
        return pair.first * pair.second + x;
    };
    std::cout << multiply_pair(50) << '\n';

    std::map<std::string, int> map;
    map.insert( {"KEY", 17} );
    map.insert( {"SECOND", 18} );

    for(auto const& [key, value] : map)
        std::cout << "Value for " << key << " is " << value << "\n";

    create_line();
    // ---------------------------------------------
    // std::variant
    std::variant<int, std::string> var;
    auto lVar = [](auto&& e) {
        std::cout << e << "\n";
    };

    var = 20;    
    // now we are sure that this won't throw an issue, if we are wrong, it will return only nullptr
    // normally use std::get
    std::string* sVar = std::get_if<std::string>(&var);
    int* iVar = std::get_if<int>(&var);

    std::cout << sVar << " " << iVar << std::endl;
    //std::cout << *sVar << " " << *iVar << std::endl;

    std::visit(lVar, var);

    var = "hello";
    sVar = std::get_if<std::string>(&var);
    iVar = std::get_if<int>(&var);
    std::cout << sVar << " " << iVar << std::endl;
    //std::cout << *sVar << " " << *iVar << std::endl;

    std::visit(lVar, var);

    create_line();
    // ---------------------------------------------
    // std::any

    std::any aObject { std::string("HelloWorld") };

    if( aObject.has_value() )
        std::cout << std::any_cast<std::string>(aObject) << std::endl;

    try 
    {
        std::cout << std::any_cast<int>(aObject) << std::endl;
    }
    catch(std::bad_any_cast&)
    {
        std::cout << "Wrong Type!" << std::endl;
    }

    create_line();
    // ---------------------------------------------
    // 

    return 0;
}

void create_line()
{
    std::cout << "----------------------------------\n";
}