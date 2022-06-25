typedef struct {

} parser_state;

parser_state parser_state_new(void);

void parse_state_free(parser_state *parser_state);
