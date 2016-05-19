Include "vorple.h";
System_file;

!============================================
! Tooltips that can be activated on request or when the mouse cursor is moved over an element.




!======================================
! Tooltips on mouseover

[ VorpleTooltipMouse text tip    id;
	! generate a unique identifier
	id = UniqueIdentifier();
	! place the text
	VorplePlaceInlineElement(id, text);
	! queue the command
	VorpleQueueJavaScriptCommand(BuildCommand("vorple.tooltip.enable('.", id, "',{content:{text:'", VorpleEscape(tip), "'}})"));
];

[ VorpleTooltipAllElementsMouse tip classes;
	VorpleQueueJavaScriptCommand(BuildCommand("vorple.tooltip.enable('.", classes, "','", VorpleEscape(tip),"')"));
];

[ VorpleTooltipElementMouse tip classes;
	VorpleTooltipAllElementsMouse(tip, BuildCommand(classes, ":last"));
];


!=======================================
! Tooltips triggered manually

[ VorpleTooltipAllElements tip classes tenths ;
	VorpleQueueJavaScriptCommand(BuildCommand("setTimeout(function(){vorple.tooltip.show('.", classes, "','", VorpleEscape(tip), "')},", IntToString(tenths), ")"));
];

[ VorpleTooltipElement tip classes tenths ;
	VorpleTooltipAllElements(tip, BuildCommand(classes, ":last"), tenths);
];

[ VorpleTooltipPrompt tip tenths ;
!	if (tenths==0) { tenths = 1; }
	VorpleTooltipElement(tip, "vorplePrompt input", tenths);
];