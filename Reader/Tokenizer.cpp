#include "Tokenizer.hpp"

// [tokenize]

void Tokenizer::tokenize()
{
    while(walker != src_end)
    {
        skip_whites();

        char c = *walker;
        Token* new_token = nullptr;

        if (false) // for homogeneity of conditions-check
            
            ;

        else if (std::isalpha(c))

            new_token = dig_word();
        
        else if (std::isdigit(c))
        
            new_token = dig_number();
        
        else if (std::ispunct(c))
        
            new_token = dig_punct();
        
        else
        {
            std::cout << "Unknown char: " << c
                      << ", ASCII: " << static_cast<int>(c) <<'\n';

            break;
        }

        if (new_token != nullptr)
            token_que.push_back(new_token);
    }
}

// [token creators]

// may be extended for more informable tokenizing
Token* Tokenizer::create_token(TokenType type,
                    text_type::const_iterator start,
                    text_type::const_iterator end)
{ return new Token{type, start, end};}

Token* Tokenizer::create_token(TokenType general_type, int specific_type,
                        text_type::const_iterator start,
                        text_type::const_iterator end)
{ return new QualifyToken{general_type, specific_type, start, end}; }

// [digging funtions]

 Token* Tokenizer::dig_word()
{
    text_type::const_iterator word_start = walker;
    
    while(walker != src_end && std::isalpha(*walker))
        ++walker;

    std::string word = std::string(word_start, walker);
    // std::cout << "get_word: " << word << '\n';
    
    if (objects.find(word) != objects.end())
    {
        ObjectType obj_type = objects.at(word);

        return create_token(TokenType::ObjectType, (int) obj_type, word_start, walker);
    }
    else if (colors.find(word) != colors.end())
    {
        ColorType color = colors.at(word);

        return create_token(TokenType::Color, (int) color, word_start, walker);
    }
    else if (props.find(word) != props.end())
    {
        PropertyType prop = props.at(word);

        return create_token(TokenType::Property, (int) prop, word_start, walker);
    }
    
    return create_token(TokenType::Unknown, word_start, walker);
}

Token* Tokenizer::dig_punct()
{
    // std::cout << "get_punct: " << *walker << '\n';

    Token* token_ptr = nullptr;
    TokenType token_type = TokenType::Unknown;

    switch (*walker)
    {
        case '!':
            token_type = TokenType::SizeScale;
            break;

        case '[':
            token_type = TokenType::LBrace;
            break;            

        case ']':
            token_type = TokenType::RBrace;
            break;            

        case '{':
            token_type = TokenType::LCurl;
            break;            

        case '}':
            token_type = TokenType::RCurl;
            break;            

        case '(':
            token_type = TokenType::LRound;
            break;            

        case ')':
            token_type = TokenType::RRound;
            break;            

        case ',':
            token_type = TokenType::Comma;
            break;            

        case ';':
            token_type = TokenType::SemiColon;
            break;

        case '=':
            token_type = TokenType::Assign;
            break;            

        default:
            token_type = TokenType::Unknown;
            break;            
    }

    token_ptr = create_token(token_type, walker, walker+1);
    ++walker;

    return token_ptr;
}

Token* Tokenizer::dig_number()
{
    std::string number_str;

    auto number_start = walker;
    while (walker != src_end && std::isdigit(*walker))
    {
        number_str.push_back(*walker);
        ++walker;
    }

    size_t number_str_len = 0;
    int number = -1;
    number = std::stoi(number_str, &number_str_len);

    return create_token(TokenType::Number, number, number_start, walker);
}

// [print functions]

std::ostream& operator<< (std::ostream& cout, const Tokenizer& tokenizer)
{
    const std::deque<Token*>& tokens = tokenizer.get_tokens();
    for (Token* token_ptr : tokens)
    {
        token_ptr->print(cout);
        std::cout << '\n' << '\n';
    }

    std::cout << '\n';

    return cout;
}

static void print_enum(std::ostream& cout, const char* token_type)
{
    const char* skipper = token_type;

    while(*skipper != ':') ++skipper;

    // skip "::"
        ++skipper;
        ++skipper;

    cout << skipper;
}

#define PRINT_ENUM(enum_value)                      \
                case enum_value:                    \
                    {print_enum(cout, #enum_value); \
                    break;}

std::ostream& operator<< (std::ostream& cout, TokenType type)
{
    cout << '[';
    switch (type)
    {
        PRINT_ENUM(TokenType::ObjectType);

        PRINT_ENUM(TokenType::Property);

        PRINT_ENUM(TokenType::Color);

        PRINT_ENUM(TokenType::SizeScale);

        // [brackets]

            PRINT_ENUM(TokenType::RBrace);
            PRINT_ENUM(TokenType::LBrace);
            PRINT_ENUM(TokenType::RCurl);
            PRINT_ENUM(TokenType::LCurl);
            PRINT_ENUM(TokenType::RRound);
            PRINT_ENUM(TokenType::LRound);

        // [delimiters]

            PRINT_ENUM(TokenType::Comma);
            PRINT_ENUM(TokenType::SemiColon);

        PRINT_ENUM(TokenType::Assign);

        PRINT_ENUM(TokenType::Number);
    
        case TokenType::Unknown:
        default:
            cout << "Unknown";
            break;
    }
    cout << ']';

    return cout;
}
#undef PRINT_ENUM

std::ostream& operator<< (std::ostream& cout, Token token)
{
    auto start = token.start();
    auto end = token.end();

    cout << token.type() << ' ';
    
    while (start != end)
        cout << *start++;

    cout << ' ';

    return cout;
}

std::ostream& Token::print (std::ostream& cout) const
{
    auto start = _start;
    auto end = _end;

    cout << _type << ' ';
    
    while (start != end)
        cout << *start++;

    cout << ' ';

    return cout;
}

std::ostream& QualifyToken::print (std::ostream& cout) const
{
    Token::print(cout);
    cout << _specific_type << '\n';
    return cout;
}

// [others]

void Tokenizer::skip_whites()
{
    while(walker != src_end && std::isspace(*walker))
        ++walker;
}
