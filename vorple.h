
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
Replace ActionPrimitive OldActionPrimitive;
Replace KeyboardPrimitive OldKeyboardPrimitive;
Replace AfterGameOver OldAfterGameOver;

! Replace routines defined in Verblib
Replace Banner OldBanner;
Replace RestartSub OldRestartSub;

#Ifnot; ! LIBRARY_VERSION is defined, so we're after parser : second pass!



#Ifndef VORPLE_LIBRARY;		! Check if we need to define that part
Constant VORPLE_LIBRARY;


Global vorple_supported;
Global VorpleCommunicationDone = 0;

#Stub VorpleStartup 0;		! use this to do things while Vorple initialises
							! for instance, to execute commands before play begins

! Vorple initialization ; put this in Initialise()

[ VorpleInitialise 		inp1 inp2 i ;
	vorple_supported = Vp_IsJs();
	if (vorple_supported == 0) { return ;}
	VorpleExecuteJavaScriptCommand("vorple.parser.setVorpleStory()");

	VorpleStartup();

	! Equivalent of the "Vorple Startup Rulebook": execute all the
	!   queued commands
	while (VorpleCommunicationDone == 0) {
		InformParser.parse_input(inputobjs);
		action = inputobjs-->0;
		! copy-pasted from the library
        inp1 = 0; inp2 = 0; i = inputobjs-->1;
        if (i >= 1) inp1 = inputobjs-->2;
        if (i >= 2) inp2 = inputobjs-->3;

        ! inp1 and inp2 hold: object numbers, or 0 for "multiple object",
        ! or 1 for "a number or dictionary address"

        if (inp1 == 1) noun = special_number1; else noun = inp1;
        if (inp2 == 1) {
           if (inp1 == 1) second = special_number2;
           else           second = special_number1;
        }
        else second = inp2;
        ! --------------------------------------------------------------
        ! Generate the action...
        if ((i == 0) ||  (i == 1 && inp1 ~= 0) || (i == 2 && inp1 ~= 0 && inp2 ~= 0)) {
           InformLibrary.begin_action(action, noun, second, 0);
        }
     }
    ! Undo marking intro as meta?
	VorpleExecuteJavaScriptCommand("vorple.parser.setTurnType('normal')");
];

[ IsVorpleSupported ;
	return vorple_supported;
];



!============================================================

! Check that the eval() stream is supported
! Currently only for Z-Machine
! by Dannii

Array streambuf buffer 200;

Constant HDR_SPECREVISION  $32;

[ Gestalt zid arg val;
	! Check a gestalt value
	@"EXT:30S" zid arg -> val;
	return val;
];

[ Vp_IsZ12 val ;
	! Check we're in a 1.2 version interpreter
	val = HDR_SPECREVISION-->0;
	if (val < $0102) rfalse;
	rtrue;
];

[ Vp_IsJS ;
	! Checking for eval() stream support
	if ( Vp_IsZ12() == false || Gestalt($30, 0, 0) == 0 ) rfalse;
	rtrue;
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

! Print a string or an array
[ PrintStringOrArray st i ;
	if (st ofclass String) { print (string) st; }
	else { 	for (i=0: i< st-->0: i++) { print (char) st->(WORDSIZE+i); } }
];

! Concatenates individual parts of a command, send a string back
[ BuildCommand str1 str2 str3 str4 str5 str6 str7 ;
	@output_stream 3 hugehugestr;
		if (str1 ~= 0) { print (PrintStringOrArray) str1; }
		if (str2 ~= 0) { print (PrintStringOrArray) str2; }
		if (str3 ~= 0) { print (PrintStringOrArray) str3; }
		if (str4 ~= 0) { print (PrintStringOrArray) str4; }
		if (str5 ~= 0) { print (PrintStringOrArray) str5; }
		if (str6 ~= 0) { print (PrintStringOrArray) str6; }
		if (str7 ~= 0) { print (PrintStringOrArray) str7; }
	@output_stream -3;
	return hugehugestr ;
];

! int to string
Constant Vorple_intarraylength = 10;
Array integer buffer Vorple_intarraylength;
[ IntToString i   j ;
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
	@output_stream 3 uid;
		print "id";
		for (i=0: i<12: i++) {
			print (char) random(10)+'0'-1;
		}
	@output_stream -3;
	return uid;
];

! Escape ^ and ' in text
Array  toescape buffer 500;
Array  safe buffer 500;
Array  temp buffer 50;
[ VorpleEscape str;
  return VorpleEscapeLineBreaks(str, "");
];

[ VorpleEscapeLineBreaks str lb    i;

	@output_stream 3 toescape;
	print (PrintStringOrArray) str;
	@output_stream -3;

	@output_stream 3 temp;
	print (PrintStringOrArray) lb;
	@output_stream -3;

	@output_stream 3 safe;
	for (i=0: i<toescape-->0: i++) {
		if ( toescape->(WORDSIZE+i) == 39) { ! single quote or \
			print (char) 92; ! \
		}
		if ( toescape->(WORDSIZE+i) ==  13) { ! line break
			print (PrintStringOrArray) temp;
			continue;
		}
		print (char) toescape->(WORDSIZE+i);
	}
	@output_stream -3;

	return safe;
];


!=======================================================
! Executing JavaScript commands

! Execute a JavaScript command (put it in the eval stream)
[ VorpleExecuteJavaScriptCommand t_0 ;
    if (IsVorpleSupported()) {
        ! open javascript channel
        @output_stream 5 streambuf;
        ! say javascript code
        print (PrintStringOrArray) t_0;
        ! close javascript channel
        @output_stream -5;
        }
];

! Queue a JavaScript command
[ VorpleQueueJavaScriptCommand t_0 ;
    if (IsVorpleSupported()) {
    	VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.queueExpression(function(){", t_0, "})"));
    }
];


!=======================================================
! Opening and closing HTML tags

[ VorpleOpenHTMLTag elt classes ;
	VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.openTag('", elt, "','", classes, "')"));
];

[ VorpleCloseHTMLTag ;
	VorpleExecuteJavaScriptCommand("vorple.parser.closeTag()");
];



!=======================================================
! Placing elements and displaying content


[ VorplePlaceElement elt classes txt ;
	if (elt == 0 or "") { elt = "tag"; }
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

[ VorplePutContentInAllElements classes txt;
	VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').text('", VorpleEscapeLineBreaks(txt, "@@92n"), "')"));
];

[ VorplePutContentInElement classes txt;
	VorplePutContentInAllElements(BuildCommand(classes, ":last"), txt);
];






!==================================================
! What kind of turn was that


! Tell Vorple what kind of turn it was
[ VorpleTypeOfTurn type ;
	VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.setTurnType('", type, "')"));
];


! If the command resulted in an error, Vorple wants to know because then it can make it disappear from the screen
! We use the routine ParserError, which is called in parserm.h when there's an error, and is by default a stub in Grammar
! If you defined your own (for instance if you use seeno.h), this will trigger an error, but just call yours "MyParserError"
# Stub MyParserError 0;
[ ParserError ;
	VorpleTypeOfTurn("error");
	return MyParserError();  ! needed for the error processing to continue
];


! Undoing
!    This is really tricky: 6/11 and 6/12 don't really have anything to hook on to do something right before undo
!         (no 'Perform_Undo' either, so we can't 'Replace Perform_Undo OldPerform_Undo' either)
!         (edit: added by a commit in October 2015, but since we want Vorple to be 6/11-compatible...)
!    Oddly enough, in the library, KeyboardPrimitive is called when taking the player's input at the beginning of a turn,
!         and then nowhere else if you're talking Z-machine!! (i.e. all the other calls are @read)
!    I don't know if this is a bug or a feature (KeyboardPrimitive in Z-machine has some V6 code that may not be suitable
!         everywhere else)
!    So that's what we use here; hopefully it doesn't break anything, let me know if it does.

!    We need to define the replace before KeyboardPrimitive appears (so, before parserm), but define our function after parserm is included because we need its global variables. This (and the meta stuff) is why we require vorple to be included both before and after parserm.h

!  or alternatively, we could have replaced AfterPrompt and look at if just_undone =1, but that might be too late for Vorple cause the turn is done by then


! Replace KeyboardPrimitive OldKeyboardPrimitive is at the beginning of the file
[ KeyboardPrimitive a_buffer a_table      nw w;
	OldKeyboardPrimitive(a_buffer, a_table);

	! See if it's undo and if it looks like it's going to work
	#Ifdef TARGET_ZCODE;
	nw = a_table->1;
	w = a_table-->1;
    if ((w == UNDO1__WD or UNDO2__WD or UNDO3__WD) && (nw==1)) {
        if (turns == START_MOVE || undo_flag == 0 || undo_flag == 1) {
			! undo is going to fail
		} else {
			VorpleTypeOfTurn("undo");
		}
	}
	#Endif;
];


! Meta actions: you need to tell Vorple if the current action is meta
! Where to call it? Roger Firth says "prior to performing the requested action, the BeforeRoutines() processing is omitted -- that is, GamePreRoutine() isn't called, and no react_before or before properties are executed."
! when you look at parserm, there's
!            if ((meta || BeforeRoutines() == false) && action < 4096)
!                ActionPrimitive();
! and ActionPrimitive is the thing that looks in the action table which action to execute. So this is the very, very end of the parser and the beginning of the action, so to speak (which is good because the I7 vorple code says "first specific action-processing rule")

[ ActionPrimitive ;
	if (meta) { VorpleTypeOfTurn("meta"); }
	OldActionPrimitive();
];





!========================================
! Interpreter communication

[ VorpleQueueCommand cmd show ;
	if (show == 0) { ! don't show the command
		VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.sendCommand(", cmd, ",{hideCommand:true})"));
	} else {
		VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.sendCommand(", cmd, ",{hideCommand:false})"));
	}
];

[ VorpleQueueSilentCommand cmd ;
	VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.sendSilentCommand(", cmd, ")"));
];

[ VorpleQueuePrimaryCommand cmd show ;
	if (show == 0) { ! don't show the command
		VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.sendPrimaryCommand(", cmd, ",{hideCommand:true})"));
	} else {
		VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.sendPrimaryCommand(", cmd, ",{hideCommand:false})"));
	}
];

[ VorpleQueueSilentPrimaryCommand cmd ;
	VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.sendSilentPrimaryCommand(", cmd, ")"));
];


[ StartVorpleStorySub ;
	VorpleExecuteJavaScriptCommand("vorple.parser.setTurnType('normal')");
	VorpleCommunicationDone = true;
];

Verb meta '__start_story'
*   -> StartVorpleStory;




!=======================================
! Element positions

! Those values are only needed in "Notifications", so we define them in "Notifications"
!      (maybe other I7 extensions needed them in the original I7 Vorple, but it doesn't apply here)


!=========================================
! Restart fix

Global question = "Are you sure you want to restart?";

[ RestartSub;
	if (IsVorpleSupported()) {
		! or maybe :
		! @output_stream 3 question;
		! L__M(##Restart,1);
		! @output_stream -3;
		VorpleExecuteJavaScriptCommand(BuildCommand("if(confirm('", VorpleEscape(question), "'))window.location.reload();"));
	}
	else { OldRestartSub(); }
];

! The only difference between this one and the library's is the response to restart
!       (that's why replacing RestartSub is not enough)
[ AfterGameOver i;

  .RRQPL;

    L__M(##Miscellany,5);

  .RRQL;

    print "> ";
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
        if (isVorpleSupported()) { VorpleExecuteJavaScriptCommand("window.location.reload();"); rtrue; } else { @restart; }
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


!===========================================
! Credits

[ Banner;
	OldBanner();
    if (IsVorpleSupported()) {
		print "Vorple version ";
    	VorplePlaceInlineElement("vorple-version","");
    	VorpleExecuteJavaScriptCommand("$('.vorple-version').html(vorple.core.getVersion())");
    }
    new_line;
];



!===========================================
! Hack to show notifications on the first turn

! Vorple uses LookRoutine, in a hack to display notifications on the first turn
! If you defined your own, this will trigger an error, but just call yours "MyParserError"
#Stub MyLookRoutine 0;

[ LookRoutine ;
	! each_turn is executed at the end of every turn, so we need a way to execute it at the very beginning too
	! cf Roger Firth's "Why don't my daemons run at the start of the game"
	#Ifdef VORPLE_NOTIFICATIONS_LIBRARY;
	if (turns == 0) { VorpleNotificationsFallback(); }
	#Endif;
	MyLookRoutine();
];






#Endif; ! for the "Ifndef VORPLE_LIBRARY"
#Endif; ! for the "Ifdef LIBRARY_VERSION"