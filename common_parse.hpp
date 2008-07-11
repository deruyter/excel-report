
#include <stdlib.h>
#include <data.hpp>

extern int warn_level;

extern int yy1lex();
extern int yy2lex();
extern int yy3lex();
extern int yy4lex();
extern int yy1error(char *s);
extern int yy2error(char *s);
extern int yy3error(char *s);
extern int yy4error(char *s);
extern int yy1parse();
extern int yy2parse();
extern int yy3parse();
extern int yy4parse();
extern int parse_info(char *file_name);
extern int parse_size(char *file_name, int disc);
extern int parse_cycle(char *file_name);
extern int parse_fail(char *file_name);

extern int disc_set;
extern TestSession *current_session;
