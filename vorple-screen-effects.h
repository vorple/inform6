Include "vorple.h";
System_file;

! Vorple equivalent of Basic Screen Effects by Emily Short. Waiting for a keypress, clearing the screen, aligning, styling and coloring text.

!=================================================
! Styles

Constant STYLE_CURSIVE = 0;
Constant STYLE_EMPHASIS = 1;
Constant STYLE_FANTASY = 2;
Constant STYLE_MONOSPACE = 3;
Constant STYLE_NOWRAP = 4;
Constant STYLE_STRIKETHROUGH = 5;
Constant STYLE_STRONG = 6;
Constant STYLE_TRANSIENT = 7;
Constant STYLE_UNDERLINE = 8;
Constant STYLE_XXSMALL = 9;
Constant STYLE_XSMALL = 10;
Constant STYLE_SMALL = 11;
Constant STYLE_LARGE = 12;
Constant STYLE_XLARGE = 13;
Constant STYLE_XXLARGE = 14;

Constant STYLE_BLACK_LETTERS = 15;
Constant STYLE_RED_LETTERS = 16;
Constant STYLE_GREEN_LETTERS = 17;
Constant STYLE_YELLOW_LETTERS = 18;
Constant STYLE_BLUE_LETTERS = 19;
Constant STYLE_MAGENTA_LETTERS = 20;
Constant STYLE_CYAN_LETTERS = 21;
Constant STYLE_WHITE_LETTERS = 22;

Constant STYLE_BLACK_BACKGROUND = 23;
Constant STYLE_RED_BACKGROUND = 24;
Constant STYLE_GREEN_BACKGROUND = 25;
Constant STYLE_YELLOW_BACKGROUND = 26;
Constant STYLE_BLUE_BACKGROUND = 27;
Constant STYLE_MAGENTA_BACKGROUND = 28;
Constant STYLE_CYAN_BACKGROUND = 29;
Constant STYLE_WHITE_BACKGROUND = 30;

[ StyleName sty ;
	if (sty ofclass string) { return sty; }
	switch(sty) {
		STYLE_CURSIVE: return "cursive";
		STYLE_EMPHASIS: return "emphasis";
		STYLE_FANTASY: return "fantasy";
		STYLE_MONOSPACE: return "monospace";
		STYLE_NOWRAP: return "nowrap";
		STYLE_STRIKETHROUGH: return "strikethrough";
		STYLE_STRONG: return "strong";
		STYLE_TRANSIENT: return "transient";
		STYLE_UNDERLINE: return "underline";
		STYLE_XXSMALL: return "xx-small";
		STYLE_XSMALL: return "x-small";
		STYLE_SMALL: return "small";
		STYLE_LARGE: return "large";
		STYLE_XLARGE: return "x-large";
		STYLE_XXLARGE: return "xx-large";
		STYLE_RED_LETTERS: return "red-foreground";
		STYLE_GREEN_LETTERS: return "green-foreground";
		STYLE_YELLOW_LETTERS: return "yellow-foreground";
		STYLE_BLUE_LETTERS: return "blue-foreground";
		STYLE_MAGENTA_LETTERS: return "magenta-foreground";
		STYLE_CYAN_LETTERS: return "cyan-foreground";
		STYLE_WHITE_LETTERS: return "white-foreground";
		STYLE_BLACK_LETTERS: return "black-foreground";
		STYLE_RED_BACKGROUND: return "red-background";
		STYLE_GREEN_BACKGROUND: return "green-background";
		STYLE_YELLOW_BACKGROUND: return "yellow-background";
		STYLE_BLUE_BACKGROUND: return "blue-background";
		STYLE_MAGENTA_BACKGROUND: return "magenta-background";
		STYLE_CYAN_BACKGROUND: return "cyan-background";
		STYLE_WHITE_BACKGROUND: return "white-background";
		STYLE_BLACK_BACKGROUND: return "black-background";
		default: return "cursive";
	}
];

[ VorpleStyle style ;
	VorpleExecuteJavaScriptCommand(BuildCommand("vorple.parser.openTag('span','", StyleName(style), "')"));
];

[ VorpleEndAllStyles ;
	VorpleExecuteJavaScriptCommand("vorple.parser.closeAllTags()");
];

[ VorpleEndStyle ;
	VorpleExecuteJavaScriptCommand("vorple.parser.closeTag()");
];


!==================================================
! Colors

Constant DEFAULT_STYLE=	1;
Constant BLACK =		2;
Constant RED =			3;
Constant GREEN =		4;
Constant YELLOW =		5;
Constant BLUE =			6;
Constant VIOLET =		7;
Constant PURPLE =		7;
Constant MAGENTA 		7;
Constant CYAN =			8;
Constant WHITE =		9;


[ ColorName color ;
	switch(color) {
		BLACK: return "black";
		RED: return "red";
		GREEN: return "green";
		YELLOW: return "yellow";
		BLUE: return "blue";
		MAGENTA: return "magenta";
		CYAN: return "cyan";
		WHITE: return "white";
	}
];

[ VorpleDefaultStyle ;
	if (IsVorpleSupported()) {
		VorpleEndStyle();
	} else { @set_colour 1 1; }
];

! To say red letters (letters written in red)
[ VorpleColorLetters color ;
	if (IsVorpleSupported()) {
		if (color == DEFAULT_STYLE or 0) { VorpleDefaultStyle(); }
		else { VorpleStyle(color+13); }
	} else { @set_colour color 0; }
];

! To say red background (background of letters from then on to the next EndStyle is red)
[ VorpleColorLettersBackground color ;
	if (IsVorpleSupported()) {
		if (color == DEFAULT_STYLE or 0) { VorpleDefaultStyle(); }
		else { VorpleStyle(color+21); }
	} else { @set_colour 0 color; }
];

! To turn the foreground red (the whole page has red foreground)
[ VorpleForegroundColor color ;
	if (IsVorpleSupported()) {
		VorpleExecuteJavaScriptCommand(BuildCommand("$('#vorpleContainer')[0].className=$('#vorpleContainer')[0].className.replace(/@@92b[a-z]+@@92-foreground/,'');$('#vorpleContainer').addClass('", ColorName(color), "-foreground')"));
	} else { @set_colour color 0; }
];

! To turn the background red (the whole page has red background)
[ VorpleBackgroundColor color ;
	if (IsVorpleSupported()) {
		VorpleExecuteJavaScriptCommand(BuildCommand("$('#vorpleContainer')[0].className=$('#vorpleContainer')[0].className.replace(/@@92b[a-z]+@@92-background/,'');$('#vorpleContainer').addClass('", ColorName(color), "-background')"));
	} else { @set_colour 0 color; }
];


!=============================================
! Spacing and pausing and clearing


[ PressSpace i;
	i=0;
	while (i ~= 13 or 31 or 32) { i = KeyCharPrimitive(); }
];

! 0 = WIN_ALL (window + status bar), 1 = WIN_STATUS, 2 = WIN_MAIN
[ VorpleClearScreen window;
	if (IsVorpleSupported() && window ~= WIN_STATUS) {
		VorpleExecuteJavaScriptCommand("$(vorple.parser._container.top).empty()");
	} else {
		ClearScreen(window);
	}
];

[ VorplePause ;
	print "^Please press SPACE to continue";
	PressSpace();
	VorpleClearScreen();
];


!==============================
! Text alignment

Constant STYLE_FIXED_WIDTH = STYLE_MONOSPACE;

[ VorpleCenterText str fixedwidth 		i j len;
	if (isVorpleSupported()) {
		if (fixedwidth == STYLE_MONOSPACE) {
			VorpleOpenHTMLTag("div", "centered");
			VorplePlaceInlineElement("monospace", str);
			VorpleCloseHTMLTag();
		} else {
			VorplePlaceBlockLevelElement("centered", str);
		}
	} else {
		! Centering in Z-machine - always fixed-width, otherwise ugly
		font off;
		print "^";
		i = ScreenWidth();
		len = str-->0;
		if (len > 63) { len = 63; }
		j = (i-len)/2 -1;
		Print__Spaces(j);
		print (PrintStringOrArray) str;
		font on;
	}
];

[ VorpleCenterTextAtRow str depth 		i j len ;
	font off;
	i = ScreenWidth();
	len = str-->0;
	if (len > 63) { len = 63; }
	j = (i-len)/2 - 1;
	MoveCursor(depth, j);
	print (PrintStringOrArray) str;
	font on;
];


[ VorpleRightAlign str ;
	! should we attempt something if IsVorpleSupported() == 0?
	VorplePlaceBlockLevelElement("right-aligned", str);
];


! === Blockquotes

[ VorplePlaceBlockquoteElement classes txt ;
	VorplePlaceElement("blockquote", classes, txt);
];

[ VorpleHideLastBlockquote ;
	VorpleExecuteJavaScriptCommand("$('blockquote:last').animate({opacity:0},500).slideUp(500,function(){$(this).remove()}");
];

[ VorpleHideAllBlockquotes ;
	VorpleExecuteJavaScriptCommand("$('blockquote').animate({opacity:0},500).slideUp(500,function(){$(this).remove()}");
];



!======== Status line
!  The way status lines are handled is different in I7 (through tables)
!  So if you want to draw your own status line, you have to use
!       Replace DrawStatusLine
!  before including "parser".
!  The following routines help you write your own DrawStatusLine.

[ VorpleScreenHeight		i screen_height ;
	i = 0->32;
		  if (screen_height == 0 or 255) screen_height = 18;
		  screen_height = screen_height - 7;
	return screen_height;
];

[ VorpleScreenWidth ;    ! this is only to avoid answering questions
	return ScreenWidth();
];

[ VorpleDeepStatus depth 		screen_width ;		! i screen_width ;
    StatusLineHeight(depth);
    screen_width = ScreenWidth();
    ClearScreen(1);
!	Here is Vorple's I6 code for this, which I don't understand - why wouldn't ClearScreen(1) work in Z-code?
!    #ifdef TARGET_GLULX;
!        ClearScreen(1);
!    #ifnot;
!        style reverse;
!        for (i=1:i<depth+1:i++)
!        {
!             @set_cursor i 1;
!             spaces(screen_width);
!        }
!    #endif;
];

[ VorpleSelectLineInStatus depth ;
	MoveCursor(depth, 1);
];


Global Vorple_right_alignment_depth = 14;

[ VorpleRightAlignStatusLine depth        screen_width o n;
	screen_width = ScreenWidth();
	n = Vorple_right_alignment_depth;
	o = screen_width - n;
	MoveCursor(depth, o);
];
