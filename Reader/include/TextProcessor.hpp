#pragma once

#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>
#include <MaPrinter.hpp>

class TextProcessor
{

    std::string _test_str;
    Tokenizer tokenizer;
    Parser parser;
    Creator creator;

public:

    TextProcessor(const std::string& test_str)
    :
        _test_str{test_str},
        tokenizer{_test_str},


    const std::vector<const Object*>& process()
    {
        Tokenizer tokenizer{_test_str};
        tokenizer.tokenize();
        std::cout << tokenizer;
        const std::deque<Token*> tokens = tokenizer.get_tokens();

        Parser parser{tokens};
        SketchNode* sketch = parser.getSketch();
        
        sketch->print(std::cout, 0);

        std::cout << "Start creating\n";
        Creator creator(sketch);
        const std::vector<const Object*>& obj_list = creator.create();

        return obj_list;
    }
};