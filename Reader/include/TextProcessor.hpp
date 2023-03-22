#pragma once

#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>
#include <MaPrinter.hpp>

class TextProcessor
{
    Tokenizer tokenizer;
    Parser parser;
    Creator creator;
    std::vector<Object*> obj_list;

public:

    TextProcessor(const std::string& src)
    {
        tokenizer.initialize(src);        
        auto& token_que = tokenizer.tokenize();

        parser.initialize(token_que);
        auto root = parser.getSketch();

        creator.initialize(root);
        obj_list = std::move(creator.create());
    }

    std::vector<Object*>& get_obj_list() { return obj_list; }
}; 