#ifndef Py_PARSER_H
#define Py_PARSER_H
#ifdef __cplusplus
extern "C" {
#endif


/* Parser interface */

#define MAXSTACK 500

typedef struct {
	int		 s_state;	/* State in current DFA */
	dfa		*s_dfa;		/* Current DFA */
	struct _node	*s_parent;	/* Where to add next node */
} stackentry;

typedef struct {
	stackentry	*s_top;		/* Top entry */
	stackentry	 s_base[MAXSTACK];/* Array of stack entries */
					/* NB The stack grows down */
} stack;

typedef struct {
	stack	 	p_stack;	/* Stack of parser states */
	grammar		*p_grammar;	/* Grammar to use */
	node		*p_tree;	/* Top of parse tree */
	int		p_generators;	/* 1 if yield is a keyword */
} parser_state;

parser_state *PyParser_New(grammar *g, int start);
void PyParser_Delete(parser_state *ps);
int PyParser_AddToken(parser_state *ps, int type, char *str, int lineno,
                      int *expected_ret);
void PyGrammar_AddAccelerators(grammar *g);

#ifdef __cplusplus
}
#endif
#endif /* !Py_PARSER_H */