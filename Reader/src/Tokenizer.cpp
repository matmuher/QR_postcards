#include <Tokenizer.hpp>
#include <EnumPrinter.hpp>
#include <TextProcessExceptions.hpp>

// [tokenize]

const std::deque<Token*>& Tokenizer::tokenize()
{
    while(walker != src_end)
    {
        skip_whites();

        char c = *walker;
        Token* new_token = nullptr;

        if (c == '"')
            
            new_token = dig_line();

        else if (std::isalpha(c))
        {
            new_token = dig_word();
        }
        else if (std::isdigit(c))
        {
            new_token = dig_number();
        }
        else if (std::ispunct(c))
        {
            new_token = dig_punct();
        }
        else
        {
            std::cout << "Unknown char: " << c
                      << ", ASCII: " << static_cast<int>(c) <<'\n';

            break;
        }

        if (new_token != nullptr)
        {
            token_que.push_back(new_token);
            new_token->set_src_line(cur_line_id);
        }
    }

    return token_que;
}

// [token creators]

// may be extended for more informable tokenizing
Token* Tokenizer::create_token(TokenType type,
                    text_type::const_iterator start,
                    text_type::const_iterator end)
{ return new Token{type, start, end}; }

Token* Tokenizer::create_token(TokenType general_type, int specific_type,
                        text_type::const_iterator start,
                        text_type::const_iterator end)
{ return new QualifyToken{general_type, specific_type, start, end}; }

// [digging funtions]

Token* Tokenizer::dig_line()
{
    ++walker; // skip starting quote
    text_type::const_iterator word_start = walker;
    
    while(walker != src_end && *walker != '"')
        ++walker;

    if (walker == src_end)
    {
        std::cout << "[ERROR] No finishing quote was found\n";
    }
    ++walker; // skip ending quote

    return create_token(TokenType::String, word_start, walker-1); 
}

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
    
    throw tokenize_error("Unknown keyword", cur_line_id, word_start);

    return create_token(TokenType::Unknown, word_start, walker);
}

Token* Tokenizer::dig_punct()
{
    // std::cout << "get_punct: " << *walker << '\n';

    Token* token_ptr = nullptr;
    TokenType token_type = TokenType::Unknown;

    switch (*walker)
    { // copypaste
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

        case '+':
            token_type = TokenType::Plus;
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
    const std::map<int, text_type::const_iterator>& lines = tokenizer.get_source_lines();

    for (auto& line : lines)
    {
        cout << line.first << ": ";

        tokenizer.print_line(cout, line.first);

        cout << '\n';
    }

    cout << '\n';

    return cout;
}

std::ostream& operator<< (std::ostream& cout, Token token)
{
    auto start = token.start();
    auto end = token.end();

    // print_enum(cout, token.type()) << ' ';
    
    while (start != end)
        cout << *start++;

    cout << ' ';

    return cout;
}

std::ostream& Token::print (std::ostream& cout) const
{
    auto start = _start;
    auto end = _end;

    print_enum(cout, _type) << ' ';
    
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
    {
        if (*walker == '\n')
        {
            // move to next line
            cur_line_beg = walker+1;
            ++cur_line_id;
            source_lines[cur_line_id] = cur_line_beg;
        }

        ++walker;
    }
}
