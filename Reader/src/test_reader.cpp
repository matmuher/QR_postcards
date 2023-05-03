#include <gtest/gtest.h>
#include <TextProcessor.hpp>
#include <Objects.hpp>

void cmp_default_objects(const Object* res, const Object* test)
{
    ASSERT_EQ(res->x(), test->x());
    ASSERT_EQ(res->y(), test->y());
    ASSERT_EQ(res->color(), test->color());
    ASSERT_EQ(res->size(), test->size());
    ASSERT_EQ(res->type(), test->type());
}

void cmp_congrats(const Congratulation* res, const Congratulation* test)
{
    auto res_lines = res->get_lines();
    auto test_lines = test->get_lines();

    ASSERT_EQ(res_lines.size(), test_lines.size());

    for (size_t i = 0; i < res_lines.size(); ++i)
    {
        auto res_msg = res_lines[i].get_msg();
        auto test_msg = test_lines[i].get_msg();

        ASSERT_TRUE(res_msg == test_msg);
    }
}

static const int MOCK_ARGC = 2;

TEST(Reader, one_object)
{
    TextProcessor text_processor;
    const char* argv[] = {"", "../tests/1_one_object.txt"};

    text_processor.get_input(MOCK_ARGC, argv);
    text_processor.process();
    auto obj_list = text_processor.get_obj_list();
    
    std::vector<Object*> correct_objs;    
    Pine pine{50, 50, 2, ColorType::Red, 5};
    correct_objs.push_back(&pine);

    for (size_t i = 0; i < obj_list.size(); ++i)
    {
        cmp_default_objects(obj_list[i], correct_objs[i]);
    }
}

TEST(Reader, many_objects)
{
    TextProcessor text_processor;
    const char* argv[] = {"", "../tests/2_many_objects.txt"};

    text_processor.get_input(MOCK_ARGC, argv);
    text_processor.process();
    auto obj_list = text_processor.get_obj_list();
    
    std::vector<Object*> correct_objs;  

    Pine pine{50, 50, 2, ColorType::Red, 5};
    correct_objs.push_back(&pine);

    Star star{20, 70, 5, ColorType::Yellow, 0};
    correct_objs.push_back(&star);

    Gift gift{10, 80, 0, ColorType::Blue, 2};
    correct_objs.push_back(&gift);

    for (size_t i = 0; i < obj_list.size(); ++i)
    {
        cmp_default_objects(obj_list[i], correct_objs[i]);
    }
}

TEST(Reader, lex_error)
{
    TextProcessor text_processor;
    const char* argv[] = {"", "../tests/3_lex_error.txt"};

    text_processor.get_input(MOCK_ARGC, argv);
    text_processor.process();
    
    ASSERT_EQ(text_processor.get_error_type(), TextProcessor::ErrorType::LEX_ERROR);
}

TEST(Reader, syntax_error)
{
    TextProcessor text_processor;
    const char* argv[] = {"", "../tests/4_syntax_error.txt"};

    text_processor.get_input(MOCK_ARGC, argv);
    text_processor.process();
    
    ASSERT_EQ(text_processor.get_error_type(), TextProcessor::ErrorType::SYNTAX_ERROR);
}

TEST(Reader, congrats)
{
    TextProcessor text_processor;
    const char* argv[] = {"", "../tests/5_congrats.txt"};

    text_processor.get_input(MOCK_ARGC, argv);
    text_processor.process();
    auto obj_list = text_processor.get_obj_list();
    
    std::vector<Object*> correct_objs;    
    // size = 1, color = white, intense = 0 : default values
    Congratulation congrat{50, 50, 1, ColorType::White, 0};

    const std::string line1 = "We wish you a Merry Xmas"; 
    const std::string line2 = "And a happy New Year!"; 

    congrat.add_line(line1);
    congrat.add_line(line2);

    correct_objs.push_back(&congrat);

    for (size_t i = 0; i < obj_list.size(); ++i)
    {
        cmp_default_objects(obj_list[i], correct_objs[i]);
        cmp_congrats(   static_cast<Congratulation*>(obj_list[i]),
                        static_cast<Congratulation*>(correct_objs[i]));
    }
}
