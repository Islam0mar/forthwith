
#define FW_CUSTOM_TOB_FLUSH
#define FW_TRACE
#undef FORTHWITH_NO_CHECKS
#define FORTHWITH_NO_CHECKS
#define FW_CUSTOM_DICT_ADD

#include "forthwith-pru.h"
#include "forthwith.h"
#include "dict.h"
#include "utilities.h"

// Define Primitives, including function bodies, etc
#define FORTH_DEFINE_PRIMITIVES
  #include "xmacros.h"
  #include "xmacros.inner.h"
  #include "xmacros.core.h"
  #include "xmacros.outer.h"
#undef FORTH_DEFINE_PRIMITIVES

#include "dict.c"
#include "forthwith.c"
#include "utilities.c"
/* #include "prompt.c" */


extern void forth_flush_tob();

__fw_noinline__
void dict_add(fword_t *entry) {
  // Load dictionary pointer
  fcell_xt* body = (fcell_xt*) entry->body;
  fword_t *ne = dict_create(entry->meta, entry->len, entry->name, body);
}
