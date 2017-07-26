#Ifndef VORPLE_COMMAND_PROMPT_CONTROL;

System_file;
Constant VORPLE_COMMAND_PROMPT_CONTROL;

Include "vorple.h";

! Changing the style and contents of the command prompt and manipulating the command history.


!=========================
! Queueing parser commands

Constant SHOW_QUEUED_COMMAND = 0;
Constant HIDE_QUEUED_COMMAND = 1;

[ VorpleQueueCommand cmd show      hide ;
    if (show == HIDE_QUEUED_COMMAND) { hide = "true"; } else { hide = "false"; }
    VorpleExecuteJavaScriptCommand(BuildCommand("vorple.prompt.queueCommand('", VorpleEscape(cmd), "',", hide, ")"));
];


!=========================
! Command history

[ VorpleAddCommandToHistory cmd ;
    VorpleExecuteJavaScriptCommand(BuildCommand("haven.prompt.history.add('", VorpleEscape(cmd), "')"));
];

[ VorpleRemoveLastCommandInHistory ;
    VorpleExecuteJavaScriptCommand("haven.prompt.history.remove()");
];

[ VorpleChangeLastCommandInHistory cmd ;
    VorpleRemoveLastCommandInHistory();
    VorpleAddCommandToHistory(cmd);
];

[ VorpleClearCommandHistory ;
    VorpleExecuteJavaScriptCommand("haven.prompt.history.clear()");
];



!=========================
! Prefilling the command line

[ VorplePrefillPrompt cmd ;
    VorpleExecuteJavaScriptCommand(BuildCommand("vorple.prompt.setValue('", VorpleEscape(cmd), "')"));
];



!=========================
! Changing the previous command

[ VorpleChangeLastCommand cmd ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.lineinput.last .prompt-input').text('", VorpleEscape(cmd), "')"));
];



!=========================
! Hiding the prompt

[ VorpleHidePrompt ;
    VorpleExecuteJavaScriptCommand("vorple.prompt.hide()");
];

[ VorpleUnhidePrompt ;
    VorpleExecuteJavaScriptCommand("vorple.prompt.unhide()");
];

[ VorpleShowPrompt ;
    VorpleUnhidePrompt();
];

#Endif;