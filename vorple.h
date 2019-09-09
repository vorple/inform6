! ==============================================================================
!   VORPLE: User interface library for interactive fiction
!
!	Core functionality of Vorple, including JavaScript evaluation and adding of
!	HTML elements.
!
!	Vorple for Inform 6 (6/11)
!
!	Usage:
!		You have to include "vorple.h" two times:
!			1)	before parser
!			2)	after verblib
!       Alternately (if you want to use another vorple extension) include
!		vorple first, then the other extension. For instance:
!
!			Include "vorple.h";
!			Include "parser";
!			Include "verblib";
!			Include "vorple-multimedia.h";
!
!		Sorry, it's not the most convenient!
!
!	Warning:
!		Vorple replaces some routines of the Inform library to ensure the
!		proper functioning of certain behaviors. If your code also replaces
!		these routines, conflicts will arise. You can declare the constant
!		"VORPLE_NO_REPLACES" to prevent Vorple from replacing these routines
!		-- you will have to manually add the Vorple modifications in this case.
!
!	Version:	3.1
!	Released:	2018/09/23
! ==============================================================================

System_file;

! ------------------------------------------------------------------------------

#Ifndef	LIBRARY_VERSION;
! First pass -- before Parser is included
! (Parser defines "LIBRARY_VERSION", so if it's not defined we are indeed
! before Parser)

#Ifndef VORPLE_NO_REPLACES;
! To change the status line, add objects to "StatusLineRulebook" with your code
! as the description of the object. See the end of this file

Replace Banner OldBanner;                                       ! (verblibm.h)
Replace L__M OldLM;                                             ! (verblibm.h)
Replace YesOrNo OldYesNo;                                       ! (verblibm.h)
Replace AfterGameOver OldAfterGameOver;                         ! (parserm.h)
#Endif; ! VORPLE_NO_REPLACES;

! ------------------------------------------------------------------------------

#Ifnot;
! LIBRARY_VERSION is defined, so we're after parser : second pass!

#Ifndef VORPLE_LIBRARY; ! TODO - check if we need to define that part
Constant VORPLE_LIBRARY;

Include "infglk.h";


! The following 'bp_output_stream()' routine is directly taken from
! JustEnoughGlulx.h by Roger Firth. (We do not include the complete extension
! to avoid potential conflicts with Glulx entry points)
Constant BP_MEMORYFREF_ROCK 661;
Constant BP_MEMORY_MAXNEST 16;
Constant BP_OUTSTREAM_SCREEN 1;
Constant BP_OUTSTREAM_SCRIPT 2;
Constant BP_OUTSTREAM_MEMORY 3;
Constant BP_OUTSTREAM_CMDFILE 4;

Array bp_memory_buf --> BP_MEMORY_MAXNEST;
Array bp_memory_str --> BP_MEMORY_MAXNEST;

Global bp_memory_nest = 0;

[ bp_output_stream str buf max
	fref;
	#Ifdef TARGET_ZCODE;
    @output_stream str buf;
    fref = max; ! Avoid compiler warning
	#Ifnot; ! TARGET_GLULX;
	switch (str) {
    	bp_outstream_Screen:
            ; ! Code to be supplied

        -bp_outstream_Screen:
            ; ! Code to be supplied

        bp_outstream_Script: ! ScriptOn
            if (gg_scriptstr ~= GLK_NULL) rfalse;
            if (gg_scriptfref == GLK_NULL) {
                gg_scriptfref = glk_fileref_create_by_prompt(
					fileusage_TextMode + fileusage_Transcript,
					filemode_Write,
					GG_SCRIPTFREF_ROCK);
                if (gg_scriptfref == GLK_NULL) rfalse;
            }
            gg_scriptstr = glk_stream_open_file(
				gg_scriptfref,
				filemode_WriteAppend,
				GG_SCRIPTSTR_ROCK);
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
            glk_stream_set_current(glk_stream_open_memory(
				buf + WORDSIZE,
				max,
				filemode_Write,
				BP_MEMORYFREF_ROCK + bp_memory_nest));
            bp_memory_nest++;

        -bp_outstream_Memory:
            if (bp_memory_nest == 0) rfalse;
            bp_memory_nest--;
            glk_stream_close(glk_stream_get_current(), gg_arguments);
            glk_stream_set_current(bp_memory_str-->bp_memory_nest);
            (bp_memory_buf-->bp_memory_nest)-->0 = gg_arguments-->1;
            bp_memory_buf-->bp_memory_nest = bp_memory_str-->bp_memory_nest
				= GLK_NULL;

        bp_outstream_Cmdfile: ! CommandsOn
            if (gg_commandstr ~= GLK_NULL) rfalse;
            fref = glk_fileref_create_by_prompt(
				fileusage_TextMode + fileusage_InputRecord,
				filemode_Write,
				GG_COMMANDWSTR_ROCK);
            if (fref == GLK_NULL) rfalse;
            gg_commandstr = glk_stream_open_file(
				fref,
				filemode_Write,
				GG_COMMANDWSTR_ROCK);
            glk_fileref_destroy(fref);
            gg_command_reading = false;
            if (gg_commandstr == GLK_NULL) rfalse;

        -bp_outstream_Cmdfile: ! CommandsOff
            if (gg_commandstr == GLK_NULL) rfalse;
            if (gg_command_reading) rfalse;
            gg_command_reading = false;
            glk_stream_close(gg_commandstr, GLK_NULL);
            gg_commandstr = GLK_NULL;
    }
	#Endif; ! TARGET_
    rtrue;
];
! Here ends the code taken from JustEnoughGlulx.h

Global vorple_support = 0;

Global fref_handshake;
Global fref_js_eval;
Global fref_js_return;
Global fref_js_return_type;

!============== Vorple Interface

! The vorple interface setup rulebook. If you want to include
! rules in that rulebook, declare an object in it and put your code
! in its description
Object VorpleInterfaceSetup;

[ VorpleSetupTheInterface    r ;
	if (isVorpleSupported()) {
	    VorpleExecuteJavaScriptCommand(
			"return window._vorpleSetupRulebookHasRun||false");
	    if (~~VorpleWhatBooleanWasReturned()) {
	        ! go through the rules of vorple interface setup
	        objectloop(r in VorpleInterfaceSetup) r.description();
	        VorpleExecuteJavaScriptCommand(
				"window._vorpleSetupRulebookHasRun=true");
	    }
	}
];

! The vorple interface update rulebook. If you want to include
! rules in that rulebook, declare an object in it and put your code
! in its description
Object VorpleInterfaceUpdate;

[ VorpleUpdateTheInterface      r ;
        if (isVorpleSupported()) {
            ! go through the rules of vorple interface construction
            objectloop (r in VorpleInterfaceUpdate) r.description();
        }
];

!===============
! Vorple Initialise

! You can use a 'VorpleStartup()' routine to do things while Vorple
! initialises. For instance, to execute commands before play begins
#Stub VorpleStartup 0;

[ VorpleInitialise  ;
	! creates references
	! TODO - remove?
	!fref_handshake = make_fref(HANDSHAKE_FILE);
	!fref_js_eval = make_fref(JS_EVAL_FILE);
	!fref_js_return = make_fref(JS_RETURN_FILE);

	! test for vorple support
	vorple_support = do_vorple_handshake();

	! execute a few user-specified commands
	VorpleStartup();

	! stop all sound and music (needed for when the story is restarted
	VorpleExecuteJavaScriptCommand(
		"$('.vorple-sound').remove();vorple.audio.stopMusic()");

	! window title
	VorpleExecuteJavaScriptCommand(
		BuildCommand("document.title='", VorpleEscape(Story), "'"));
	! vorple interface setup rules
        VorpleSetupTheInterface();
        ! vorple interface construction rules
        VorpleUpdateTheInterface();
];

Global VorpleCommunicationDone = 0;

Constant BUFLEN = 500; ! used for most arrays
Array current_prompt buffer (BUFLEN-1);

[ StartVorpleStorySub ;
    ! set prefix
    VorpleExecuteJavaScriptCommand("vorple.prompt.setPrefix('>')");
    bp_output_stream(3, current_prompt, BUFLEN-1);
    print ">";
    bp_output_stream(-3);
    VorpleCommunicationDone = true;
];

Verb meta '__start_story'
	*   -> StartVorpleStory;


!================================
! Runtime errors

[ VorpleThrowRuntimeError text ;
	print "  *** Vorple run-time error: ";
	PrintStringOrArray(text);
	print " ***  ";
];


!================================
! Interpreter handshake

! Code by Juhana, cribbed from Zarf's examples probably? or from I7 code?

!Already defined in "infglk" TODO - remove?
!Constant filemode_Write = 1;
!Constant filemode_Read = 2;
!Constant filemode_ReadWrite = 3;

! TODO - this is used e.g. with "VorpleExecuteJavaScriptCommand()" is called
! with just a string ; should we increase that just in case (but then it costs
! more memory)?
Constant HANDSHAKE_FILE = "VpHndshk";
Constant JS_EVAL_FILE = "VpJSEval";

Array mybuffer -> BUFLEN+4;
Array mybuffer2 -> BUFLEN+4;
Array gg_result --> 2;

[ do_vorple_handshake  exists str len       ix;
    fref_handshake = make_fref(HANDSHAKE_FILE);
    exists = glk($0067, fref_handshake); ! fileref_does_file_exist
    if( ~~exists ) {
		! handshake file doesn't exist, not a Vorple interpreter
		rfalse;
	}
    str = open_file(fref_handshake, HANDSHAKE_FILE, filemode_Write);
    write_stream(str, "Callooh!");
    glk($0044, str, gg_result); ! stream_close

    ! next open the file again and close it immediately.
    ! filemode_ReadWrite signals the interpreter that it should read the file.
    !str = open_file(fref_handshake, HANDSHAKE_FILE, filemode_ReadWrite);
    !glk($0044, str, gg_result); ! stream_close
    signalFileToRead(fref_handshake, HANDSHAKE_FILE);

    ! the interpreter now writes "Callay!" to the file
    str = open_file(fref_handshake, HANDSHAKE_FILE, filemode_Read);
    len = glk($0092, str, mybuffer, BUFLEN); ! get_buffer_stream
    ! compare_string, but with a small twist (get_buffer_stream starts writing
	! at 0, not at WORDSIZE)
    ("Callay!").print_to_array(mybuffer2, BUFLEN);
    if (len ~= 7) {
		! wrong response from the interpreter
		rfalse;
	}
    for (ix=0 : ix<len : ix++) {
       if (mybuffer->ix ~= mybuffer2->(WORDSIZE+ix)) rfalse;
    }
    glk($0044, str, gg_result); ! stream_close
    rtrue; ! everything passed, it's a Vorple interpreter
];

[ signalFileToRead  myref myhandshake     str ;
    ! next open the file again and close it immediately.
    ! filemode_ReadWrite signals the interpreter that it should read the file.
    str = open_file(myref, myhandshake, filemode_ReadWrite);
    glk($0044, str, gg_result); ! stream_close
];

[ make_fref name   len fref;
    len = name.print_to_array(mybuffer, BUFLEN);
    if (len == BUFLEN) {
		VorpleThrowRuntimeError("File name too long; please increase BUFLEN");
	}
    mybuffer->3 = $E0;
    mybuffer->(4+len) = 0;
    fref = glk($0061, 0, mybuffer+3, 0); ! fileref_create_by_name
    ! (This is a type-data, binary file.)
    if (~~fref) {
        print "Unable to create fileref: ", name, "^";
        quit;
    }
    return fref;
];

[ open_file fref name   mode str;
    str = glk($0042, fref, mode, 0); ! stream_open_file
    if (~~str) {
        print "Unable to open stream: ", (string) name, "^";
        quit;
    }
    return str;
];

[ write_stream str val   len;
    if (val ofclass String) {
        len = val.print_to_array(mybuffer, BUFLEN);
        if (len == BUFLEN) {
			VorpleThrowRuntimeError(
				"JS command too long; please increase BUFLEN");
		}
        glk($0085, str, mybuffer+4, len); ! put_buffer_stream
    } else {
        glk($0085, str, val+4, val-->0);
    }
];

[ compare_string buf blen val   len ix;
    if (buf ofclass String) {
        blen = buf.print_to_array(mybuffer, BUFLEN);
    } else {
        bp_output_stream(3, mybuffer, BUFLEN);
        print (PrintStringOrArray) buf;
        bp_output_stream(-3);
        blen = buf-->0;
    }

    if (blen == BUFLEN) {
		VorpleThrowRuntimeError("compare_string: string number 1 too long;
 			please increase BUFLEN");
	}

    if (val ofclass String) {
        len = val.print_to_array(mybuffer2, BUFLEN);
    } else {
        bp_output_stream(3, mybuffer2, BUFLEN);
        print (PrintStringOrArray) val;
        bp_output_stream(-3);
        len = val-->0;
    }

    if (len == BUFLEN) {
		VorpleThrowRuntimeError("compare_string: string number 2 too long;
			please increase BUFLEN");
	}

    if (len ~= blen) rfalse;
    for (ix=0 : ix<len : ix++) {
        if (mybuffer->(WORDSIZE+ix) ~= mybuffer2->(WORDSIZE+ix)) rfalse;
    }
    return true;
];

[ Vp_IsJs ;
    return do_vorple_handshake();
];

[ isVorpleSupported ;
    return vorple_support;
];


!=======================================================
! utils

Constant LEN_LONGSTR = 500;
Constant LEN_SHORTSTR = 200;
Constant LEN_HUGESTR = 1500;
Constant LEN_HUGEHUGESTR = 2000;
Array longstr buffer LEN_LONGSTR;
Array shortstr buffer LEN_SHORTSTR;
Array hugestr buffer LEN_HUGESTR;
Array hugehugestr buffer LEN_HUGEHUGESTR;

! Print a string or an array (a buffer array, to be more precise)
[ PrintStringOrArray st i ;
	if (st ofclass String) {
		print (string) st;
	}
	else {
		for (i=0: i< st-->0: i++) {
			print (char) st->(WORDSIZE+i);
		}
	}
];

! Concatenates individual parts of a command, send a string back
[ BuildCommand str1 str2 str3 str4 str5 str6 str7;
	!@output_stream 3 hugehugestr;
    bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
	if (str1 ~= 0) { print (PrintStringOrArray) str1; }
	if (str2 ~= 0) { print (PrintStringOrArray) str2; }
	if (str3 ~= 0) { print (PrintStringOrArray) str3; }
	if (str4 ~= 0) { print (PrintStringOrArray) str4; }
	if (str5 ~= 0) { print (PrintStringOrArray) str5; }
	if (str6 ~= 0) { print (PrintStringOrArray) str6; }
	if (str7 ~= 0) { print (PrintStringOrArray) str7; }
    !@output_stream -3;
    bp_output_stream(-3);
    if (hugehugestr-->0 == LEN_HUGEHUGESTR) {
		VorpleThrowRuntimeError("JS command too long; please increase
			LEN_HUGEHUGESTR");
	}
	return hugehugestr ;
];

! int to string
Constant Vorple_intarraylength = 10;
Array integer buffer Vorple_intarraylength;
[ IntToString i   j ;
    if (i == 0) {
		integer-->0 = 1;
		integer->(WORDSIZE) = '0';
		return integer;
	}
	integer-->0 = Vorple_intarraylength;
	for (j=Vorple_intarraylength-1: j>-1: j--) {
		integer->(WORDSIZE+j) = i%10 + '0';
		i = (i - i%10)/10;
	}
	! trim leading zeros
	i=0; while ( integer->(WORDSIZE+i) == '0' or 0) { i++; }
	for (j=i: j<(integer-->0): j++) {
		integer->(WORDSIZE+j-i) = integer->(WORDSIZE+j);
		integer->(WORDSIZE+j) = 0;
	}
	integer-->0 = (integer-->0)-i;
	return integer;
];

! Unique identifiers
Array uid buffer 15;
[ UniqueIdentifier  i;
	!@output_stream 3 uid;
    bp_output_stream(3, uid, 15);
	print "id";
	for (i=0: i<12: i++) {
		print (char) random(10)+'0'-1;
	}
	!@output_stream -3;
    bp_output_stream(-3);
	return uid;
];

! Escape ^ and ' in text
Array toescape buffer 500;
Array safe buffer 500;
Array temp buffer 50;
[ VorpleEscape str;
	return VorpleEscapeLineBreaks(str, "");
];

! When we use print_to_array, '^' is replaced by a new line character but Zcode
! uses 'carriage return' ('\r'), which was the norm on old micros (ZX, C64,
! etc) and Mac before OSX and Glulx uses 'line feed' ('\n'), like Unix. There
! are lots of norms and conventions out there: Windows uses '\r\n', which means
! Unix files sometimes appear in Windows as one big line. Anyway, it's just a
! big standardisation debate
#Ifdef TARGET_ZCODE;
Constant NEW_LINE_CHAR = 13; ! '\r'
#Ifnot; ! TARGET_GLULX
Constant NEW_LINE_CHAR = 10; ! '\n'
#Endif;

! Transform a char into a Unicode value
[ Unicode x      y;
	y = (x & $7f00) / $100;
	if (x<0) y = y + $80;
	x = x & $ff;
	print (hexdigit) y/$10, (hexdigit) y, (hexdigit) x/$10, (hexdigit) x;
];

[ hexdigit x;
	x = x % $10;
	switch (x) {
    	0 to 9: print x;
    	10: print "a";
		11: print "b";
		12: print "c";
    	13: print "d";
		14: print "e";
		15: print "f";
	}
];

Array uniarray --> (BUFLEN+1);
Array strcloseresult --> 2;

[ VorpleEscapeLineBreaks text lb    i c oldstr str;
	for (i=0 : i<BUFLEN : i++)
		uniarray-->i = 0;

	oldstr = glk($0048); ! stream_get_current
	str = glk($0139, uniarray, BUFLEN, 1, 0); ! stream_open_memory_uni
	if (str == 0) {
		VorpleThrowRuntimeError("Unable to open memory stream.");
	}

	glk($0047, str); ! stream_set_current
	print (PrintStringOrArray) text;
	glk($0047, oldstr); ! stream_set_current
	glk($0044, str, strcloseresult); ! stream_close

	if (strcloseresult-->0 ~= 0)
		VorpleThrowRuntimeError("Memory stream records more than zero characters read");
	! strcloseresult-->1 is now the length of the stream

	!@output_stream 3 temp;
	bp_output_stream(3, temp, 50);
	print (PrintStringOrArray) lb;
	!@output_stream -3;
	bp_output_stream(-3);

	!@output_stream 3 safe;
	bp_output_stream(3, safe, BUFLEN);

	for (i=0: i<BUFLEN: i++) {
		c = uniarray-->i;

		if (c == 0) {
			break;
		}

		if (c == 39 || c == 92 || c == 34) { ! single quote or \ or double quote
			print (char) 92; ! \
			print (char) c;
			continue;
		}

		if (c == NEW_LINE_CHAR) { ! line break
			print (PrintStringOrArray) temp;
			continue;
		}

		if (c > 127) {
			print (char) 92; print "u";  ! \u
			Unicode(c);
			continue;
		} 

		print (char) c;
	}

	!@output_stream -3;
	bp_output_stream(-3);

	return safe;
];


!========================================
! JS Code execution

! Note: if you wish to know and use the result of a JS command,
!       the command must use the keyword "return". Use then
!       "VorpleWhatWasReturned" to get the answer as a string,
!       or "VorpleWhatBooleanWasReturned", "VorpleWhatTextWasReturned"
!       or "VorpleWhatNumberWasReturned" if you're sure of the type
!       of value this was ("VorpleWhatType" will tell you).
!       Example:
!         VorpleExecuteJavaScriptCommand("return 'foo'");
!         VorpleExecuteJavaScriptCommand("'bar'");
!         print VorpleWhatWasReturned();   ! prints "foo"

[ VorpleExecuteJavaScriptCommand cmd    str ;
    if (isVorpleSupported()) {
        fref_js_eval = make_fref(JS_EVAL_FILE);
        str = open_file(fref_js_eval, JS_EVAL_FILE, filemode_Write);
        write_stream(str, cmd);
        glk($0044, str, gg_result); ! stream_close
        !str = open_file(fref_js_eval, JS_EVAL_FILE, filemode_ReadWrite);
        !glk($0044, str, gg_result); ! stream_close
        signalFileToRead(fref_js_eval, JS_EVAL_FILE);
    }
];

Constant JS_RETURN_FILE = "VpJSRtrn";
Constant JS_RETURN_TYPE_FILE = "VpJSType";

! TODO - do we need that? or can we use other ones declared later?
Array returnedValue -> BUFLEN+4;
Array returnedValuebuffer buffer BUFLEN+4;

[ VorpleWhatWasReturned     str len i ;
    if (isVorpleSupported()) {
        fref_js_return = make_fref(JS_RETURN_FILE);
        str = open_file(fref_js_return, JS_RETURN_FILE, filemode_Read);
        ! what is returned here is an array, but we want a buffer array for the
        !      rest of the code
        len = glk($0092, str, returnedValue, BUFLEN); ! get_buffer_stream
        returnedValuebuffer-->0 = len;
        for (i=0: i<len: i++) {
			returnedValuebuffer->(WORDSIZE+i) = returnedValue->i;
		}
        glk($0044, str, gg_result); ! stream_close
        return returnedValuebuffer;
    } else {
		return "";
	}
];


! TODO - do we need that? or can we use other ones declared later?
Array returnedValueType -> BUFLEN+4;
Array returnedValueTypebuffer buffer BUFLEN+4;

[ VorpleWhatTypeWasReturned     str len i;
    if (isVorpleSupported()) {
        fref_js_return_type = make_fref(JS_RETURN_TYPE_FILE);
        str = open_file(fref_js_return_type, JS_RETURN_TYPE_FILE, filemode_Read);
        ! what is returned here is an array, but we want a buffer array for the
        !      rest of the code
        len = glk($0092, str, returnedValueType, BUFLEN); ! get_buffer_stream
        returnedValueTypebuffer-->0 = len;
        for (i=0: i<len: i++) {
			returnedValueTypebuffer->(WORDSIZE+i) = returnedValueType->i;
		}
        glk($0044, str, gg_result); ! stream_close
        return returnedValueTypebuffer;
    } else {
        return "";
    }
];

[ VorpleWhatTextWasReturned     txt len i ;
    if (~~isVorpleSupported()) {
		return "";
	}
    txt = VorpleWhatWasReturned();
    if ( ~~compare_string(VorpleWhatTypeWasReturned(), 4, "text")) {
		return txt;
	}
    len = txt-->0;
    ! remove first and last character
    for (i=1: i<len: i++) {
        txt->(WORDSIZE+i-1) = txt->(WORDSIZE+i);
    }
    txt-->0 = len-2;
    return txt;
];

[ VorpleConvertTextIntoNumber txt     s l neg prev res n c d;
    if (txt ofclass String) {
		txt.print_to_array(returnedValuebuffer, BUFLEN);
		return VorpleConvertTextIntoNumber(returnedValuebuffer);
	}
    s = 0;
    l = txt-->0;
    if (l == 0) {
		return 0;
	}
    neg = 0;
    prev = 0;
    if (txt->(WORDSIZE) == '-') {
		neg = 1; s++;
	}
    res = 0;
    for (n=s: n<l: n++) {
        c = txt->(WORDSIZE+n);
        d = 0;
        if (c < 58 && c > 47) { d = c-48; }
        if (c == '.') {
            ! floating point numbers are rounded into integers,
            ! because Inform doesn't do floating points.
            ! The rounding is to the closest integer, and
            ! "-1.500000" is rounded to -1 (but "1.50000" is rounded to 2)
            n++;
            c = txt->(WORDSIZE+n); ! first decimal
            if ( c >= 53) {        ! >= 5
                if (neg==1 && c == 53) { ! -.5XXXXX
                    ! Check: if -1.5, round to -1, if -1.51, round to 2
                    n++;
                    while (n<l) {
                        if (txt->(WORDSIZE+n) ~= 48) { ! ==0
                            jump notAll0s;
                        }
                        n++;
                    }
                    return -res;
                }
                .notAll0s;
                res++;
            }
            if (neg==1) { return -res; } else { return res; }
        }
        res = res * 10 + d;
        ! Check for overflow
        if (prev > res) {
            VorpleThrowRuntimeError(BuildCommand("Number ", txt, " exceeds Glulx range"));
            return 0;
        }
        prev = res;
    }
    if (neg == 1) { return -res; } else { return res; }
];

[ VorpleWhatNumberWasReturned       txt;
    if (isVorpleSupported() == false) { return 0; }
    txt = VorpleWhatWasReturned();
    if (compare_string(VorpleWhatTypeWasReturned(), 4, "text")) {
		txt = VorpleWhatTextWasReturned();
	}
    else {
        if (~~compare_string(VorpleWhatTypeWasReturned(), 6, "number")) {
            VorpleThrowRuntimeError(
				BuildCommand("Trying to convert return value of type ",
				VorpleWhatTypeWasReturned(), " into a number"));
            return 0;
        }
    }
    return VorpleConvertTextIntoNumber(txt);
];

[ VorpleWhatBooleanWasReturned   txt type ;
    if (~~isVorpleSupported()) {
		return false;
	}
    txt = VorpleWhatTextWasReturned();
    type = VorpleWhatTypeWasReturned();
    if (~~compare_string(type, 11, "truth state")) {
        VorpleThrowRuntimeError(
			BuildCommand("Trying to convert return value of type ", type,
			" into a boolean"));
        return false;
    }
    return compare_string(VorpleWhatWasReturned(),
		VorpleWhatWasReturned()-->0, "true");
];


!=======================================================
! Opening and closing HTML tags

[ VorpleOpenHTMLTag elt classes ;
    if (classes == 0) {
		classes = "";
	}
    VorpleExecuteJavaScriptCommand(
		BuildCommand("vorple.layout.openTag('", elt, "','", classes, "')"));
];

[ VorpleCloseHTMLTag ;
	VorpleExecuteJavaScriptCommand("vorple.layout.closeTag()");
];


!=======================================================
! Placing elements and displaying content

[ VorplePlaceElement elt classes txt ;
    if (elt == 0 or "") {
		elt = "tag";
	}
    if (classes == 0) {
		classes = "";
	}
    if (txt == 0) {
		txt = "";
	}
    VorpleOpenHTMLTag(elt, classes);
    print (PrintStringOrArray) txt;
    VorpleCloseHTMLTag();
];

[ VorplePlaceInlineElement classes txt ;
    VorplePlaceElement("span", classes, txt);
];

[ VorplePlaceSpanElement classes txt ;
    VorplePlaceInlineElement(classes, txt);
];

[ VorplePlaceBlockLevelElement classes txt ;
    VorplePlaceElement("div", classes, txt);
];

[ VorplePlaceDivElement classes txt ;
    VorplePlaceBlockLevelElement(classes, txt);
];

[ VorplePlaceElementAtTopLevel classes ;
    VorpleExecuteJavaScriptCommand("vorple.layout.focus('main#haven')");
    VorplePlaceBlockLevelElement(classes);
];

[ VorplePutContentInAllElements classes txt;
	VorpleExecuteJavaScriptCommand(
		BuildCommand("$('.", classes, "').text('",
		VorpleEscapeLineBreaks(txt, "@@92n"), "')"));
];

[ VorplePutContentInElement classes txt;
	VorplePutContentInAllElements(BuildCommand(classes, ":last"), txt);
];


!========================================
! Output focus

[ VorpleSetOutputFocus target ;
    VorpleExecuteJavaScriptCommand(
		BuildCommand("vorple.layout.focus('.", target, "')"));
];

[ VorpleSetOutputFocusMainWindow ;
    VorpleExecuteJavaScriptCommand("vorple.layout.focus('#window0')");
];


!=========================================
! Counting and testing for existence

[ VorpleCountElements elt ;
    VorpleExecuteJavaScriptCommand(
		BuildCommand("return $('.'+'", elt, "'.split(' ').join('.')).length"));
    return VorpleWhatNumberWasReturned();
];

[ VorpleElementExists elt ;
    return ( VorpleCountElements(elt) > 0);
];


!==========================================
! Scrolling

[ VorpleScrollToElement classes ;
    VorpleExecuteJavaScriptCommand(
		BuildCommand("vorple.layout.scrollTo('.", classes, ":last')"));
];

[ VorpleScrollToBottom ;
    VorpleExecuteJavaScriptCommand("vorple.layout.scrollToEnd()");
];


!=====================================
! Prompt

! This is an internal helper variable that shouldn't be changed manually (it
! will get overwritten). To change the prompt, two possibilities:
!	1)	change "Prompt" in your grammar file
!	2)	use the stub MyVorplePrompt() and return true when you're done
! Do not use LibraryMessages with Prompt (the Vorple prompt won't be changed).
! You can only set the prompt to a string (any command such as a tooltip or a
! hyperlink will get ignored).
Array Vorple_prompt buffer (BUFLEN-1);
#Stub MyVorplePrompt 0;

[ VorpleAppendToLM act n x1 s;
	x1 = s; ! avoid compiler warning
    if (act == ##Prompt) {
        ! This is also the moment we update the user interface
        VorpleUpdateTheInterface();
        ! Let's include notifications fallback here too
        #Ifdef VORPLE_NOTIFICATIONS;
        VorpleNotificationsFallback();
        #Endif;
        ! Now print the prompt in Vorple
        if (isVorpleSupported()) {
            bp_output_stream(3, Vorple_prompt, BUFLEN-1);
            ! if you haven't used the stub, Vorple will go look in "Prompt" in
            ! the grammar file. Either way, don't forget to start with a newline!
                if (~~MyVorplePrompt()) OldLM(##Prompt);
            bp_output_stream(-3);

            ! if the prompt has changed since last turn...
            if (~~compare_string(Vorple_prompt, BUFLEN-1, current_prompt)) {
                ! set it
                VorpleExecuteJavaScriptCommand(
					BuildCommand("vorple.prompt.setPrefix('",
					VorpleEscape(Vorple_prompt), "')"));
                ! change the current_prompt variable
                bp_output_stream(3, current_prompt, BUFLEN-1);
                print (PrintStringOrArray) Vorple_prompt;
                bp_output_stream(-3);
            }
			return true;
        }
    }
    ! Fix for music not stopping after restoring a game
    if (act == ##Restore && n == 2) {
        if (isVorpleSupported()) {
            VorpleExecuteJavaScriptCommand(
				"$('.vorple-sound').remove();vorple.audio.stopMusic()");
        }
    }
	return false;
];


!==============================
! UI Blocking

[ VorpleBlockUserInterface ;
    VorpleExecuteJavaScriptCommand("vorple.layout.block()");
];

[ VorpleUnblockUserInterface ;
    VorpleExecuteJavaScriptCommand("vorple.layout.unblock()");
];


!==============================
! Credits

[ VorpleAppendToBanner ;
    if (isVorpleSupported()) {
        VorpleExecuteJavaScriptCommand("return vorple.version");
        print "Vorple version ";
        PrintStringOrArray(VorpleWhatTextWasReturned());
	new_line;
    }
	return true;
];


!===========================================
! Replaced routines

#Ifndef VORPLE_NO_REPLACES;
[ Banner;
	OldBanner();
	VorpleAppendToBanner();
	return true;
];

[ L__M act n x1 s;
	if (VorpleAppendToLM(act, n, x1, s)) return true;
	OldLM(act, n, x1, s);
	return true;
];

! Matches the 6/11 (2004) version
[ YesOrNo i j;
    for (::) {
        #Ifdef TARGET_ZCODE;
        if (location == nothing || parent(player) == nothing) read buffer parse;
        else read buffer parse DrawStatusLine;
        j = parse->1;
        #Ifnot; ! TARGET_GLULX;
        KeyboardPrimitive(buffer, parse);
        j = parse-->0;
        #Endif; ! TARGET_
        if (j) { ! at least one word entered
            i = parse-->1;
            if (i == YES1__WD or YES2__WD or YES3__WD) rtrue;
            if (i == NO1__WD or NO2__WD or NO3__WD) rfalse;
        }
        L__M(##Quit, 1);
        if (isVorpleSupported()==0) print "> ";
    }
];

! Matches the 6/11 (2004) version
[ AfterGameOver i;

  .RRQPL;

    L__M(##Miscellany,5);

  .RRQL;

    if (isVorpleSupported()==0) { print "> "; }
    #Ifdef TARGET_ZCODE;
    #IfV3; read buffer parse; #Endif; ! V3
    temp_global=0;
    #IfV5; read buffer parse DrawStatusLine; #Endif; ! V5
    #Ifnot; ! TARGET_GLULX
    KeyboardPrimitive(buffer, parse);
    #Endif; ! TARGET_
    i = parse-->1;
    if (i == QUIT1__WD or QUIT2__WD) {
        #Ifdef TARGET_ZCODE;
        quit;
        #Ifnot; ! TARGET_GLULX
        quit;
        #Endif; ! TARGET_
    }
    if (i == RESTART__WD) {
        #Ifdef TARGET_ZCODE;
        @restart;
        #Ifnot; ! TARGET_GLULX
        @restart;
        #Endif; ! TARGET_
    }
    if (i == RESTORE__WD) {
        RestoreSub();
        jump RRQPL;
    }
    if (i == FULLSCORE1__WD or FULLSCORE2__WD && TASKS_PROVIDED==0) {
        new_line; FullScoreSub();
        jump RRQPL;
    }
    if (deadflag == 2 && i == AMUSING__WD && AMUSING_PROVIDED==0) {
        new_line; Amusing();
        jump RRQPL;
    }
    #IfV5;
    if (i == UNDO1__WD or UNDO2__WD or UNDO3__WD) {
        if (undo_flag == 0) {
            L__M(##Miscellany, 6);
            jump RRQPL;
        }
        if (undo_flag == 1) jump UndoFailed2;
        #Ifdef TARGET_ZCODE;
        @restore_undo i;
        #Ifnot; ! TARGET_GLULX
        @restoreundo i;
        i = (~~i);
        #Endif; ! TARGET_
        if (i == 0) {
          .UndoFailed2;
            L__M(##Miscellany, 7);
        }
        jump RRQPL;
    }
    #Endif; ! V5
    L__M(##Miscellany, 8);
    jump RRQL;
];

#Endif; ! VORPLE_NO_REPLACES;

#Endif; ! for the "Ifndef VORPLE_LIBRARY"
#Endif; ! for the "Ifdef LIBRARY_VERSION"
