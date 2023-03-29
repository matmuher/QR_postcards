#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>
#include <MaPrinter.hpp>
#include <TextProcessor.hpp>

int main()
{
    std::string test_str =  "pine [ -2, 7 ] {colo=red;}\n"
                            "star{color=yellow;}";

    TextProcessor text_processor{test_str};
    auto obj_list = text_processor.get_obj_list();

    for (auto elem : obj_list)
    {
        elem->print(std::cout);
        std::cout << "\n\n";
    }

    return 0;
}
