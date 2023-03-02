#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>

int main()
{
    std::string test_str = "pine!!!!![12,24]{color = blue; pine{color=red;}}"
                           "star[1,2]{color=red;}"
                           "pine!!![0, 12]{color = green;}"
                           "star[1, 333]{color = red;}"; //

    Tokenizer tokenizer{test_str};
    tokenizer.tokenize();
    std::cout << tokenizer;
    const std::deque<Token*> tokens = tokenizer.get_tokens();

    Parser parser{tokens};
    SketchNode* sketch = parser.getSketch();
    
    sketch->print(std::cout, 0);

    Creator creator(sketch);
    auto obj_list = creator.create();

    for (auto elem : obj_list)
    {
        elem->print(std::cout);
        std::cout << "\n\n";
    }
    // std::cout << parser;
    // parser.get_objects();

    return 0;
}
