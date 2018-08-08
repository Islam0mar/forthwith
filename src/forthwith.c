#include <stdlib.h>

#include "forthwith.h"


// TODO: fix implicit global...
struct forthwith_context context;

fw_ctx_t *ctx;

#define global_state ctx->var_head

#include <stdio.h>
#include <string.h>

IP_t *forth_alloc_var(fw_ctx_t* ctx) {
  // this incrs in multipls of IP_t size
  ctx->user_head = ctx->user_head + 1;
  return ctx->user_head;
}

int forth_init() {
  uint8_t cell_sz = sizeof(fcell_t);

  struct forthwith_context nctx = {
    .psp_size =  128 * cell_sz,
    .rsp_size =  128 * cell_sz,
    .user_size =   64 * cell_sz,
    .dict_size = 512 * cell_sz,
  };

  nctx.psp_base = nctx.psp_head = calloc(1, nctx.psp_size);
  nctx.rsp_base = nctx.rsp_head = calloc(1, nctx.rsp_size);
  nctx.user_base = nctx.user_head = calloc(1, nctx.user_size);
  nctx.dict_base = nctx.dict_head = calloc(1, nctx.dict_size);

  printf("psp base: %p\n", nctx.psp_base);
  printf("rsp base: %p\n", nctx.rsp_base);
  printf("user base: %p\n", nctx.user_base);
  printf("dict base: %p\n", nctx.dict_base);

  context = nctx;
  ctx = &context;

  return -1;
}

#define ADDR(x) ((IP_t)&x)

__attribute__ ((noinline)) int forth_push(fw_ctx_t* ctx, fcell_t val) {
  if (ctx->psp_head <= ctx->psp_base + ctx->psp_size) {
    *ctx->psp_head = val;
    ctx->psp_head++;
    return 1;
  }
  else
    return -1;
}

__attribute__ ((noinline)) fcell_t *forth_pop(fw_ctx_t* ctx) {
  if (ctx->psp_head > ctx->psp_base) {
    ctx->psp_head--;
    return ctx->psp_head;
  }
  else
    return NULL;
}

int forth_bootstrap(fw_ctx_t* ctx) {
  /* fcell_t* user_head = ctx->user_head; */

  /* IP_t test[] = [ */
  /*                   ADDR(lit), */
  /*                   ADDR(doconst), */
  /*                   ]; */

  /* dict_create */
  return -1;
}

__attribute__ ((noinline))
fw_call forth_exec(FORTH_REGISTERS) {

  // ...
  copy_reg(psp, tos);

  popd(u);
  popd(rsp);
  popd(ip);
  popd(x);
  popd(tos);
  popd(w);

  /* __jump(next); */
  jump(next);
}

__attribute__ ((noinline)) int forth_eval(IP_t *instr) {

  forth_push(ctx, 9);
  forth_push(ctx, 11);

  fcell_t *tosptr = forth_pop(ctx);
  printf("context: tos: %p %lld \n", tosptr, tosptr == NULL ? 0 : *tosptr);

  forth_push(ctx, /* w */ (fcell_t)instr);
  forth_push(ctx, /* tos */ 10);
  forth_push(ctx, /* x */ 0);
  forth_push(ctx, /* ip */ (fcell_t)instr+8);
  forth_push(ctx, /* rsp */ (fcell_t)ctx->rsp_head);
  forth_push(ctx, /* u */ (fcell_t)ctx);

  fcell_t p = (fcell_t)ctx->psp_head;

  printf("context: psp_head: %p (%p)\n", ctx->psp_head, ctx->psp_base);
  printf("context: rsp_head: %p (%p)\n", ctx->rsp_head, ctx->rsp_base);
  printf("context: instr: %p \n", instr);
  printf("context: ctx: %p \n", ctx);
  forth_exec(0, p, 0, 0, 0, 0);

  /* ctx->psp_head = p; */
  save_psp(psp);

  return 0;
}

