#pragma once

#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>
#include <MaPrinter.hpp>
#include <TextProcessExceptions.hpp>

std::ostream& print_error(const std::string& msg)
{
    return std::cout << "[error" << ": " << msg << ']';
}

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
            print_error("grammar") << ' ' << err.what() << '\n';
            tokenizer.print_line(std::cout, err.line_id);
            tokenizer.print_anchor(std::cout, err.line_id, err.anchor);
            std::cout << '\n';
        }
        catch(parse_error& err)
        {
            print_error("syntax") << ' ';

            std::cout << "Required token: {" << str_enum(err.required_type);
            std::cout << "}, but got: {" << str_enum(err.got_type) << "}\n";

            tokenizer.print_line(std::cout, err.line_id);
            tokenizer.print_anchor(std::cout, err.line_id, err.anchor);

            std::cout << '\n'; 
        }
        catch(std::exception& err)
        {
            std::cout << err.what() << '\n';
        }
    }

    std::vector<Object*>& get_obj_list() { return obj_list; }
}; 