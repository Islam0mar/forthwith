
#ifndef _FW_DICT_H_
#define _FW_DICT_H_

fcell_xt *forth_alloc_var();
fcell_xt *forth_alloc_var_len(fcell_t len);

fword_t* alloc_dict();
char* alloc_string(uint8_t len);

// Use data stack
void parse_word(uint8_t idx, uint8_t len, char *tib);
void parse_number(uint8_t base, uint8_t len, char *addr);

// Helpers 
fword_t* dict_create(uint8_t mask, uint8_t len, char *name);
fword_t* dict_find(int8_t len, char *name);

#endif // _FW_DICT_H_
