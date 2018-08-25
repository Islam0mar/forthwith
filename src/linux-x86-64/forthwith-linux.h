
#ifndef __HEADER_IMPL_X86__
#define __HEADER_IMPL_X86__

#include "forthwith-linux-consts.h"
#include "forthwith.h"

#include <stddef.h>

// ========================================================================== //
// Platform "Helpers"
// ========================================================================== //

/*
  e.g. macro hackery. pretty sure there's some incantations to Cthululu somewhere in here... but not too much worse (IMHO) compared to asm macro's, much less 20 different asm macro syntaxes
*/

/* #define fw_label(l) _fw_label(l) */
#define _asm_jump() \
  __asm__ ("" :: "r" (ip));         \
  __asm__ ("" :: "r" (psp));        \
  __asm__ ("" :: "r" (rsp));        \
  __asm__ ("" :: "r" (bpsp));        \
  __asm__ ("" :: "r" (brsp));        \
  __asm__ ("" :: "r" (x))


// Define some specific jumps, by linux, this should support most unix-likes or proper unixes
#ifdef __MACH__
#define __jump(r) __asm__("jmp " "_" #r)
#define __label(r) _ ## r
#elif __linux__
#define __jump(r) __asm__("jmp " #r)
#define __label(r) r
#else
#define __jump(r) __asm__("jmp " #r)
#define __label(r) r
#endif

#define __call(r) __asm__("callq " #r)
#define _call(r) __call(r)

#define _jump(r) __jump( r )
#define ___jump_reg(r) __asm__(r); 

#define __jump_reg(r) ___jump_reg( "jmpq *" #r )
#define _jump_reg(r, x) __jump_reg( r )

#define _fw_asm(r, a, x, b, c, y, d) __asm__(r " " a #x b "," c #y d)

#define _fw_asm_to_addr(c, x, y) _fw_asm(c, "", x, "", "(", y, ")")
#define _fw_asm_from_addr(c, x, y) _fw_asm(c, "(", x, ")", "", y, "")
#define _fw_asm_const(c, x, y) _fw_asm(c, "", x, "", "", y, "")
#define _fw_asm_to_addr_off(c, x, y, o) _fw_asm(c, "", x, "", #o "(", y, ")")
#define _fw_asm_from_addr_off(c, x, y, o) _fw_asm(c, #o "(", x, ")", "", y, "")

#define __fw_asm_single(c, x) __asm__( c " " #x)
#define _fw_asm_single(c, x) __fw_asm_single( c, x)

// ========================================================================== //
// Standard "Definitions"
// ========================================================================== //

/* The forth impl interfaces */
#define jump_reg(r) _jump_reg( reg_ ## r, __jump_reg )
#define jump(reg) _jump( reg ); _asm_jump()

#define load_const(x, y) _fw_asm_const("movq", y, reg_##x)
#define load_addr(x, y) _fw_asm_from_addr("movq", reg_##y, reg_##x)
#define store_addr(x, y) _fw_asm_to_addr("movq", reg_##y, reg_##x)

#define load_addr_byte(x, y) _fw_asm_from_addr("movb", reg_##y, reg_##x)
#define store_addr_byte(x, y) _fw_asm_to_addr("movb", reg_##y, reg_##x)

#define load_addr_off(x, y, o) _fw_asm_from_addr_off("movq", reg_##y, reg_##x, o)
#define store_addr_off(x, y, o) _fw_asm_to_addr_off("movq", reg_##y, reg_##x, o)
#define calc_addr_off(x, y, o) _fw_asm_from_addr_off("leaq", reg_##y, reg_##x, o)

// Incr & Decr
#define add_const(x, y) _fw_asm_const("addq", y, reg_##x)
#define sub_const(x, y) _fw_asm_const("subq", y, reg_##x)

// Bitwise
#define xor_reg(x, y) _fw_asm_const("xorq", reg_##y, reg_##x)
#define and_reg(x, y) _fw_asm_const("andq", reg_##y, reg_##x)
#define or_reg(x, y) _fw_asm_const("orq", reg_##y, reg_##x)

#define lshift_reg(x, y) copy_reg(xrcx, y); _fw_asm_const("shlq", reg_xcl, reg_##x)
#define rshift_reg(x, y) copy_reg(xrcx, y); _fw_asm_const("shrq", reg_xcl, reg_##x)

#define not_reg(y) _fw_asm_const("notq", reg_##y, "")
#define not_reg(y) _fw_asm_const("notq", reg_##y, "")

// Jumps
// ... unique labels? hmmm...
// -- gcc/clang seem to handle read-only if/else branches fine

// Signed Arithmetic
#define adds_reg(x, y) _fw_asm_const("addq", reg_##y, reg_##x)
#define addu_reg(x, y) _fw_asm_const("addq", reg_##y, reg_##x)

#define subu_reg(x, y) _fw_asm_const("subq", reg_##y, reg_##x)
#define subs_reg(x, y) _fw_asm_const("subq", reg_##y, reg_##x)

#define muls_reg(x, y) copy_reg(xrax, x); _fw_asm_single("imulq", reg_##y); copy_reg(x, xrax)
#define divs_reg(x, y) copy_reg(xrax, x); _fw_asm_single("idivq", reg_##y); copy_reg(x, xrax)

#define copy_reg(x, y) _fw_asm_const("movq", reg_##y, reg_##x)
#define incr_reg(reg) add_const(reg, $word_sz)
#define decr_reg(reg) sub_const(reg, $word_sz)

#define _pushd_cell(reg) store_addr(psp, reg); add_const(psp, $word_sz)
#define _popd_cell(reg)  sub_const(psp, $word_sz); load_addr(reg, psp)

#define _pushd_0
#define _pushd_1 _pushd_cell(s1);
#define _pushd_2 _pushd_cell(s2); _pushd_1;
#define _pushd_3 _pushd_cell(s3); _pushd_2;
#define _pushd_4 _pushd_cell(s4); _pushd_3;
#define _pushd(n) _pushd_ ## n

#define _popd_0
#define _popd_1          _popd_cell(s1)
#define _popd_2 _popd_1; _popd_cell(s2)
#define _popd_3 _popd_2; _popd_cell(s3)
#define _popd_4 _popd_3; _popd_cell(s4)
#define _popd(n) _popd_ ## n

#define _pushr(reg) store_addr(rsp, reg); add_const(rsp, $word_sz)
#define _popr(reg)  sub_const(rsp, $word_sz); load_addr(reg, rsp)

#define _pushu(reg) store_addr(u, reg); add_const(u, $word_sz)
#define _popu(reg)  sub_const(u, $word_sz); load_addr(reg, u)

#define save_psp(reg) \
  load_const(xrax, $ctx_psp);                   \
  store_addr_off(xrax, reg, $stack_of_head) // size

#define load_psp(reg) \
  load_const(xrax, $ctx_psp);                   \
  load_addr_off(reg, xrax, $stack_of_head) // sizeof one word

#define call(lbl) _call( __label(lbl) )

#define __call_reg(r) __asm__("callq *" #r )
#define _call_reg(r) __call_reg( r )
#define call_reg(r) _call_reg( reg_ ## r )

#define ret(reg)                                 \
  __asm__("ret");


#define _checkd psp < bpsp
#define _checkr rsp < brsp

#endif // __HEADER_IMPL_X86__

