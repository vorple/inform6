! ---------------------------------------------------------------------------- !
!   JustEnoughGlulx.h by Roger Firth (roger@firthworks.com)
!
!       1.1 Jun 2001
!           Added bp_Graphic(), bp_Sound() and bp_B/U/R.
!
!       1.0 Jun 2001
!           Original version.
!
! ---------------------------------------------------------------------------- !
!   Installation: add the line:
!
!       Include "JustEnoughGlulx";
!
!   immediately after the Include "Parser" line.
!
! ---------------------------------------------------------------------------- !
!
!   JustEnoughGlulx implements a small set of functions to support bi-platform
!   games -- games that will compile without change using both the original
!   Inform compiler and the new Glulx compiler. The goal is to increase the
!   ease with which a game originally written for the Inform Z-machine can also
!   be run on the Glulx VM.

!   For instructions on using the functions, see
!   http://www.firthworks.com/roger/glulx/
!
! ---------------------------------------------------------------------------- !

System_file;                            ! Don't warn about unused routines
#ifndef  JustEnoughGlulx;               ! Do nothing if already #Included
Constant JustEnoughGlulx;

Global   bp_GraphicSupport = false;     ! Graphics supported?
Global   bp_SoundSupport = false;       ! Sound supported?
Global   bp_GraphicSoundWarning = true; ! Show warning if unable to output?

#ifdef  TARGET_GLULX;
    Message "Glulx Inform compiler --> Glulx VM.";
    Include "InfGlk";               ! Wrappers for Glk functions

    Constant    BP_SOUNDCHAN_ROCK   660;
    Global      bp_soundchan = 0;

    Constant    BP_MEMORYFREF_ROCK  661;
    Constant    BP_MEMORY_MAXNEST   16;
    Array       bp_memory_buf -->   BP_MEMORY_MAXNEST;
    Array       bp_memory_str -->   BP_MEMORY_MAXNEST;
    Global      bp_memory_nest = 0;

    [ InitGlkWindow winrock;
        switch (winrock) {
            0:                      ! Phase 1: Start of execution
                ;
            GG_MAINWIN_ROCK:        ! Phase 2: About to set up story window
                glk_stylehint_set(wintype_TextBuffer, style_User1, stylehint_Oblique, true);
                glk_stylehint_set(wintype_TextBuffer, style_User2, stylehint_ReverseColor, true);
                ;
            GG_STATUSWIN_ROCK:      ! Phase 3: About to set up status window
                ;
            1:                      ! Phase 4: After window setup
                bp_GraphicSupport = glk_gestalt(gestalt_Graphics);
                bp_SoundSupport = glk_gestalt(gestalt_Sound);
                if (bp_SoundSupport)
                    bp_soundchan = glk_schannel_create(BP_SOUNDCHAN_ROCK);
                ;
            GG_QUOTEWIN_ROCK:       ! Phase 5: Quote window needed during game
                ;
            }
        rfalse;                     ! Continue with standard setup
        ];

    [ IdentifyGlkObject phase type ref rock
        id;
        switch (phase) {
            0:                      ! Phase 0: Set object references to zero
                bp_soundchan = 0;
                ;
            1:                      ! Phase 1: For unrecognized windows/streams/filerefs
                ;
            2:                      ! Phase 3: For other user-defined objects
                id = glk_schannel_iterate(0, gg_arguments);
                while (id) {
                    switch(gg_arguments-->0) {
                        BP_SOUNDCHAN_ROCK: bp_soundchan = id;
                        }
                    id = glk_schannel_iterate(id, gg_arguments);
                    }
            }
        return type + ref + rock;   ! Avoid compiler warning
        ];

    #ifndef DEBUG;
    Global      gg_commandstr = GLK_NULL;
    Global      gg_command_reading = false;
    #endif;

#ifnot; #ifdef  WORDSIZE;
    Message "Glulx Inform compiler --> Z-machine.";

#ifnot;
    Message  "Original Inform compiler --> Z-machine.";
    Constant    TARGET_ZCODE    0;
    Constant    WORDSIZE        2;

#endif; #endif;

! ---------------------------------------------------------------------------- !

Constant bp_style_Roman         0;
Constant bp_style_Bold          1;
Constant bp_style_Underline     2;
Constant bp_style_Reverse       3;

[ bp_style sty;
#ifdef  TARGET_ZCODE;
        switch (sty) {
            bp_style_Roman:     style roman;
            bp_style_Bold:      style bold;
            bp_style_Underline: style underline;
            bp_style_Reverse:   style reverse;
            }

#ifnot; ! TARGET_GLULX;
        switch (sty) {
            bp_style_Roman:     glk_set_style(style_Normal);
            bp_style_Bold:      glk_set_style(style_Emphasized);
            bp_style_Underline: glk_set_style(style_User1);
            bp_style_Reverse:   glk_set_style(style_User2);
            }

#endif; ! TARGET_
        return sty; ];

[ bp_B txt; bp_style(bp_style_Bold);      print (string) txt; bp_style(bp_style_Roman); ];
[ bp_U txt; bp_style(bp_style_Underline); print (string) txt; bp_style(bp_style_Roman); ];
[ bp_R txt; bp_style(bp_style_Reverse);   print (string) txt; bp_style(bp_style_Roman); ];

! ---------------------------------------------------------------------------- !

[ bp_Graphic img num
        x; x = bp_GraphicSupport;
        new_line;
#ifdef  TARGET_GLULX;
        if (x) { spaces num; x = glk_image_draw(gg_mainwin, img, imagealign_InlineDown); }
#endif; ! TARGET_
        if (x) new_line;
        else if (bp_GraphicSoundWarning) print "[Graphic ", img, " should have appeared here.]^";
        new_line;
        img = num;                  ! Avoid compiler warning
        return x;
        ];

[ bp_Sound snd num
        x; x = bp_SoundSupport;
#ifdef  TARGET_GLULX;
        if (x) x = glk_schannel_play_ext(bp_soundchan, snd, num);
#endif; ! TARGET_
        if ((~~x) && bp_GraphicSoundWarning) print "^[Sound ", snd, " should have played here.]^^";
        snd = num;                  ! Avoid compiler warning
        return x;
        ];

! ---------------------------------------------------------------------------- !

Constant bp_window_Lower        0;
Constant bp_window_Upper        1;
Constant bp_window_All         -1;

[ bp_erase_window win;
#ifdef  TARGET_ZCODE;
        @erase_window win;

#ifnot; ! TARGET_GLULX;
        switch (win) {
            bp_window_Lower:    glk_window_clear(gg_mainwin);
            bp_window_Upper:    glk_window_clear(gg_statuswin);
            bp_window_All:      glk_window_clear(gg_mainwin); glk_window_clear(gg_statuswin);
            }

#endif; ! TARGET_
        return win; ];

! ---------------------------------------------------------------------------- !

[ bp_window_get_width win;
#ifdef  TARGET_ZCODE;
        win = 0->$21;

#ifnot; ! TARGET_GLULX;
        switch (win) {
            bp_window_Lower:    glk_window_get_size(gg_mainwin,gg_arguments,gg_arguments+WORDSIZE);
            bp_window_Upper:    glk_window_get_size(gg_statuswin,gg_arguments,gg_arguments+WORDSIZE);
            default:            gg_arguments-->0 = 0;
            }
        win = gg_arguments-->0;

#endif; ! TARGET_
        return win; ];

[ bp_window_get_height win;
#ifdef  TARGET_ZCODE;
        win = 0->$20;

#ifnot; ! TARGET_GLULX;
        switch (win) {
            bp_window_Lower:    glk_window_get_size(gg_mainwin,gg_arguments,gg_arguments+WORDSIZE);
            bp_window_Upper:    glk_window_get_size(gg_statuswin,gg_arguments,gg_arguments+WORDSIZE);
            default:            gg_arguments-->WORDSIZE = 0;
            }
        win = gg_arguments-->WORDSIZE;

#endif; ! TARGET_
        return win; ];

! ---------------------------------------------------------------------------- !

[ bp_set_window win;
#ifdef  TARGET_ZCODE;
        @set_window win;

#ifnot; ! TARGET_GLULX;
        switch (win) {
            bp_window_Lower:    glk_set_window(gg_mainwin);
            bp_window_Upper:    glk_set_window(gg_statuswin);
            }

#endif; ! TARGET_
        return win; ];

! ---------------------------------------------------------------------------- !

[ bp_set_cursor lin col;
#ifdef  TARGET_ZCODE;
        @set_cursor lin col;

#ifnot; ! TARGET_GLULX;
        glk_window_move_cursor(gg_statuswin, col-1, lin-1);

#endif; ! TARGET_
        rtrue; ];

! ---------------------------------------------------------------------------- !

Constant bp_instream_Keyboard   0;
Constant bp_instream_Cmdfile    1;

[ bp_input_stream str
        fref;
#ifdef  TARGET_ZCODE;
        @input_stream str;
        fref = 0;                   ! Avoid compiler warning

#ifnot; ! TARGET_GLULX;
        switch (str) {
            bp_instream_Keyboard:
                ;                   ! Code to be supplied (not sure we need anything)

            bp_instream_Cmdfile:    ! CommandsRead
                if (gg_commandstr ~= GLK_NULL) rfalse;
                fref = glk_fileref_create_by_prompt(fileusage_TextMode+fileusage_InputRecord, filemode_Read, GG_COMMANDRSTR_ROCK);
                if (fref == GLK_NULL) rfalse;
                gg_commandstr = glk_stream_open_file(fref, filemode_Read, GG_COMMANDRSTR_ROCK);
                glk_fileref_destroy(fref);
                gg_command_reading = true;
                if (gg_commandstr == GLK_NULL) rfalse;
            }

#endif; ! TARGET_
        rtrue; ];

! ---------------------------------------------------------------------------- !

Constant bp_outstream_Screen    1;
Constant bp_outstream_Script    2;
Constant bp_outstream_Memory    3;
Constant bp_outstream_Cmdfile   4;

[ bp_output_stream str buf max
        fref;
#ifdef  TARGET_ZCODE;
        @output_stream str buf;
        fref = max;                 ! Avoid compiler warning

#ifnot; ! TARGET_GLULX;
        switch (str) {
            bp_outstream_Screen:
                ;                   ! Code to be supplied

            -bp_outstream_Screen:
                ;                   ! Code to be supplied

            bp_outstream_Script:    ! ScriptOn
                if (gg_scriptstr ~= GLK_NULL) rfalse;
                if (gg_scriptfref == GLK_NULL) {
                    gg_scriptfref = glk_fileref_create_by_prompt(fileusage_TextMode+fileusage_Transcript, filemode_Write, GG_SCRIPTFREF_ROCK);
                    if (gg_scriptfref == GLK_NULL) rfalse;
                    }
                gg_scriptstr = glk_stream_open_file(gg_scriptfref, filemode_WriteAppend, GG_SCRIPTSTR_ROCK);
                if (gg_scriptstr == GLK_NULL) rfalse;
                glk_window_set_echo_stream(gg_mainwin, gg_scriptstr);

            -bp_outstream_Script:   ! ScriptOff
                if (gg_scriptstr == GLK_NULL) rfalse;
                glk_stream_close(gg_scriptstr, GLK_NULL);
                gg_scriptstr = GLK_NULL;

            bp_outstream_Memory:
                if (bp_memory_nest == BP_MEMORY_MAXNEST) rfalse;
                bp_memory_buf-->bp_memory_nest = buf;
                bp_memory_str-->bp_memory_nest = glk_stream_get_current();
                glk_stream_set_current(glk_stream_open_memory(buf+WORDSIZE, max, filemode_Write, BP_MEMORYFREF_ROCK+bp_memory_nest));
                bp_memory_nest++;

            -bp_outstream_Memory:
                if (bp_memory_nest == 0) rfalse;
                bp_memory_nest--;
                glk_stream_close(glk_stream_get_current(), gg_arguments);
                glk_stream_set_current(bp_memory_str-->bp_memory_nest);
                (bp_memory_buf-->bp_memory_nest)-->0 = gg_arguments-->1;
                bp_memory_buf-->bp_memory_nest = bp_memory_str-->bp_memory_nest = GLK_NULL;

            bp_outstream_Cmdfile:   ! CommandsOn
                if (gg_commandstr ~= GLK_NULL) rfalse;
                fref = glk_fileref_create_by_prompt(fileusage_TextMode+fileusage_InputRecord, filemode_Write, GG_COMMANDWSTR_ROCK);
                if (fref == GLK_NULL) rfalse;
                gg_commandstr = glk_stream_open_file(fref, filemode_Write, GG_COMMANDWSTR_ROCK);
                glk_fileref_destroy(fref);
                gg_command_reading = false;
                if (gg_commandstr == GLK_NULL) rfalse;

            -bp_outstream_Cmdfile:  ! CommandsOff
                if (gg_commandstr == GLK_NULL) rfalse;
                if (gg_command_reading) rfalse;
                gg_command_reading = false;
                glk_stream_close(gg_commandstr, GLK_NULL);
                gg_commandstr = GLK_NULL;
            }

#endif; ! TARGET_
        rtrue; ];

! ---------------------------------------------------------------------------- !

Constant bp_sound_High          1;
Constant bp_sound_Low           2;

[ bp_sound_effect num;
#ifdef  TARGET_ZCODE;
        @sound_effect num;

#ifnot; ! TARGET_GLULX;
        glk_schannel_play(bp_soundchan, num);

#endif; ! TARGET_
        return num; ];

! ---------------------------------------------------------------------------- !

Constant bp_keycode_Unknown     0;
Constant bp_keycode_Left      131;
Constant bp_keycode_Right     132;
Constant bp_keycode_Up        129;
Constant bp_keycode_Down      130;
Constant bp_keycode_Return     13;
Constant bp_keycode_Delete      8;
Constant bp_keycode_Escape     31;
Constant bp_keycode_Tab         9;
Constant bp_keycode_Func1     133;
Constant bp_keycode_Func2     134;
Constant bp_keycode_Func3     135;
Constant bp_keycode_Func4     136;
Constant bp_keycode_Func5     137;
Constant bp_keycode_Func6     138;
Constant bp_keycode_Func7     139;
Constant bp_keycode_Func8     140;
Constant bp_keycode_Func9     141;
Constant bp_keycode_Func10    142;
Constant bp_keycode_Func11    143;
Constant bp_keycode_Func12    144;

[ bp_read_char
        x;
#ifdef  TARGET_ZCODE;
        @read_char 1 -> x;

#ifnot; ! TARGET_GLULX;
        x = KeyCharPrimitive(); switch (x) {
            keycode_Unknown:    x = bp_keycode_Unknown;
            keycode_Left:       x = bp_keycode_Left;
            keycode_Right:      x = bp_keycode_Right;
            keycode_Up:         x = bp_keycode_Up;
            keycode_Down:       x = bp_keycode_Down;
            keycode_Return:     x = bp_keycode_Return;
            keycode_Delete:     x = bp_keycode_Delete;
            keycode_Escape:     x = bp_keycode_Escape;
            keycode_Tab:        x = bp_keycode_Tab;
            keycode_PageUp:     x = bp_keycode_Unknown;
            keycode_PageDown:   x = bp_keycode_Unknown;
            keycode_Home:       x = bp_keycode_Unknown;
            keycode_End:        x = bp_keycode_Unknown;
            keycode_Func1:      x = bp_keycode_Func1;
            keycode_Func2:      x = bp_keycode_Func2;
            keycode_Func3:      x = bp_keycode_Func3;
            keycode_Func4:      x = bp_keycode_Func4;
            keycode_Func5:      x = bp_keycode_Func5;
            keycode_Func6:      x = bp_keycode_Func6;
            keycode_Func7:      x = bp_keycode_Func7;
            keycode_Func8:      x = bp_keycode_Func8;
            keycode_Func9:      x = bp_keycode_Func9;
            keycode_Func10:     x = bp_keycode_Func10;
            keycode_Func11:     x = bp_keycode_Func11;
            keycode_Func12:     x = bp_keycode_Func12;
            }

#endif; ! TARGET_
        return x; ];

! ---------------------------------------------------------------------------- !

[ bp_storeb arr off val;
        #ifdef TARGET_ZCODE;    @storeb arr off val;    #endif;
        #ifdef TARGET_GLULX;    @astoreb arr off val;   #endif;
        return val; ];

[ bp_storew arr off val;
        #ifdef TARGET_ZCODE;    @storew arr off val;    #endif;
        #ifdef TARGET_GLULX;    @astore arr off val;    #endif;
        return val; ];

! ---------------------------------------------------------------------------- !

#endif; ! JustEnoughGlulx;
! ---------------------------------------------------------------------------- !
