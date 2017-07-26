#Ifndef VORPLE_NOTIFICATIONS;

System_file;
Constant VORPLE_NOTIFICATIONS;	! to let vorple know this is included

Include "vorple.h";


! "Notifications that display a short message on the screen and disappear after a few seconds."


!========================================
! Displaying a notification
! Vorple uses toastr and its built-in types

Constant NOTIFICATION_INFO = 1;
Constant NOTIFICATION_SUCCESS = 2;
Constant NOTIFICATION_WARNING = 3;
Constant NOTIFICATION_ERROR = 4;

! by default, notifications last for 7 seconds
Global NOTIFICATION_DURATION = 7;

[ VorpleNotification text type title duration   typestr ;
        switch(type){
            NOTIFICATION_INFO: typestr="info";
            NOTIFICATION_SUCCESS: typestr="success";
            NOTIFICATION_WARNING: typestr="warning";
            NOTIFICATION_ERROR: typestr="error";
            default: typestr="info";
        }
        if (duration == 0) { duration = NOTIFICATION_DURATION; }
        if (title == 0) {
            VorpleExecuteJavaScriptCommand(BuildCommand("toastr.", typestr, "('", VorpleEscape(text), "',{timeOut: ", IntToString(duration), "000, escapeHtml: true})"));
        } else {
            ! we have two strings to escape, but the way the utils work it's easier to do it this way
            !@output_stream 3 hugehugestr;
            bp_output_stream(3, hugehugestr, 2000);
            print "toastr.";
            PrintStringOrArray(typestr);
            print "('";
            PrintStringOrArray(VorpleEscape(text));
            print "','";
            PrintStringOrArray(VorpleEscape(title));
            print "',{timeOut: ";
            PrintStringOrArray(IntToString(duration));
            print "000, escapeHtml: true})";
            !@output_stream -3;
            bp_output_stream(-3);
            VorpleExecuteJavaScriptCommand(hugehugestr);
        }
        VorpleInsertNotification(text);
];


!=============================================
! The same behavior, but for interpreters for which Vorple isn't supported

Constant VORPLE_MAX_NUMBER_NOTIFS = 16;

Array displayednotifs table VORPLE_MAX_NUMBER_NOTIFS;
Global Vorple_notifarraysinit = 0;


! TODO: does this code work for glulx?! (-->i et tout...)

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

	if (isVorpleSupported() == 0) {
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

#Endif;