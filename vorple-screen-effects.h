#Ifndef VORPLE_SCREEN_EFFECTS;

System_file;
Constant VORPLE_SCREEN_EFFECTS;

Include "vorple.h";

! Vorple equivalent of Basic Screen Effects by Emily Short. Waiting for a keypress, clearing the screen, aligning, styling and coloring text.



!================================
! Utilities

[ VorpleScreenHeight		i screen_height ;
    if (isVorpleSupported()) {
        return 24;
    } else {
        i = 0->32;
        if (screen_height == 0 or 255) screen_height = 18;
        screen_height = screen_height - 7;
        return screen_height;
    }
];

[ VorpleScreenWidth ;
    if (isVorpleSupported()) {
        return 80; ! FIXME when juhana replied
    } else {
	return ScreenWidth();
    }
];


!================================
! Aligned text

Constant STYLE_FIXED_WIDTH = STYLE_MONOSPACE;

[ VorpleCenterText str 		i j len;
	if (isVorpleSupported()) {
                VorplePlaceBlockLevelElement("center-align", str);
	} else {
		! Centering in Z-machine - always fixed-width, otherwise ugly
		! TODO: glulx
		font off;
		print "^";
		!i = ScreenWidth();
                !print "i =",i;
		!len = str-->0;       ! THIS IS NOT GOOD gives the wrong length
                !print "len ="; print len;
		!if (len > i-2) { len = i-2; }
		!j = (i-len)/2 -1;
                !print "j =",j;
		!!!!!!!Print__Spaces(j);
		print (PrintStringOrArray) str;
		font on;
	}
];


[ VorpleRightAlign str ;
        !should we attempt something if IsVorpleSupported() == 0?
        VorplePlaceBlockLevelElement("right-aligned", str);
];




!=======================================
! Boxed quotations





! TODO: what does this do?
!
! Section 3 - Displaying boxed quotations (in place of Section SR5/1/7 - Saying - Fonts and visual effects in Standard Rules by Graham Nelson)
! 
! To say bold type -- running on
!	(documented at phs_bold):
!	(- style bold; -).
! To say italic type -- running on
!	(documented at phs_italic):
!	(- style underline; -).
! To say roman type -- running on
!	(documented at phs_roman):
!	(- style roman; -).
! To say fixed letter spacing -- running on
!	(documented at phs_fixedspacing):
!	(- font off; -).
! To say variable letter spacing -- running on
!	(documented at phs_varspacing):
!	(- font on; -).
! To display the boxed quotation (Q - text) in Glulx
!	(documented at ph_boxed):
!	(- DisplayBoxedQuotation({-box-quotation-text:Q}); -).


[ VorpleBoxedQuotation text ;
    if (isVorpleSupported()) {
        VorplePlaceElement("blockquote", 0, text);
    } else {
        ! TODO: support for multi-line box quotations in the glulx fallback
        ! note: "box text;" doesn't work!...
        return ;
    }
];







!=================================================
! Styles and colors


Constant STYLE_CURSIVE = 0;
Constant STYLE_EMPHASIS = 1;
Constant STYLE_FANTASY = 2;
Constant STYLE_IMPACT = 2;
Constant STYLE_MONOSPACE = 3;
Constant STYLE_NOWRAP = 4;
Constant STYLE_STRIKETHROUGH = 5;
Constant STYLE_STRONG = 6;
! Constant STYLE_TRANSIENT = 7;
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
Constant STYLE_BROWN_LETTERS = 31;
Constant STYLE_DARK_GRAY_LETTERS = 32;
Constant STYLE_LIGHT_GRAY_LETTERS = 33;
Constant STYLE_LIGHT_BLUE_LETTERS = 34;
Constant STYLE_LIGHT_GREEN_LETTERS = 35;
Constant STYLE_LIGHT_CYAN_LETTERS = 36;
Constant STYLE_LIGHT_RED_LETTERS = 37;
Constant STYLE_LIGHT_MAGENTA_LETTERS = 38;

Constant STYLE_BLACK_BACKGROUND = 23;
Constant STYLE_RED_BACKGROUND = 24;
Constant STYLE_GREEN_BACKGROUND = 25;
Constant STYLE_YELLOW_BACKGROUND = 26;
Constant STYLE_BLUE_BACKGROUND = 27;
Constant STYLE_MAGENTA_BACKGROUND = 28;
Constant STYLE_CYAN_BACKGROUND = 29;
Constant STYLE_WHITE_BACKGROUND = 30;
Constant STYLE_BROWN_BACKGROUND = 39;
Constant STYLE_DARK_GRAY_BACKGROUND = 40;
Constant STYLE_LIGHT_GRAY_BACKGROUND = 41;
Constant STYLE_LIGHT_BLUE_BACKGROUND = 42;
Constant STYLE_LIGHT_GREEN_BACKGROUND = 43;
Constant STYLE_LIGHT_CYAN_BACKGROUND = 44;
Constant STYLE_LIGHT_RED_BACKGROUND = 45;
Constant STYLE_LIGHT_MAGENTA_BACKGROUND = 46;

[ StyleName sty ;
	if (sty ofclass string) { return sty; }
	switch(sty) {
		STYLE_CURSIVE: return "cursive font";
		STYLE_EMPHASIS: return "emphasized font";
		STYLE_FANTASY: return "fantasy font";
		STYLE_MONOSPACE: return "monospace font";
		STYLE_NOWRAP: return "nowrap font";
		STYLE_STRIKETHROUGH: return "strikethrough font";
		STYLE_STRONG: return "strong font";
!		STYLE_TRANSIENT: return "transient";
		STYLE_UNDERLINE: return "underlined font";
		STYLE_XXSMALL: return "xx-small font";
		STYLE_XSMALL: return "x-small font";
		STYLE_SMALL: return "small font";
		STYLE_LARGE: return "large font";
		STYLE_XLARGE: return "x-large font";
		STYLE_XXLARGE: return "xx-large font";
		STYLE_RED_LETTERS: return "red letters";
		STYLE_GREEN_LETTERS: return "green letters";
		STYLE_YELLOW_LETTERS: return "yellow letters";
		STYLE_BLUE_LETTERS: return "blue letters";
		STYLE_MAGENTA_LETTERS: return "magenta letters";
		STYLE_CYAN_LETTERS: return "cyan letters";
		STYLE_WHITE_LETTERS: return "white letters";
		STYLE_BLACK_LETTERS: return "black letters";
                STYLE_BROWN_LETTERS: return "brown letters";
                STYLE_DARK_GRAY_LETTERS: return "dark gray letters";
                STYLE_LIGHT_GRAY_LETTERS: return "light gray letters";
                STYLE_LIGHT_BLUE_LETTERS: return "light blue letters";
                STYLE_LIGHT_GREEN_LETTERS: return "light green letters";
                STYLE_LIGHT_CYAN_LETTERS: return "light cyan letters";
                STYLE_LIGHT_RED_LETTERS: return "light red letters";
                STYLE_LIGHT_MAGENTA_LETTERS: return "light magenta letters";
                
		STYLE_RED_BACKGROUND: return "red background";
		STYLE_GREEN_BACKGROUND: return "green background";
		STYLE_YELLOW_BACKGROUND: return "yellow background";
		STYLE_BLUE_BACKGROUND: return "blue background";
		STYLE_MAGENTA_BACKGROUND: return "magenta background";
		STYLE_CYAN_BACKGROUND: return "cyan background";
		STYLE_WHITE_BACKGROUND: return "white background";
		STYLE_BLACK_BACKGROUND: return "black background";
                STYLE_BROWN_BACKGROUND: return "brown background";
                STYLE_DARK_GRAY_BACKGROUND: return "dark gray background";
                STYLE_LIGHT_GRAY_BACKGROUND: return "light gray background";
                STYLE_LIGHT_BLUE_BACKGROUND: return "light blue background";
                STYLE_LIGHT_GREEN_BACKGROUND: return "light green background";
                STYLE_LIGHT_CYAN_BACKGROUND: return "light cyan background";
                STYLE_LIGHT_RED_BACKGROUND: return "light red background";
                STYLE_LIGHT_MAGENTA_BACKGROUND: return "light magenta background";
		default: return "cursive";
	}
];

[ VorpleStyle style ;
    VorpleExecuteJavaScriptCommand(BuildCommand("vorple.layout.openTag('span', '", StyleName(style),"'.split(' ').join('-').toLowerCase())"));
];

[ VorpleEndStyle ;
    VorpleCloseHTMLTag();
];

!!!!!  -- not ready yet --

[ VorpleApplyStyleToPage sty ;
    return ;
    ! VorpleExecuteJavaScriptCommand(BuildCommand("$('#vorpleContainer')[0].className=$('#vorpleContainer')[0].className.replace(/\b[a-z]+\-foreground/,'');$('#vorpleContainer').addClass('", color, "-foreground')"));
];

! TODO: how do i make it so that when opacity is not set (i.e. by default) it should be 100 ?
[ VorpleSetElementTextColor classes red green blue opacity ;
    ! set the text color to RGB, + opacity in percent
    return ;
];

[ VorpleSetElementsTextColor classes red green blue opacity ;
    VorpleSetElementTextColor(BuildCommand(classes, ":last"), red, green, blue, opacity);
];

[ VorpleSetElementBackgroundColor classes red green blue opacity ;
    ! set background color to RGB, + opacity in percent
    return ;
];

[ VorpleSetElementsBackgroundColor classes red green blue opacity ;
    VorpleSetElementBackgroundColor(BuildCommand(classes, ":last"), red, green, blue, opacity);
];


! TODO: is there a way to have Z-machine fallback for this? Here's the old code:
!
!
![ VorpleDefaultStyle ;
!	} else { @set_colour 1 1; }
!];
!
! To say red letters (letters written in red)
![ VorpleColorLetters color ;
!	} else { @set_colour color 0; }
!];
!
! To say red background (background of letters from then on to the next EndStyle is red)
![ VorpleColorLettersBackground color ;
!	} else { @set_colour 0 color; }
!];
!
! To turn the foreground red (the whole page has red foreground)
![ VorpleForegroundColor color ;
!	} else { @set_colour color 0; }
!];
!
! To turn the background red (the whole page has red background)
![ VorpleBackgroundColor color ;
!	} else { @set_colour 0 color; }
!];



!!!!



!=============================
! Headers

[ VorplePlaceHeader level text classes ;
    if (classes == 0) { classes = ""; }
    VorplePlaceElement(BuildCommand("h", IntToString(level)), classes, text);
];


!=============================
! Lists

[ VorpleDisplayOrderedList list classes         len i ;
    if (classes == 0) { classes = ""; }
    ! List must be a table, like page 43 of DM4
    len = list-->0;
    VorpleOpenHTMLTag("ol", classes);
    if (isVorpleSupported()) {
        for (i=1: i<=len: i++) {
            VorplePlaceElement("li", 0, list-->i);
        }
    } else {
        for (i=1: i<=len: i++) {
            print " "; print (string) IntToString(i); print ". "; print (string) list-->i; print "^";
        }
    }
    VorpleCloseHTMLTag();
];

[ VorpleDisplayUnorderedList list classes         len i ;
    if (classes == 0) { classes = ""; }
    ! List must be a table, like page 43 of DM4
    len = list-->0;
    VorpleOpenHTMLTag("ul", classes);
    if (isVorpleSupported()) {
        for (i=1: i<=len: i++) {
            VorplePlaceElement("li", 0, list-->i);
        }
    } else {
        for (i=1: i<=len: i++) {
            print " * "; print (string) list-->i; print "^";
        }
    }
    VorpleCloseHTMLTag();
];


#Endif;