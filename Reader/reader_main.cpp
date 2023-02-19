#include "Tokenizer.hpp"
#include "Parser.hpp"

void DFS(ParseNode* root)
{
    static int ident = 0;
    std::cout << '\n';

    if (root)
    {
        auto st  = root->childrenBegin();
        auto end = root->childrenEnd();

        int cur_ident = ident;
        while (cur_ident--)
            std::cout << "    ";
        
        std::cout << root << '\n';

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
    std::string test_str = "pine![12,24]{color = blue;size=5;}"
                           "star[1,2]{color=red;}"; //

    Tokenizer tokenizer{test_str};
    tokenizer.tokenize();
    std::cout << tokenizer;
    const std::deque<Token*> tokens = tokenizer.get_tokens();

    Parser parser{tokens};
    ParseNode* sketch = parser.getSketch();
    
    auto st  = sketch->childrenBegin();
    auto end = sketch->childrenEnd();

    sketch->print(std::cout, 0);

    // std::cout << parser;
    // parser.get_objects();

    return 0;
}
