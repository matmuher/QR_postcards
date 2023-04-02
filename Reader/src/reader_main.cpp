#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>
#include <MaPrinter.hpp>
#include <TextProcessor.hpp>

int main(int argc, const char* argv[])
{
    TextProcessor text_processor;
    text_processor.get_input(argc, argv);
    text_processor.process();

    auto obj_list = text_processor.get_obj_list();

    for (auto elem : obj_list)
    {
        elem->print(std::cout);
        std::cout << "\n\n";
    }

    return 0;
}
