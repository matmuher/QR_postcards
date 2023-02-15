#include "Tokenizer.hpp"
#include "Parser.hpp"

int main()
{
    std::string test_str = "pine![12,24]{color = blue;}"; //

    Tokenizer tokenizer{test_str};
    tokenizer.tokenize();
    std::cout << tokenizer;
    const std::deque<Token*> tokens = tokenizer.get_tokens();

    Parser parser{tokens};
    ParseTree* sketch = parser.getSketch();
    // std::cout << parser;
    // parser.get_objects();

    return 0;
}