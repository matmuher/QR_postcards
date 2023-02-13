#include "Tokenizer.hpp"

int main()
{
    std::string test_str = "pine![12,24]{color = blue;}"; //

    Tokenizer tokenizer{test_str};
    tokenizer.tokenize();
    std::cout << tokenizer;

    const std::deque<Token*> tokens = tokenizer.get_tokens();

    return 0;
}