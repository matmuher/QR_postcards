#pragma once

#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>
#include <MaPrinter.hpp>

class TextProcessor
{

    std::string _src;
    Tokenizer tokenizer;
    Parser parser;
    Creator creator;
    const std::vector<Object*>& obj_list;

public:

    TextProcessor(const std::string& src)
    :
        _src{src},
        tokenizer{_src},
        parser{tokenizer.tokenize()},
        creator{parser.getSketch()},
        obj_list{creator.create()}
    {
        std::cout << "Print tokenizer result:\n";
        std::cout << tokenizer;
    }

    const std::vector<Object*>& get_obj_list() const { return obj_list; }
}; 