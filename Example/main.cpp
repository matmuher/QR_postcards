#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>

int main()
{
    std::string test_str = "pine!!!!![12,24]{color = blue;}"
                           "star[1,2]{color=red;}"
                           "pine!!![0, 12]{color = green;}"
                           "star[1, 333]{color = red;}"; //

    std::vector<const Object*> obj_list;
    //                                                              size x  y
    obj_list.push_back(new Object{ObjectType::Pine, ColorType::Green, 4, 5, 0});
    obj_list.push_back(new Object{ObjectType::Star, ColorType::Red,   2, 0, 10});

    for (auto elem : obj_list)
    {
        elem->print(std::cout);
        std::cout << "\n\n";
    }
    // std::cout << parser;
    // parser.get_objects();

    return 0;
}
