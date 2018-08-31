
forth_core("'", 1, F_NORMAL, tick, "( -- n )", {
    popd(0);
    // Get address of next word from codeword list (e.g. same as lit)
    load_addr(x, ip);
    incr_reg(ip);
    // push on stack
    copy_reg(s1, x);
    pushd(1);
    jump(next);
  });

/*	STATE		Is the interpreter executing code (0) or compiling a word (non-zero)? */
forth_variable(STATE, 5, ctx_vars, state);
forth_variable(HERE, 4, ctx_user, head); // get pointer to current user word location

// Follows Jone's Forth here
forth_docall("create", 6, F_NORMAL, create, "( n -- )", docreate);
forth_docall(",", 1, F_NORMAL, comma, "( n -- )", docomma);
forth_docall("[", 1, F_IMMED, lbrac, "( n -- )", dolbrac);
forth_docall("]", 1, F_NORMAL, rbrac, "( n -- )", dorbrac);
forth_docall("xmask", 5, F_NORMAL, xmask, "( n -- )", doxmask);

forth_docall("word", 4, F_NORMAL, word, "( -- )", doword);
forth_docall("number", 6, F_NORMAL, number, "( c n -- n )", donumber);
forth_docall("find", 4, F_NORMAL, find, "( c n -- )", dofind);
forth_docall("cfa", 3, F_NORMAL, cfa, "( n -- )", docfa);

forth_docall("emit", 4, F_NORMAL, emit, "( n -- )", doemit);
forth_docall("err", 3, F_NORMAL, err, "( n -- )", doerr);

// Toggle hidden flag -- unhide the word
forth_word("immed", 5, F_IMMED, immed, "( p -- )",
           XT(tick), (fcell_xt)F_IMMED, XT(xmask),
           );

/* Executes word on tos */
forth_core("exec", 4, F_NORMAL, exec, "( n -- )", {
  popd(1);
  copy_reg(x, s1);
  pushd(0);
  jump_reg(x);
});

/* Increments the IP by offset to affect branching */
forth_core("branch", 6, F_NORMAL, branch, "{offset} ( -- )", {
    popd(0);
    load_addr(x, ip);
    addu_reg(ip, x);
    pushd(0);
    jump(next);
});

/* Increments the IP by offset to affect branching */
forth_core("0branch", 7, F_NORMAL, zbranch, "{offset} ( n -- )", {
    // load stack
    popd(1);
    /* copy_reg(x, s1); */

    jump_ifzero(s1, branch);
    /* add_const(ip, $word_ptr_sz); */
    incr_reg(ip);

    pushd(0);
    jump(next);
});

// Forth Words in Forth (pihsnoipmahc FWW!)
forth_word(":", 1, F_NORMAL, colon, "( p -- )",
           XT(word), // Get the name of the new word
           XT(zbranch),
              XCELLS(6),
              XT(create), // CREATE the dictionary entry / header
              XT(tick),
              *(fcell_xt*)XT(docolon),
                XT(comma), // Append DOCOLON (the codeword).
              XT(rbrac), // Go into compile mode.
           XT(semi), // Return from the function.
           );

forth_word(";", 1, F_NORMAL | F_IMMED, semicolon, "( p -- )",
           XT(tick),
              XT(semi),
                XT(comma), // Append EXIT (so the word will return).

           XT(tick),
              (fcell_xt)F_HIDDEN,
              XT(xmask), // Toggle hidden flag -- unhide the word

           XT(tick),
              (fcell_xt)F_WORD,
              XT(xmask), // Toggle hidden flag -- unhide the word

           XT(lbrac), // Go back to IMMEDIATE mode.
           XT(semi), // Return from the function.
           );


/*
  Interpret Numbers
*/
forth_word("itpnum", 6, F_NORMAL, itpnum, "{tib} ( -- *c l )",
           XT(number),
           XT(zbranch),
              XCELLS(7),

              XT(tick),
              (fcell_xt)FW_ERR_NOWORD,
              XT(err),
              XT(drop),

           XT(branch),
              XCELLS(10),

              XTV(STATE),
                XT(fetch),

              XT(zbranch),
                XCELLS(6),
                XT(tick),
                XT(tick),
                XT(comma),
                XT(comma),
                XT(drop),

           XT(semi),
           );

// try finding word and run if compiling, or execute if immediate
forth_word("itpnext", 7, F_NORMAL, itpnext, "{tib} ( -- *c l )",
           XT(find),
           XT(zbranch),
              XCELLS(14),

              XTV(STATE),
              XT(fetch),
              XT(xoru),

              XT(zbranch),
                XCELLS(5),
                XT(cfa),
                XT(comma),
              XT(branch),
                XCELLS(3),
                XT(cfa),
                XT(runs),

           XT(branch),
              XCELLS(2),
              XT(itpnum),
           XT(semi),
           );


/*
  Magic Interpreter
 */
forth_word("interpret", 9, F_NORMAL, interpret, "( p -- )",
           XT(word),
           XT(zbranch),
              XCELLS(13),
              XT(itpnext),

              XTV(STATE),
                XT(fetch),
              XT(tick),
                (fcell_xt)FW_ERR_NOWORD,
              XT(ssub),
              XT(zbranch),
                XCELLS(2),
                XT(interpret),
              XT(quits),

           XT(branch),
              XCELLS(3),
              XT(drop),
              XT(drop),

           XT(semi),
           );

