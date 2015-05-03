#ifndef YYINPUT_H
#define YYINPUT_H

#define YY_INPUT(buf,len,max) \
  int rl_num_chars_to_read=max-1; \
  char *s=readline(0);        \
  if (s) {                    \
    add_history(s);           \
    len=strlen(s);            \
    strcpy(buf,s);            \
    buf[len++]='\n';          \
    free(s);                  \
  } else len=0;

#endif /* YYINPUT_H */
