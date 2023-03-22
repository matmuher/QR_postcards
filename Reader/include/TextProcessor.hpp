#pragma once

#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>
#include <MaPrinter.hpp>
#include <TextProcessExceptions.hpp>

class TextProcessor
{
    Tokenizer tokenizer;
    Parser parser;
    Creator creator;
    std::vector<Object*> obj_list;

public:

    TextProcessor(const std::string& src)
    {
        try
        {
            tokenizer.initialize(src);        
            auto& token_que = tokenizer.tokenize();
            std::cout << tokenizer;

            parser.initialize(token_que);
            auto root = parser.getSketch();

            creator.initialize(root);
            obj_list = std::move(creator.create());
        }
        catch(tokenize_error& err)
        {
            std::cout << "[error] " << err.what() << '\n';
            tokenizer.print_context(std::cout, err._anchor_it);
            std::cout << '\n';
        }
        catch(std::exception& err)
        {
            std::cout << err.what() << '\n';
        }
    }

    std::vector<Object*>& get_obj_list() { return obj_list; }
}; 