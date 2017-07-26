#Ifndef VORPLE_HYPERLINKS;

System_file;
Constant VORPLE_HYPERLINKS;

Include "vorple.h";

! Hyperlinks that either open a web site, execute a parser command or evaluate JavaScript code.

! this is a horribly hacky way to implement the rules in the rulebook
Object hyperlinksrule1 "web link event handler rule" VorpleInterfaceSetup
    with description [; VorpleExecuteJavaScriptCommand("$(document).on('click', 'a.vorple-link.vorple-commandlink', function(e) { e.preventDefault(); vorple.prompt.queueCommand($(this).data('command'), $(this).data('silent'))})"); ];


!=======================================
! Link to a website

Constant SAME_WINDOW = 1;
Constant NOT_SAME_WINDOW = 0;

[ VorpleLinkWebsiteText url text sameWindowOrNot classes     id;
	id = UniqueIdentifier();
	VorplePlaceElement("a", BuildCommand("vorple-link vorple-weblink link-", id, " ", classes), text);
	VorpleExecuteJavaScriptCommand(BuildCommand("$('a.link-", id, "').attr('href','", VorpleEscape(url),"')"));
	if (sameWindowOrNot == 0) {
		VorpleExecuteJavaScriptCommand(BuildCommand("$('a.link-", id, "').attr('target','_blank')"));
	}
];

[ VorpleLinkWebsite url sameWindowOrNot ;
	VorpleLinkWebsiteText(url, url, sameWindowOrNot, "");
];


!=======================================
! Link to a parser command

Constant DONT_SHOW_COMMAND = 1;

[ VorpleLinkCommandText cmd text dontShow classes       id silent;
        if (dontShow == DONT_SHOW_COMMAND) {
            silent = "true";
        } else { silent = "false"; }
	id = UniqueIdentifier();
	VorplePlaceElement("a", BuildCommand("vorple-link vorple-commandlink link-", id, " ", classes), text);
	VorpleExecuteJavaScriptCommand(BuildCommand("$('a.link-", id, "').attr('href', '#').attr('data-command', '", VorpleEscape(cmd), "').attr('data-silent', '", silent, "')"));
];

[ VorpleLinkCommand cmd dontShow classes ;
	VorpleLinkCommandText(cmd, cmd, dontShow, classes);
];


!=======================================
! Link to a JavaScript command

[ VorpleLinkJavaScript cmd text classes     id ;
        id = UniqueIdentifier();
        VorplePlaceElement("a", BuildCommand("vorple-link vorple-jslink link-", id, " ", classes), text);
        VorpleExecuteJavaScriptCommand(BuildCommand("$('a.link-", id, "').attr('href','#').on('click', function(e) {e.preventDefault();", cmd, "})"));
];


!=======================================
! Disable links

[ VorpleLinkDisableName classes ;
        VorpleExecuteJavaScriptCommand(BuildCommand("$('a.vorple-link.", classes, "').replaceWith(function() { return $('<span>').addClass('vorple-disabled-link ", classes, "').html($(this).html())}).length"));
];

[ VorpleLinkDisableInElement elem ;
        VorpleExecuteJavaScriptCommand(BuildCommand("$('.", elem, " a.vorple-link').replaceWith(function() { return $('<span>').addClass('vorple-disabled-link').html($(this).html())}).length"));
];

[ VorpleLinkDisableAll ;
        VorpleExecuteJavaScriptCommand("$('a.vorple-link').replaceWith(function() { return $('<span>').addClass('vorple-disabled-link').html($(this).html())}).length");
];

#Endif;