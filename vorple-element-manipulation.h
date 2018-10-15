#Ifndef VORPLE_ELEMENT_MANIPULATION;

System_file;
Constant VORPLE_ELEMENT_MANIPULATION;

Include "vorple.h";

! Adding, removing, hiding, moving and other basic manipulation of HTML document elements.



!===========================
! Clear, Remove, Hide, Show

[ VorpleClearElement classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().empty()"));
];

[ VorpleClearAllElementsNamed classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').empty()"));
];



[ VorpleRemoveElement classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().remove()"));
];

[ VorpleRemoveAllElementsNamed classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').remove()"));
];



[ VorpleHideElement classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().hide()"));
];

[ VorpleHideAllElementsNamed classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').hide()"));
];



[ VorpleShowElement classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().show()"));
];

[ VorpleShowAllElementsNamed classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').show()"));
];


!============================
! Move under, at start, before, after


[ VorpleMoveElementUnder classes target ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().appendTo('.", target, "')"));
];

[ VorpleMoveAllElementsUnder classes target ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').appendTo('.", target, "')"));
];



[ VorpleMoveElementAtStart classes target ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().prependTo('.", target, "')"));
];
[ VorpleMoveAllElementsAtStart classes target ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').prependTo('.", target, "')"));
];



[ VorpleMoveElementBefore classes target ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().insertBefore($('.", target, ":last'))"));
];
[ VorpleMoveAllElementsBefore classes target ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').insertBefore($('.", target, ":last'))"));
];



[ VorpleMoveElementAfter classes target ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().insertAfter($('.", target, ":last'))"));
];
[ VorpleMoveAllElementsAfter classes target ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').insertAfter($('.", target, ":last'))"));
];



!=======================
! Add, remove names, and rename

[ VorpleAddNameToElement newname classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().addClass('", newname, "')"));
];
[ VorpleAddNameToAllElements newname classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').addClass('", newname, "')"));
];



[ VorpleRemoveNameFromElement oldname classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().removeClass('", oldname, "')"));
];
[ VorpleRemoveNameFromAllElements oldname classes ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').removeClass('", oldname, "')"));
];



[ VorpleRenameElement oldname newname ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", oldname, "').last().removeClass('", oldname, "').addClass('", newname, "')"));
];
[ VorpleRenameAllElements oldname newname ;
    VorpleExecuteJavaScriptCommand(BuildCommand("$('.", oldname, "').removeClass('", oldname, "').addClass('", newname, "')"));
];


#Endif;