#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>

int main()
{
    std::string test_str = "congrat[2,2]{+\"meow\"{size=0;} +\"Hellow, world!\"{size=1;}}"
                           "congrat[2,2]{+\"meow\"{size=0;} +\"Hellow, world!\"{size=1;}}"; //

    Tokenizer tokenizer{test_str};
    tokenizer.tokenize();
    std::cout << tokenizer;
    const std::deque<Token*> tokens = tokenizer.get_tokens();

    Parser parser{tokens};
    SketchNode* sketch = parser.getSketch();
    
    sketch->print(std::cout, 0);

    std::cout << "Start creating\n";
    Creator creator(sketch);
    auto obj_list = creator.create();

    for (auto elem : obj_list)
    {
        elem->print(std::cout);
        std::cout << "\n\n";
    }

    return 0;
}
