#Ifndef VORPLE_STATUS_LINE;

System_file;
Constant VORPLE_STATUS_LINE;

Include "vorple.h";
Include "vorple-element-manipulation.h";

! Comment this out to not get a status line with full width
!Constant VORPLE_STATUS_LINE_FULL_WIDTH;
! Don't change this number directly - internal use only
Global VorpleStatusLineSize = 0;


!===============================
! Construct the Vorple status line with a given number of columns

! Default is something on the left and something on the right
! Change this if you want more or less
Global VorpleStatusLineColumns = 2;

Object VorpleStatusLineConstructionRule "" VorpleInterfaceSetup
    with description [; 
        VorpleConstructStatusLine(VorpleStatusLineColumns);
    ];

[ VorpleConstructStatusLine cols ;
    if (cols > 3 || cols < 0) {
        VorpleThrowRuntimeError("Vorple Status Line: status line must have exactly 1, 2 or 3 columns.");
        return false;
    }
    VorpleStatusLineSize = cols;
    VorpleStatusLineRemove();
    VorplePlaceElementAtTopLevel("status-line-container");
    #IfDef VORPLE_STATUS_LINE_FULL_WIDTH;
    VorpleExecuteJavaScriptCommand("$('.status-line-container').prependTo('main#haven')");
    #Ifnot;
    VorpleExecuteJavaScriptCommand("$('.status-line-container').prependTo('#output')");
    #Endif;
    VorpleSetOutputFocus("status-line-container");
    if (cols > 1) {
        VorplePlaceBlockLevelElement("status-line-left col-xs lg-only");
    }
    if (cols ~=2) {
        VorplePlaceBlockLevelElement("status-line-middle col-xs lg-only");
    }
    if (cols > 1) {
        VorplePlaceBlockLevelElement("status-line-right col-xs lg-only");
    }
    VorplePlaceBlockLevelElement("status-line-mobile col-xs sm-only");
    VorpleSetOutputFocusMainWindow();
];


!===============================
! Drawing the usual status line


Constant LEN_STATUS 200;
Array VorpleStatusLineLeftHand buffer LEN_STATUS;
Array VorpleStatusLineMiddleHand buffer LEN_STATUS;
Array VorpleStatusLineRightHand buffer LEN_STATUS;
Array VorpleStatusLineMobile buffer LEN_STATUS;
! Put this variable at 0 if you don't want those to be updated with the usual location/score text
Constant VORPLE_USUAL_STATUS_LINE = 1;

Object VorpleDrawStatusLine "" VorpleInterfaceUpdate
    with description [ r ;
        if (VORPLE_USUAL_STATUS_LINE == 1) {
            ! update the columns text using the usual DrawStatusLine
            VorpleStatusLineUsualInfo();
        }
        if (child(StatusLineRulebook) == nothing) {
            VorpleDefaultStatusLine.description();
        } else {
            objectloop(r in StatusLineRulebook) { r.description(); }
        }
        return true;
    ];


Object StatusLineRulebook "status line rulebook";

Object VorpleDefaultStatusLine
    with description [;
        if (VorpleStatusLineSize > 1) {
            VorplePutContentInAllElements("status-line-left", VorpleStatusLineLeftHand);
        }
        if (VorpleStatusLineSize ~= 2) {
            VorplePutContentInAllElements("status-line-middle", VorpleStatusLineMiddleHand);
        }
        if (VorpleStatusLineSize > 1) {
            VorplePutContentInAllElements("status-line-right", VorpleStatusLineRightHand);
        }
        VorplePutContentInAllElements("status-line-mobile", VorpleStatusLineMobile);
        return ;
    ];


! Update the columns text using the usual DrawStatusLine code
[ VorpleStatusLineUsualInfo       width;
    width = ScreenWidth();
    bp_output_stream(3, VorpleStatusLineLeftHand, LEN_STATUS);
    ! TODO: bug: out of bounds memory access??
    !if (location == thedark) {
    !    print (name) location;
    !} else {
    !    FindVisibilityLevels();
    !    if (visibility_ceiling == location)
    !        print (name) location;
    !    else
    !        print (The) visibility_ceiling;
    !}
    print (name) location;
    bp_output_stream(-3);
    
    bp_output_stream(3, VorpleStatusLineMobile, LEN_STATUS);
    PrintStringOrArray(VorpleStatusLineLeftHand);
    bp_output_stream(-3);
    
    bp_output_stream(3, VorpleStatusLineRightHand, LEN_STATUS);
        if (sys_statusline_flag && width > 53) {
            print (string) TIME__TX;
            LanguageTimeOfDay(sline1, sline2);
        } else {
            if (width > 66) {
                #Ifndef NO_SCORE;
                print (string) SCORE__TX, sline1;
                #Endif;
                print "   ";
                print (string) MOVES__TX, sline2;
            }
            #Ifndef NO_SCORE;
            if (width > 53 && width <= 66) {
                print sline1, "/", sline2;
            }
            #Endif;
        }
    bp_output_stream(-3);
];
    
!===============================
! Refresh, remove, clear
    

[ VorpleStatusLineRefresh ;
    if (isVorpleSupported() && VorpleStatusLineSize > 0) {
        ! save the internal state of line breaks
        VorpleDrawStatusLine.description();
        ! restore the internal state of line breaks
    }
];

[ VorpleStatusLineRemove ;
    VorpleRemoveElement("status-line-container");
];

[ VorpleStatusLineClear ;
    VorpleExecuteJavaScriptCommand("$('.status-line-container').children().empty()");
];

#Endif;