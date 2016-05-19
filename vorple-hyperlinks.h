Include "vorple.h";
System_file;

! Hyperlinks that either open a web site or execute a parser command.


! Link to a website

Constant SAME_WINDOW = 1;
Constant NOT_SAME_WINDOW = 0;

[ VorpleLinkWebsiteText url text sameWindowOrNot      id;
	id = UniqueIdentifier();
	VorplePlaceElement("a", BuildCommand(id, " webLink"), text);
	VorpleExecuteJavaScriptCommand(BuildCommand("$('.", id, "').attr('href','", VorpleEscape(url),"')"));
	if (sameWindowOrNot == 0) {
		VorpleExecuteJavaScriptCommand(BuildCommand("$('.", id, "').attr('target','_blank')"));
	}
];

[ VorpleLinkWebsite url sameWindowOrNot ;
	VorpleLinkWebsiteText(url, url, sameWindowOrNot);
];



! Link to a parser command

Constant DONT_SHOW_COMMAND = 1;
Constant DONT_SHOW_RESPONSE = 2;
Constant DONT_SHOW_COMMAND_OR_RESPONSE = 3;

[ VorpleLinkCommandText cmd text dontShow       id classes;
	id = UniqueIdentifier();
	classes = BuildCommand(id, " commandLink");
	if ( dontShow && 1 ) { classes = BuildCommand(classes, " hideCommand"); } ! don't show command
	if ( dontShow && 2 ) { classes = BuildCommand(classes, " hideResponse"); } ! don't show response
	VorplePlaceElement("a", classes, text);
	VorpleExecuteJavaScriptCommand(BuildCommand("$('.", id, "').attr('href','", VorpleEscape(cmd), "')"));
];

[ VorpleLinkCommand cmd dontShow ;
	VorpleLinkCommandText(cmd, cmd, dontShow);
];