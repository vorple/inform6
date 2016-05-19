Include "vorple.h";
System_file;

Constant VORPLE_NOTIFICATIONS_LIBRARY;	! to let vorple know this is included

! "Notifications and dialogs."

!============================================
! Element positions and their names

Constant VORPLE_POSITION_TOP_LEFT = 0;
Constant VORPLE_POSITION_TOP_CENTER = 1;
Constant VORPLE_POSITION_TOP_RIGHT = 2;
Constant VORPLE_POSITION_LEFT_TOP = 3;
Constant VORPLE_POSITION_RIGHT_TOP = 4;
Constant VORPLE_POSITION_LEFT_CENTER = 5;
Constant VORPLE_POSITION_CENTER_LEFT = 6;
Constant VORPLE_POSITION_SCREEN_CENTER = 7;
Constant VORPLE_POSITION_RIGHT_CENTER = 8;
Constant VORPLE_POSITION_CENTER_RIGHT = 9;
Constant VORPLE_POSITION_LEFT_BOTTOM = 10;
Constant VORPLE_POSITION_RIGHT_BOTTOM = 11;
Constant VORPLE_POSITION_BOTTOM_LEFT = 12;
Constant VORPLE_POSITION_BOTTOM_CENTER = 13;
Constant VORPLE_POSITION_BOTTOM_RIGHT = 14;
Constant VORPLE_POSITION_TOP_BANNER = 15;
Constant VORPLE_POSITION_BOTTOM_BANNER = 16;

! noty name
[ VorplePositionName pos ;
	switch(pos) {
		VORPLE_POSITION_TOP_BANNER: return "top";
		VORPLE_POSITION_BOTTOM_BANNER: return "bottom";
		VORPLE_POSITION_TOP_LEFT: return "topLeft";
		VORPLE_POSITION_TOP_CENTER: return "topCenter";
		VORPLE_POSITION_TOP_RIGHT: return "topRight";
		VORPLE_POSITION_CENTER_LEFT: return "centerLeft";
		VORPLE_POSITION_SCREEN_CENTER: return "center";
		VORPLE_POSITION_CENTER_RIGHT: return "centerRight";
		VORPLE_POSITION_BOTTOM_LEFT: return "bottomLeft";
		VORPLE_POSITION_BOTTOM_CENTER: return "bottomCenter";
		VORPLE_POSITION_BOTTOM_RIGHT: return "bottomRight";
		default: return "";
	}
];


!=============================================
! Functions

Constant VORPLE_MAX_NUMBER_NOTIFS = 16;

Array displayednotifs table VORPLE_MAX_NUMBER_NOTIFS;
Global Vorple_notifarraysinit = 0;

[ VorpleNotification str pos ;
	if (pos == 0) {
		VorpleExecuteJavaScriptCommand(BuildCommand("vorple.notify.show('", VorpleEscape(str), "')"));
	} else {
		VorpleExecuteJavaScriptCommand(BuildCommand("vorple.notify.show('", VorpleEscape(str),"',{layout:'", VorplePositionName(pos), "'})"));
	}
	VorpleInsertNotification(str);
];

[ VorpleAlert str ;
	VorpleExecuteJavaScriptCommand(BuildCommand("vorple.notify.alert('", VorpleEscape(str), "')"));
	VorpleInsertNotification(str);
];

[ VorpleHideNotifications ;
	VorpleExecuteJavaScriptCommand("vorple.notify.closeAll()");
];

[ VorpleDefaultNotifPosition pos;
	VorpleExecuteJavaScriptCommand(BuildCommand("vorple.notify.defaults.layout='", VorplePositionName(pos), "'"));
];



! Implementation + Fallback for the Z-machine

[ VorpleInsertNotification str         i ;
	if (Vorple_notifarraysinit == 0) {
		! need to initialise the array first
		for (i=0: i<VORPLE_MAX_NUMBER_NOTIFS: i++) {
				displayednotifs-->i = 0;
		}
		Vorple_notifarraysinit=1;
	}

	for (i=0: i<VORPLE_MAX_NUMBER_NOTIFS: i++) {
		if ( displayednotifs-->i == 0) { break;}
	}
	if (i >= VORPLE_MAX_NUMBER_NOTIFS) { style bold; print "Vorple error: notification table too small.^"; style roman; }
	displayednotifs-->i = str;
];


Object vorple_notif
	with found_in [; rtrue; ],
	each_turn [; VorpleNotificationsFallback(); rtrue; ],
has scenery concealed;

[ VorpleNotificationsFallback     i ;
	if (Vorple_notifarraysinit == 0) {
		! need to initialise the array first
		for (i=0: i<VORPLE_MAX_NUMBER_NOTIFS: i++) {
				displayednotifs-->i = 0;
		}
		Vorple_notifarraysinit=1;
	}

	if (IsVorpleSupported() == 0) {
		style underline;
		for (i=0: i<VORPLE_MAX_NUMBER_NOTIFS: i++) {
			if ( displayednotifs-->i ~= 0) {
				print "^[";
				print (PrintStringOrArray) displayednotifs-->i;
				print "]";
			}
		}
		style roman;
	}
	for (i=0: i<VORPLE_MAX_NUMBER_NOTIFS: i++) {
		displayednotifs-->i = 0;
	}
];
