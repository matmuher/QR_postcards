#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>
#include <MaPrinter.hpp>
#include <TextProcessor.hpp>

int main()
{
    std::string test_str =  "pine[2,2]{color=red;}"
                        "star{color=yellow;}";

#if 0
    Tokenizer tokenizer{test_str};
    tokenizer.tokenize();
    std::cout << tokenizer;
    const std::deque<Token*> tokens = tokenizer.get_tokens();

    Parser parser{tokens};
    SketchNode* sketch = parser.getSketch();

    sketch->print(std::cout, 0);

    std::cout << "Start creating\n";
    Creator creator(sketch);
#endif

    TextProcessor text_processor{test_str};
    auto obj_list = text_processor.get_obj_list();

    for (auto elem : obj_list)
    {
        elem->print(std::cout);
        std::cout << "\n\n";
    }

    return 0;
}
