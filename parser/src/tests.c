/*
 * File: tests.c
 * Author: Matthew Nappo
 * Another main method that runs some tests I wrote during
 * the development process. Unrelated to the actual assignment.
 */
#ifdef TEST

#include <stdlib.h>
#include <stdio.h>
#include "parse_tree.h"
#include "rd_parser.h"
#include "stack.h"
#include "grammar.h"
#include "parse_table.h"

void test_parse_tree_simpler()
{
    ParseTree t = make_2child("P", make_leaf("Q"), make_leaf("R"));
    tree_print(t);
    tree_free(t);

    ParseTree r = right_most_child(t);
    tree_print(r);
}

void test_leftmost()
{
    //ParseTree t = make_2child("PP", make_1child("QQ", make_leaf("X")), make_2child("R", make_leaf("A"), make_leaf("BB")));
    ParseTree t = make_2child("PP", make_leaf("QQ"), make_2child("RR", make_leaf("AA"), make_leaf("BB")));
    tree_print(t);

    ParseTree l;
    l = left_most_category_leaf(t, make_leaf("X"), true);
    tree_print(l);
    //add_child(l, make_leaf("X"));
    tree_print(t);

    l = left_most_category_leaf(t, make_leaf("Y"), true);
    tree_print(l);
    //add_child(l, make_leaf("Y"));
    tree_print(t);

    l = left_most_category_leaf(t, make_leaf("YY"), true);
    tree_print(l);
    //add_child(l, make_leaf("Y"));
    tree_print(t);

    tree_free(t);
}

void test_leftmost2()
{
    //ParseTree t = make_2child("PP", make_1child("QQ", make_leaf("X")), make_2child("R", make_leaf("A"), make_leaf("BB")));
    ParseTree t = make_2child("PP", make_leaf("Q"), make_2child("RR", make_leaf("AA"), make_leaf("BB")));
    tree_print(t);

    ParseTree l;
    l = left_most_category_leaf(t, NULL, false);
    if (l == NULL) printf("L NULL 1\n");
    tree_print(l);
    //add_child(l, make_leaf("X"));
    tree_print(t);

    l = left_most_category_leaf(t, NULL, false);
    if (l == NULL) printf("L NULL 2\n");
    tree_print(l);
    //add_child(l, make_leaf("Y"));
    tree_print(t);

    l = left_most_category_leaf(t, NULL, false);
    if (l == NULL) printf("L NULL 3\n");
    tree_print(l);
    //add_child(l, make_leaf("Y"));
    tree_print(t);

    tree_free(t);
}

void test_leftmost3()
{
    ParseTree t = make_2child("Expr", make_1child("Atomic", make_leaf("Set")), make_leaf("ExprT"));

    ParseTree l = left_most_category_leaf(t, make_leaf("X"), true);

    tree_print(l);
    tree_print(t);
}

void test_parse_tree()
{
    ParseTree l1 = make_leaf("C");
    ParseTree l2 = make_leaf("B");
    ParseTree t2 = make_2child("X", l1, l2);
    ParseTree t3 = make_3child("L", make_leaf("M"), t2, make_leaf("Z"));
    ParseTree t = make_2child("Q", t3, make_leaf("S"));
    tree_print(t);
    tree_free(t);
}

void test_parse_tree_child()
{
    ParseTree l1 = make_leaf("C");
    ParseTree l2 = make_leaf("B");
    ParseTree t = make_2child("X", l1, l2);

    tree_print(t);

    ParseTree x1 = make_leaf("x 1");
    ParseTree x2 = make_2child("X 2", make_leaf("x 2.1"), make_leaf("x 2.2"));
    ParseTree x3 = make_leaf("x 3");

    add_child(t, x1);
    add_child(t, x2);
    add_child(t, x3);

    tree_print(t);

    tree_free(t);
}

void test_number()
{
    char *s = "1215";
    ParseTree n = number(&s);
    tree_print(n);
}

void test_elements()
{
    ParseTree e;
    char *s1 = "1,234,56";
    char *s2 = "1,2";
    e = elements(&s1);
    tree_print(e);
    e = elements(&s2);
    tree_print(e);
}

void test_element()
{
    ParseTree e;
    char *s1 = "123";
    char *s2 = "8";
    char *s3 = "9765";

    e = element(&s1);
    tree_print(e);
    e = element(&s2);
    tree_print(e);
    e = element(&s3);
    tree_print(e);
}

void test_set_tail()
{
    ParseTree st;
    char *s1 = "}";
    char *s2 = "2,4}";
    st = set_tail(&s1);
    tree_print(st);
    st = set_tail(&s2);
    tree_print(st);
}

void test_set() {
    ParseTree s;
    char *s1 = "{123,43,125}";
    char *s2 = "{}";
    s = set(&s1);
    tree_print(s);
    s = set(&s2);
    tree_print(s);
}

void test_expr_(ParseTree e, char *s)
{
    e = expr(&s);
    tree_print(e);
}

void test_expr()
{
    ParseTree e = NULL;
    char *s[] = {
        "{}", "{1,2}", "({})", "({5,6})", "(({5,6})U{1,2})^({}U{42})",
        "{}", "{1}", "{1,2}", "{1}U{1,2}", "({1}U{2})^({2}U{3})"
    };
    for (int i = 0; i < sizeof(s)/sizeof(s[0]); i++) {
        test_expr_(e, s[i]);
    }
}

struct test {
    int a;
    char *s;
};

typedef struct test* T;

T new_T(int a, char *s)
{
    T t = malloc(sizeof(struct test));
    t->a = a;
    t->s = s;
    return t;
}

void print_T(void *tp)
{
    T t = (T) tp;
    printf("T { a: %d, s: %s }\n", t->a, t->s);
}

void free_T(void *tp)
{
    T t = (T) tp;
    //free(t->s); // ? they're literals...
    free(t);
}

void test_stack()
{
    Stack s = init_stack();

    T t1 = new_T(10, "apple");
    T t2 = new_T(31, "pears");
    T t3 = new_T(18, "grape");

    push(s, t1);
    push(s, t2);
    push(s, t3);

    print_stack(s, print_T);

    T tx = (T)pop(s);
    printf("popped:"); print_T(tx);
    free_T(tx);

    print_stack(s, print_T);

    printf("is_empty: %d\n", is_empty(s));

    free_stack(s, free_T);
}

#define PT_TEST(CATG, TERM)                                \
    do {                                                   \
     printf("%s | %c : ", CATG, TERM);                     \
     print_production(parsetable_lookup(pt, CATG, TERM));  \
    } while (0);

void test_parsetable()
{
    Grammar g = build_set_algebra_grammar();

    ParseTable pt = build_set_algebra_parsetable(g);

    PT_TEST("Expr",   '{');
    PT_TEST("Expr",   '(');
    PT_TEST("ExprT",  '^');
    PT_TEST("ExprT",  'U');
    PT_TEST("Atomic", '{');
    PT_TEST("Atomic", '(');
    PT_TEST("Set",    '{');
    PT_TEST("SetT",   '}');

    PT_TEST("Expr",   '{');
    PT_TEST("Expr",   '(');

    PT_TEST("ExprT",  '^');
    PT_TEST("ExprT",  'U');

    PT_TEST("Atomic", '{');
    PT_TEST("Atomic", '(');

    PT_TEST("Set",    '{');

    PT_TEST("SetT",   '}');

    PT_TEST("SetT",   '0');
    PT_TEST("SetT",   '9');

    PT_TEST("Elmts",  '0');
    PT_TEST("Elmts",  '9');

    PT_TEST("ElmtsT", ',');

    PT_TEST("Elmt",   '0');
    PT_TEST("Elmt",   '9');

    PT_TEST("Num",    '0');
    PT_TEST("Num",    '9');
    PT_TEST("NumT",   '0');
    PT_TEST("NumT",   '9');

    PT_TEST("Dig",   '0');
    PT_TEST("Dig",   '1');
    PT_TEST("Dig",   '2');
    PT_TEST("Dig",   '3');
    PT_TEST("Dig",   '4');
    PT_TEST("Dig",   '5');
    PT_TEST("Dig",   '6');
    PT_TEST("Dig",   '7');
    PT_TEST("Dig",   '8');
    PT_TEST("Dig",   '9');

    PT_TEST("ExprT",   EPSILON);
    /*
    PT_TEST("Dig",   '8');
    PT_TEST("Dig",   '9');
    */
    parsetable_print(pt);
}

int main(int argc, char *args[])
{
    //test_parse_tree_child();
    //test_equality();
    //test_parsetable();
    //test_stack();
    //test_parse_tree_simpler();
    //test_leftmost();
    test_leftmost2();
    test_leftmost3();
    //test_parse_tree();
    //test_number();
    //test_element();
    //test_elements();
    //test_set_tail();
    //test_set();
    //test_expr();

    return 0;
}

#endif
