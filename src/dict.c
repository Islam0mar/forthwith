
#include "forthwith.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

fcell_xt *forth_alloc_var_len(fcell_t len) {
  // this incrs in multipls of IP_t size
  ctx->user->head = ctx->user->head + len;
  return (fcell_xt *)ctx->user->head;
}
fcell_xt *forth_alloc_var() {
  return (fcell_xt *)forth_alloc_var_len(1);
}

fword_t* alloc_dict() {
  // 'allocate' new entry in dict head
  fword_t* curr_dict = ctx->dict->head;
  fword_t* next_dict = ctx->dict->head + 1;

  // update dict head
  next_dict->prev = curr_dict;
  ctx->dict->head = next_dict;

  return curr_dict;
}

char* alloc_string(uint8_t len) {
  // 'allocate' new entry in dict head
  char* curr_string = ctx->strings->head;
  char* next_string = ctx->strings->head + len + 1; // for extra null term and len

  // update dict head
  ctx->strings->head = next_string;
  curr_string[len] = '\0'; // just in case 

  return curr_string;
}

fword_t* dict_create(uint8_t mask, uint8_t len, char *name) {

  fword_t *entry = alloc_dict();

  entry->meta = mask;
  entry->len = len;
  entry->name = alloc_string(len);

  memcpy(entry->name, name, len);

  return entry;
}

/* FIND (name? – address). */
fword_t* dict_find(int8_t len, char *name) {
  // Load dictionary pointer
  fword_t* word_ptr = ctx->dict->head;

  // Iterate over words, looking for match
  while (word_ptr != NULL) {
    fword_t word = *word_ptr;
    if (word.len == len) {
      int8_t i;
      for (i = 0; i < len; i++) {
        if (name[i] != word.name[i])
          break;
      }
      // word found
      if (i == len)
        return word_ptr;
    }

    // no match
    word_ptr = word.prev;
  }
  return NULL;
}

static bool is_whitespace(char c) {
  return (c == '\0') | (c == ' ') | (c == '\t') | (c == '\r') | (c == '\n');
}

uint8_t parse_word(uint8_t idx, uint8_t len, char *tib) {

  uint8_t word_start;
  char c;

whitespace:
  while ((c = tib[idx]) && ((idx < len) & is_whitespace(c)))
    idx++;

  /* backcomment: */
  if (c == '\\') {
    while ((c = tib[idx]) && ((idx < len) & (c != '\n')))
      idx++;
    goto whitespace;
  }

  /* parcomment: */
  if (c == '(') {
    while ((c = tib[idx]) && ((idx < len) & (c != ')')))
      idx++;
    goto whitespace;
  }

  goto word;

word:
  word_start = idx++;
  while ((c = tib[idx]) && ((idx < len) & !is_whitespace(c)))
    idx++;

  printf("idx: i: %u ws: %u of %u\n", idx, word_start, len);
  printf("   ---  %p\n", tib[word_start]);
  printf("   ---  %s\n", tib + word_start);

  // push word addr
  char *word_head = tib + word_start;
  forth_push( (fcell_t)word_head );

  // push word count
  forth_push(idx - word_start);

  return idx;
}

const char num_basis[] = "-0123456789ABCDEF";


void parse_number(uint8_t base, uint8_t len, char *addr) {

  uint8_t idx = 0;
  bool err = false;
  fcell_t num = 0;
  char c;

  while ((c = addr[idx]) && ((idx < len) & !err)) {
    err = true;
    for (int i = 0; i < base; i++) {
      if (num_basis[i] == c) {
        num = base * num + i - 1;
        err = false;
        break;
      }
    }
  }

  forth_push(num);
  forth_push((uint8_t)err);
}
