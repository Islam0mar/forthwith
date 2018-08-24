

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "forthwith.h"

#define accessor(struct_name, var_name) \
  __fw_noinline__ \
  fcell_t accessor_ ## struct_name ## _st_ ## var_name(struct_name *t) { return (fcell_t)t->var_name; }

accessor(fw_ctx_regs_t, w);
accessor(fw_ctx_regs_t, x);
accessor(fw_ctx_regs_t, ip);

accessor(fw_ctx_vars_t, state);
accessor(fw_ctx_vars_t, error);
accessor(fw_ctx_vars_t, tib_idx);
accessor(fw_ctx_vars_t, tib_len);
accessor(fw_ctx_vars_t, tib_str);
accessor(fw_ctx_vars_t, tob_idx);
accessor(fw_ctx_vars_t, tob_len);
accessor(fw_ctx_vars_t, tob_str);

accessor(fw_ctx_stack_t, head);
accessor(fw_ctx_stack_t, base);
accessor(fw_ctx_stack_t, size);

accessor(fw_ctx_t, regs);
accessor(fw_ctx_t, vars);
accessor(fw_ctx_t, psp);
accessor(fw_ctx_t, rsp);
accessor(fw_ctx_t, user);
accessor(fw_ctx_t, dict);
accessor(fw_ctx_t, strings);

#undef accessor
#define accessor(struct_name, var_name)                                 \
  printf(#struct_name " :: " #var_name " -> %ld \n", \
         accessor_ ## struct_name ## _st_ ## var_name(var_ptr_ ## struct_name)); \

fw_ctx_regs_t var_fw_ctx_regs_t = { 1, 2, 3 };
fw_ctx_vars_t var_fw_ctx_vars_t = { 1, 2, 3, 4, (char*)5, 6, 7, (char*)8 };
fw_ctx_stack_t var_fw_ctx_stack_t = { (fcell_xt)1, (fcell_xt)2, 3 };
fw_ctx_t var_fw_ctx_t = { (void*)1, (void*)2, (void*)3, (void*)4, (void*)5, (void*)6, (void*)7};

__fw_noinline__
void examples_accessors_regs() {
  fw_ctx_regs_t *var_ptr_fw_ctx_regs_t = &var_fw_ctx_regs_t; 
  accessor(fw_ctx_regs_t, w);
  accessor(fw_ctx_regs_t, x);
  accessor(fw_ctx_regs_t, ip);
}

__fw_noinline__
void examples_accessors_vars() {
  fw_ctx_vars_t *var_ptr_fw_ctx_vars_t = &var_fw_ctx_vars_t; 
  accessor(fw_ctx_vars_t, state);
  accessor(fw_ctx_vars_t, error);
  accessor(fw_ctx_vars_t, tib_idx);
  accessor(fw_ctx_vars_t, tib_len);
  accessor(fw_ctx_vars_t, tib_str);
  accessor(fw_ctx_vars_t, tob_idx);
  accessor(fw_ctx_vars_t, tob_len);
  accessor(fw_ctx_vars_t, tob_str);
}

__fw_noinline__
void examples_accessors_stack() {
  fw_ctx_stack_t *var_ptr_fw_ctx_stack_t = &var_fw_ctx_stack_t; 
  accessor(fw_ctx_stack_t, head);
  accessor(fw_ctx_stack_t, base);
  accessor(fw_ctx_stack_t, size);
}

__fw_noinline__
void examples_accessors_ctx() {
  fw_ctx_t *var_ptr_fw_ctx_t = &var_fw_ctx_t; 
  accessor(fw_ctx_t, regs);
  accessor(fw_ctx_t, vars);
  accessor(fw_ctx_t, psp);
  accessor(fw_ctx_t, rsp);
  accessor(fw_ctx_t, user);
  accessor(fw_ctx_t, dict);
  accessor(fw_ctx_t, strings);
}

// Bitwise //
#define binary_ops(opname)                                 \
  printf(#opname "  -> %ld \n", ex_ ## opname(14, 2)); \
  printf(#opname "  -> %ld \n", ex_ ## opname(9, 1));


__fw_noinline__
fcell_t ex_lshiftc(fcell_t a, fcell_t b) {
  return a << b;
}

__fw_noinline__
fcell_t ex_rshiftc(fcell_t a, fcell_t b) {
  return a >> b;
}

__fw_noinline__
fcell_t ex_lshiftu(uint32_t a, uint32_t b) {
  return a << b;
}

__fw_noinline__
fcell_t ex_rshiftu(uint32_t a, uint32_t b) {
  return a >> b;
}

__fw_noinline__
fcell_t ex_mul(fcell_t a, fcell_t b) {
  return a * b;
}

__fw_noinline__
fcell_t ex_div(fcell_t a, fcell_t b) {
  return a * b;
}

__fw_noinline__
void examples_bitwise() {
  binary_ops(lshiftc);
  binary_ops(rshiftc);
  binary_ops(lshiftu);
  binary_ops(rshiftu);
  binary_ops(mul);
  binary_ops(div);
}

__fw_noinline__ fw_call call_reg_bpsp() {printf("bpsp\n");}
__fw_noinline__ fw_call call_reg_psp() {printf("psp\n");}
__fw_noinline__ fw_call call_reg_brsp() {printf("brsp\n");}
__fw_noinline__ fw_call call_reg_rsp() {printf("rsp\n");}
__fw_noinline__ fw_call call_reg_ip() {printf("ip\n");}
__fw_noinline__ fw_call call_reg_x() {printf("x\n");}

fw_call examples_call(FORTH_REGISTERS) {
  if (bpsp) { call_reg_bpsp(); }
  if (psp) { call_reg_psp(); }

  if (brsp) { call_reg_brsp(); }
  if (rsp) { call_reg_rsp(); }

  if (ip) { call_reg_ip(); }
  if (x) { call_reg_x(); }
}

int main(int argv, char **argc) {
  
  examples_accessors_regs();
  examples_accessors_vars();
  examples_accessors_stack();
  examples_accessors_ctx();

  examples_bitwise();
}
