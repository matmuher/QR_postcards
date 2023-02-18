#include "Tokenizer.hpp"
#include "Parser.hpp"

void DFS(ParseNode* root)
{
    static int ident = 0;
    std::cout << "meow" << '\n';

    if (root)
    {
        auto st  = root->childrenBegin();
        auto end = root->childrenEnd();

        int cur_ident = ident;
        while (cur_ident--)
            std::cout << "    ";
        
        if (root->parse_type() == ParseType::TokenType)
            std::cout << root->token_type();
        else
            std::cout << root->parse_type();

        std::cout << '\n';

        while (st != end) 
        {   
            ident++;
            DFS(*st++);
            ident--;
        }
    }
}

int main()
{
    std::string test_str = "pine![12,24]{color = blue;}"; //

    Tokenizer tokenizer{test_str};
    tokenizer.tokenize();
    std::cout << tokenizer;
    const std::deque<Token*> tokens = tokenizer.get_tokens();

    Parser parser{tokens};
    ParseNode* sketch = parser.getSketch();
    std::cout << "wtf\n";
    DFS(sketch);
    // std::cout << parser;
    // parser.get_objects();

    return 0;
}