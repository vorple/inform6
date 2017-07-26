#Ifndef VORPLE_MODAL_WINDOWS;

System_file;
Constant VORPLE_MODAL_WINDOWS;

Include "vorple.h";

! Modal windows are dialog prompts or other information windows that pop up on top of the play area and require user action to dismiss.


[ VorpleShowModalWindow text ;
    if (text == 0) { text = ""; }
    if (isVorpleSupported()) {
        VorpleExecuteJavaScriptCommand(BuildCommand("vex.closeAll();vex.dialog.open({message:'", VorpleEscapeLineBreaks(text, "@@92n"), "',buttons:[vex.dialog.buttons.YES],callback:vorple.layout.unblock});vorple.layout.block()"));
    } else { print (string) text; }
];

[ VorpleSetFocusToModalWindow ;
    VorpleSetOutputFocus("vex-dialog-message");        ! defined in vorple.h
];

[ VorpleCloseModalWindow ;
    VorpleExecuteJavaScriptCommand("vex.closeAll()");
];

#Endif;