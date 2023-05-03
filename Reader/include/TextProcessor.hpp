#pragma once

#include <fstream>
#include <cstring>

#include <Tokenizer.hpp>
#include <Parser.hpp>
#include <Creator.hpp>
#include <MaPrinter.hpp>
#include <TextProcessExceptions.hpp>

#ifdef NDEBUG

    #define LOG(arg)

#else

    #define LOG(arg) std::cout << arg

#endif

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
    std::string _src;
    int _background_id = 0;

public:

    enum ErrorType
    {
        NO_ERROR,
        SYNTAX_ERROR,
        LEX_ERROR
    };

private:

    ErrorType error_type = NO_ERROR;

public:

    TextProcessor(const std::string& src) : _src{src} {}
    TextProcessor() {};

    int get_input(int argc, const char* argv[])
    {
        _src.clear();

        if (argc > 1)
        {
            std::ifstream input_file;
            input_file.open(argv[1]);

            if (input_file.fail())
            {
                std::cout << argv[1] << ": " << std::strerror(errno) << '\n';
                return 1;
            }

            std::stringstream buffer_stream;

            buffer_stream << input_file.rdbuf();
            _src = buffer_stream.str();
        }
        else
        {
            for (std::string str; std::getline(std::cin, str);)
            {
                if (!str.empty())
                    _src += (str + '\n');
            }
        }

        return 0;

    }

    void process()
    {
        try
        {
            tokenizer.initialize(_src);        
            auto& token_que = tokenizer.tokenize();
            LOG(tokenizer);

            parser.initialize(token_que);
            auto root = parser.getSketch();
            _background_id = parser.get_background();

            LOG("Background id: " <<  _background_id << '\n');

            creator.initialize(root);
            obj_list = std::move(creator.create());
        }
        catch(tokenize_error& err)
        {
            print_error("grammar") << ' ' << err.what() << '\n';
            tokenizer.print_line(std::cout, err.line_id);
            tokenizer.print_anchor(std::cout, err.line_id, err.anchor);
            std::cout << '\n';

            error_type = ErrorType::LEX_ERROR;
        }
        catch(parse_error& err)
        {
            print_error("syntax") << ' ';

            std::cout << "Required token: {" << str_enum(err.required_type);
            std::cout << "}, but got: {" << str_enum(err.got_type) << "}\n";

            tokenizer.print_line(std::cout, err.line_id);
            tokenizer.print_anchor(std::cout, err.line_id, err.anchor);

            std::cout << '\n'; 

            error_type = ErrorType::SYNTAX_ERROR;
        }
        catch(std::exception& err)
        {
            std::cout << err.what() << '\n';
        }
    }

    int get_background() const { return _background_id; }

    std::vector<Object*>& get_obj_list() { return obj_list; }

    ErrorType get_error_type() const { return error_type; }
}; 