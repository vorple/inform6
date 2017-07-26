! Core functionality of Vorple, including JavaScript evaluation and adding HTML elements.


! Vorple for Inform 6 (6/11)
! Usage:
! 		Include "vorple.h" before parser AND after verblib
!       Alternately (if you want to use another vorple extension) include vorple first,
!			then the other extension.
!		For instance:
!			Include "vorple.h";
!			Include "parser";
!			Include "verblib";
!			Include "vorple-multimedia.h";
!
!   Sorry, it's not the most convenient!

System_file;


! First pass (before 'parser' is included)
! (Parser defines "LIBRARY_VERSION", so if it's not defined we are indeed before parser)
#Ifndef	LIBRARY_VERSION;


! Replace routines defined in parser

! Replace routines defined in Verblib
Replace Banner OldBanner;
Replace L__M OldLM;

! to change the status line, add objects to StatusLineRulebook with your code as the description of the object
! see the end of this file
Replace DrawStatusLine OldDrawStatusLine;

#Ifnot; ! LIBRARY_VERSION is defined, so we're after parser : second pass!



#Ifndef VORPLE_LIBRARY;		! Check if we need to define that part

! this is a horribly hacky way to implement the vorple interface setup rules
! if you want to include more rules in that rulebook, declare an object in it and put your code in the description

Object VorpleInterfaceSetup;


! we include JustEnoughGlulx.h for a couple of bi-platform functionalities ! TODO: check license here
Include "JustEnoughGlulx";

Constant VORPLE_LIBRARY;

Global vorple_support = 0;

Global fref_handshake;
Global fref_js_eval;
Global fref_js_return;

#Stub VorpleStartup 0;		! use this to do things while Vorple initialises
                                ! for instance, to execute commands before play begins

[ VorpleInitialise      r ;
  ! creates references
  !fref_handshake = make_fref(HANDSHAKE_FILE);
  !fref_js_eval = make_fref(JS_EVAL_FILE);
  !fref_js_return = make_fref(JS_RETURN_FILE);
  
  ! test for vorple support
  vorple_support = do_vorple_handshake();
  
  ! execute a few user-specified commands
  VorpleStartup();
  
  ! stop all sound and music (needed for when the story is restarted
  VorpleExecuteJavaScriptCommand("$('.vorple-sound').remove();vorple.audio.stopMusic()");
  
  ! window title
  VorpleExecuteJavaScriptCommand(BuildCommand("document.title='", VorpleEscape(Story), "'"));
  
  ! vorple interface setup rules
  ! is the test for vorple support superflous here, since VorpleExecuteJavaScriptCommand just doesnt do anything for normal terps?
  if (isVorpleSupported()) {
      ! execute the "Vorple interface setup rules"
      VorpleExecuteJavaScriptCommand("window._vorpleSetupRulebookHasRun||false");
      if (VorpleWhatBooleanWasReturned() == false) {
          ! go through the rules of vorple interface setup
          objectloop(r in VorpleInterfaceSetup) r.description();
          VorpleExecuteJavaScriptCommand("window._vorpleSetupRulebookHasRun=true");
      }
  }
];

Global VorpleCommunicationDone = 0;

[ StartVorpleStorySub ;
    VorpleExecuteJavaScriptCommand("vorple.prompt.setPrefix('>')");
    VorpleCommunicationDone = true;
];

Verb meta '__start_story'
*   -> StartVorpleStory;


!================================
! Runtime errors

[ VorpleThrowRuntimeError text ;
    print "  *** Vorple run-time error: ";  PrintStringOrArray(text); print " ***  ";
];


!================================
! Interpreter handshake

! Code by Juhana, cribbed from Zarf's examples probably? or from I7 code?

!Already defined in JustEnoughGlulx
!Constant filemode_Write = 1;
!Constant filemode_Read = 2;
!Constant filemode_ReadWrite = 3; 

! TODO: this is used e.g. with VorpleExecuteJavaScriptCommand is called with just a string ; should we increase that just in case (but then it costs more memory)?
Constant BUFLEN = 500;
Constant HANDSHAKE_FILE = "VpHndshk";
Constant JS_EVAL_FILE = "VpJSEval";

Array mybuffer -> BUFLEN+4;
Array mybuffer2 -> BUFLEN+4;
Array gg_result --> 2;

[ do_vorple_handshake  exists str len       ix;
    fref_handshake = make_fref(HANDSHAKE_FILE);
    exists = glk($0067, fref_handshake); ! fileref_does_file_exist
    if( ~~exists ) rfalse;	   ! handshake file doesn't exist, not a Vorple interpreter
    str = open_file(fref_handshake, HANDSHAKE_FILE, filemode_Write);
    write_stream(str, "Callooh!");
    glk($0044, str, gg_result); ! stream_close
    
    ! next open the file again and close it immediately. 
    ! filemode_ReadWrite signals the interpreter that it should read the file.
    !str = open_file(fref_handshake, HANDSHAKE_FILE, filemode_ReadWrite);
    !glk($0044, str, gg_result); ! stream_close
    signalFileToRead(fref_handshake, HANDSHAKE_FILE);
    
    ! the interpreter now writes "Callay!" to the file.
    str = open_file(fref_handshake, HANDSHAKE_FILE, filemode_Read);
    len = glk($0092, str, mybuffer, BUFLEN);    ! get_buffer_stream
    ! compare_string, but with a small twist (get_buffer_stream starts writing at 0, not at WORDSIZE)
    ("Callay!").print_to_array(mybuffer2, BUFLEN);
    if (len ~= 7) rfalse;   ! wrong response from the interpreter
    for (ix=0 : ix<len : ix++) {
       if (mybuffer->ix ~= mybuffer2->(WORDSIZE+ix)) rfalse;
    }
    glk($0044, str, gg_result); ! stream_close
    rtrue;  ! everything passed, it's a Vorple interpreter
];

[ signalFileToRead  myref myhandshake     str ;
    ! next open the file again and close it immediately. 
    ! filemode_ReadWrite signals the interpreter that it should read the file.
    str = open_file(myref, myhandshake, filemode_ReadWrite);
    glk($0044, str, gg_result); ! stream_close
];

[ make_fref name   len fref;
    len = name.print_to_array(mybuffer, BUFLEN);
    if (len == BUFLEN) VorpleThrowRuntimeError("File name too long; please increase BUFLEN");
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
        if (len == BUFLEN) VorpleThrowRuntimeError("JS command too long; please increase BUFLEN");
        glk($0085, str, mybuffer+4, len); ! put_buffer_stream
    } else {
        glk($0085, str, val+4, val-->0);
    }
];

[ compare_string buf blen val   len ix;
    if (buf ofclass String) {
        blen = buf.print_to_array(mybuffer, BUFLEN);
        if (blen == BUFLEN) VorpleThrowRuntimeError("compare_string: string number 1 too long; please increase BUFLEN");
        len = val.print_to_array(mybuffer2, BUFLEN);
        if (len == BUFLEN) VorpleThrowRuntimeError("compare_string: string number 2 too long; please increase BUFLEN");
        if (len ~= blen) rfalse;
        for (ix=0 : ix<len : ix++) {
            if (mybuffer->(WORDSIZE+ix) ~= mybuffer2->(WORDSIZE+ix)) rfalse;
        }
    } else {
        len = val.print_to_array(mybuffer2, BUFLEN);
        if (len == BUFLEN) VorpleThrowRuntimeError("compare_string: string number 2 too long; please increase BUFLEN");
        if (len ~= blen) rfalse;
        for (ix=0 : ix<len : ix++) {
            if (buf->(WORDSIZE+ix) ~= mybuffer2->(WORDSIZE+ix)) rfalse;
        }
    }
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
Constant LEN_SHORTSTR= 200;
Constant LEN_HUGESTR = 1500;
Constant LEN_HUGEHUGESTR = 2000;
Array  longstr buffer LEN_LONGSTR;
Array  shortstr buffer LEN_SHORTSTR;
Array  hugestr buffer LEN_HUGESTR;
Array  hugehugestr buffer LEN_HUGEHUGESTR;

! Print a string or an array (a buffer array, to be more precise)
[ PrintStringOrArray st i ;
	if (st ofclass String) { print (string) st; }
	else { for (i=0: i< st-->0: i++) { print (char) st->(WORDSIZE+i); } }
];

! Concatenates individual parts of a command, send a string back
[ BuildCommand str1 str2 str3 str4 str5 str6 str7 ;
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
        if (hugehugestr-->0 == LEN_HUGEHUGESTR) VorpleThrowRuntimeError("JS command too long; please increase LEN_HUGEHUGESTR");
	return hugehugestr ;
];

! int to string
Constant Vorple_intarraylength = 10;
Array integer buffer Vorple_intarraylength;
[ IntToString i   j ;
        if (i == 0) { integer-->0 = 1; integer->(WORDSIZE) = '0'; return integer; }
	integer-->0 = Vorple_intarraylength;
	for (j=Vorple_intarraylength-1: j>-1: j--) {
		integer->(WORDSIZE+j) = i%10 + '0';
		i = (i - i%10)/10;
	}
	! trim leading zeros
	i=0; while ( integer->(WORDSIZE+i) == '0' or 0) {i++; }
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
Array  toescape buffer 500;
Array  safe buffer 500;
Array  temp buffer 50;
[ VorpleEscape str;
  return VorpleEscapeLineBreaks(str, "");
];



! When we use print_to_array, '^' is replaced by a new line character
!       but Zcode uses 'carriage return' ('\r'), which was the norm on old micros (ZX, C64, etc) and Mac before OSX
!       and Glulx uses 'line feed' ('\n'), like Unix.
!   There are lots of norms and conventions out there: Windows uses '\r\n', which means
!       Unix files sometimes appear in Windows as one big line
!   Anyway, it's just a big standardisation debate
#Ifdef TARGET_ZCODE;
Constant NEW_LINE_CHAR = 13;     ! '\r'
#Ifnot; ! TARGET_GLULX
Constant NEW_LINE_CHAR = 10;     ! '\n'
#Endif;



[ VorpleEscapeLineBreaks str lb    i;

	!@output_stream 3 toescape;
        bp_output_stream(3, toescape, 500);
	print (PrintStringOrArray) str;
	!@output_stream -3;
        bp_output_stream(-3);

	!@output_stream 3 temp;
        bp_output_stream(3, temp, 50);
	print (PrintStringOrArray) lb;
	!@output_stream -3;
        bp_output_stream(-3);

	!@output_stream 3 safe;
        bp_output_stream(3, safe, 500);
	for (i=0: i<toescape-->0: i++) {
		if ( toescape->(WORDSIZE+i) == 39 || toescape->(WORDSIZE+i) == 92) { ! single quote or \
			print (char) 92; ! \
		}
		if ( toescape->(WORDSIZE+i) == NEW_LINE_CHAR) { ! line break
			print (PrintStringOrArray) temp;
			continue;
		}
		print (char) toescape->(WORDSIZE+i);
	}
	!@output_stream -3;
        bp_output_stream(-3);

	return safe;
];


!========================================
! JS Code execution

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

! do we need that? or can we use other ones declared later?
Array returnedValue -> BUFLEN+4;
Array returnedValuebuffer buffer BUFLEN+4;

[ VorpleWhatWasReturned     str len i ;
    if (isVorpleSupported()) {
        fref_js_return = make_fref(JS_RETURN_FILE);
        str = open_file(fref_js_return, JS_RETURN_FILE, filemode_Read);
        ! what is returned here is an array, but we want a buffer array for the rest of the code
        len = glk($0092, str, returnedValue, BUFLEN);    ! get_buffer_stream
        returnedValuebuffer-->0 = len;
        for (i=0: i<len: i++) { returnedValuebuffer->(WORDSIZE+i) = returnedValue->i; }
        glk($0044, str, gg_result); ! stream_close
        return returnedValuebuffer;
    } else { return ""; }
];

[ VorpleWhatType txt        len c d;
    if (isVorpleSupported() == false) { return "nothing"; }
    
    ! assume this is called on a buffer array (like the one returned by VorpleWhatWasReturned)
    ! if not, try again after conversion
    if (txt ofclass String) {
        txt.print_to_array(returnedValuebuffer, BUFLEN);
        return VorpleWhatType(returnedValuebuffer);
    }
    
    len = txt-->0;
    if (compare_string(txt, len, "")) { return "nothing"; }
    if (compare_string(txt, len, "undefined")) { return "nothing"; }
    if (compare_string(txt, len, "null")) { return "nothing"; }
    if (compare_string(txt, len, "true")) { return "truth state"; }
    if (compare_string(txt, len, "false")) { return "truth state"; }
    if (compare_string(txt, len, "NaN")) { return "NaN"; }
    if (compare_string(txt, len, "Infinity")) { return "infinity"; }
    if (compare_string(txt, len, "-Infinity")) { return "infinity"; }
    c = txt->(WORDSIZE);  ! 1st character
    d = txt->(WORDSIZE+len-1); ! last character
    switch(c) {
        39: if (len == 1 || d ~= 39) { return "unknown"; } else { return "text"; }      ! 39 = '
        '[': return "list";
        '{': return "object";
    }
    ! this is so ugly!...
    if ( txt->(WORDSIZE) == 'f' && txt->(WORDSIZE) == 'u' && txt->(WORDSIZE) == 'n'
      && txt->(WORDSIZE) == 'c' && txt->(WORDSIZE) == 't' && txt->(WORDSIZE) == 'i'
      && txt->(WORDSIZE) == 'o' && txt->(WORDSIZE) == 'n' && txt->(WORDSIZE) == ' ') {
        return "function";
    }
    ! corresponding regexp : "^\-?\d+(\.\d+)?$"
    c = 0;      ! current number of letter
    ! "^\-?"
    if (txt->(WORDSIZE) == '-') { c++; }
    ! "\d+"
    if (txt->(WORDSIZE+c) > 58 || txt->(WORDSIZE+c) < 48) { return "unknown"; }
    while ( c < len && txt->(WORDSIZE+c) <= 58 && txt->(WORDSIZE+c) >= 48) { c++; }
    ! "()?$"
    if (c == len) { return "number";}
    ! "\.\d+"
    if (txt->(WORDSIZE+c) ~= '.') { return "unknown"; } else { c++; }
    while ( c < len && txt->(WORDSIZE+c) <= 58 && txt->(WORDSIZE+c) >= 48) { c++; }
    ! "$"
    if (c == len) { return "number";} else { return "unknown"; }
];

[ VorpleWhatTextWasReturned     txt len i ;
    if (isVorpleSupported() == false) { return ""; }
    txt = VorpleWhatWasReturned();
    if ( compare_string(VorpleWhatType(txt), 4, "text") == false) { return txt; }
    len = txt-->0;
    ! remove first and last character
    for (i=1: i<len: i++) {
        txt->(WORDSIZE+i) = txt->(WORDSIZE+i-1);
    }
    txt-->0 = len-2;
    return txt;
];

[ VorpleConvertTextIntoNumber txt     s l neg prev res n c d;
    if (txt ofclass String) { txt.print_to_array(returnedValuebuffer, BUFLEN); return VorpleConvertTextIntoNumber(returnedValuebuffer); }
    s = 0;
    l = txt-->0;
    if (l == 0) { return 0 ;}
    neg = 0;
    prev = 0;
    if (txt->(WORDSIZE) == '-') { neg = 1; s++; }
    res = 0;
    
    for (n=s: n<l: n++) {
        c = txt->(WORDSIZE+n);
        d = 0;
        if (c < 58 && c > 47) { d = c-48; }
        if (c == '.') {
            ! TODO floating points
            
!				let first decimal be text character number N + 1 in T converted into a number;
!				if first decimal > 5:
!					increment result;
!				else if first decimal is 5:
!					if negated is false:
!						increment result;
!					otherwise unless T exactly matches the regular expression "\-\d+\.50*":
!						increment result;
!				break;
        }
        res = res * 10 + d;
        if (prev > res) {
            VorpleThrowRuntimeError("Number ", txt, " exceeds Glulx range");
            return 0;
        }
        prev = res;
    }
    if (neg == 1) { return 0-res; } else { return res; }
];


[ VorpleWhatNumberWasReturned       txt;
    if (isVorpleSupported() == false) { return 0; }
    txt = VorpleWhatWasReturned();
    if (compare_string(VorpleWhatType(txt), 4, "text")) { txt = VorpleWhatTextWasReturned(); }
    else {
        if (compare_string(VorpleWhatType(txt), 6, "number") == false) {
            VorpleThrowRuntimeError(BuildCommand("Trying to convert return value of type ", VorpleWhatType(txt), " into a number"));
            return 0;
        }
    }
    return VorpleConvertTextIntoNumber(txt);
];

[ VorpleWhatBooleanWasReturned   txt type ;
    if (isVorpleSupported() == false) { return false;}
    txt = VorpleWhatTextWasReturned();
    type = VorpleWhatType(txt);
    if (compare_string(type, 11, "truth state") == false) {
        VorpleThrowRuntimeError(BuildCommand("Trying to convert return value of type ", type, " into a boolean"));
        return false;
    }
    return compare_string(VorpleWhatWasReturned(), VorpleWhatWasReturned()-->0, "true");
];




!=======================================================
! Opening and closing HTML tags

[ VorpleOpenHTMLTag elt classes ;
    if (classes == 0) { classes = ""; }
    VorpleExecuteJavaScriptCommand(BuildCommand("vorple.layout.openTag('", elt, "','", classes, "')"));
];

[ VorpleCloseHTMLTag ;
	VorpleExecuteJavaScriptCommand("vorple.layout.closeTag()");
];



!=======================================================
! Placing elements and displaying content


[ VorplePlaceElement elt classes txt ;
    if (elt == 0 or "") { elt = "tag"; }
    if (classes == 0) { classes = ""; }
    if (txt == 0) { txt = ""; }
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
	VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').text('", VorpleEscapeLineBreaks(txt, "@@92n"), "')"));
];

[ VorplePutContentInElement classes txt;
	VorplePutContentInAllElements(BuildCommand(classes, ":last"), txt);
];


!========================================
! Output focus

[ VorpleSetOutputFocus target ;
    VorpleExecuteJavaScriptCommand(BuildCommand("vorple.layout.focus('.", target, "')"));
];

[ VorpleSetOutputFocusMainWindow ;
    VorpleExecuteJavaScriptCommand("vorple.layout.focus('#window0')");
];


!=========================================
! Counting and testing for existence

[ VorpleCountElements elt ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.'+'", elt, "'.split(' ').join('.')).length"));
    return VorpleWhatNumberWasReturned();
];

[ VorpleElementExists elt ;
    return ( VorpleCountElements(elt) > 0);
];


!==========================================
! Scrolling

[ VorpleScrollToElement classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("vorple.layout.scrollTo('.", classes, ":last')"));
];

[ VorpleScrollToBottom ;
    VorpleExecuteJavaScriptCommand("vorple.layout.scrollToEnd()");
];





!=====================================
! Prompt

! This is an internal helper variable that shouldn't be changed manually (it will get overwritten).
! To change the prompt, change the usual "Prompt" action (via the LibraryMessages object for instance).
! However you can only set it to a string (any command such as a tooltip or a hyperlink will get ignored).
Array Vorple_prompt buffer BUFLEN;

[ L__M act n x1 s;
    ! Print the prompt in Vorple
    if (act == ##Prompt) {
        if (isVorpleSupported()) {
            !@output_stream 3 Vorple_prompt;
            bp_output_stream(3, Vorple_prompt, BUFLEN);
            OldLM(##Prompt);
            !@output_stream -3;
            bp_output_stream(-3);
            VorpleExecuteJavaScriptCommand(BuildCommand("vorple.prompt.setPrefix('", VorpleEscape(Vorple_prompt), "')"));
            return;
        }
    }
    ! Fix for music not stopping after restoring a game
    if (act == ##Restore && n == 2) {
        if (isVorpleSupported()) {
            VorpleExecuteJavaScriptCommand("$('.vorple-sound').remove();vorple.audio.stopMusic()");
        }
    }
    OldLM(act, n, x1, s);
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

[ Banner ;
    OldBanner();
    if (isVorpleSupported()) {
        VorpleExecuteJavaScriptCommand("vorple.version");
        print "Vorple version "; PrintStringOrArray(VorpleWhatTextWasReturned()); print " preview";
    }
    new_line;
];


!===========================================
! Hack to show notifications on the first turn

! Vorple uses LookRoutine, in a hack to display notifications on the first turn
! If you defined your own, this will trigger an error, but just call yours "MyLookRoutine"
#Stub MyLookRoutine 0;

[ LookRoutine ;
	! each_turn is executed at the end of every turn, so we need a way to execute it at the very beginning too
	! cf Roger Firth's "Why don't my daemons run at the start of the game"
	#Ifdef VORPLE_NOTIFICATIONS;
	if (turns == 0) { VorpleNotificationsFallback(); }
	#Endif;
	MyLookRoutine();
];


!===========================================
! Refresh the status line

Object StatusLineRulebook "status line rulebook";

! if you don't want normal status lines, insert "remove drawNormalStatusLine;" in your Initialise() routine
Object drawNormalStatusLine "draw normal statusline" StatusLineRulebook
    with description [; OldDrawStatusLine(); rtrue; ];

[ DrawStatusLine r ;
    objectloop(r in StatusLineRulebook) { r.description(); }
];






#Endif; ! for the "Ifndef VORPLE_LIBRARY"
#Endif; ! for the "Ifdef LIBRARY_VERSION"