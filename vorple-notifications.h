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
            VorpleInsertNotification(text);
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
            ! Fallback if vorple isn't supported
            VorpleInsertNotification(BuildCommand(title, " - ", text));
        }
];


!=============================================
! The same behavior, but for interpreters for which Vorple isn't supported

Constant LEN_NOTIFICATIONS_FALLBACK = 2000;
Array notifs_array buffer LEN_NOTIFICATIONS_FALLBACK;
Array notifs_copy buffer LEN_NOTIFICATIONS_FALLBACK;
Global notifs=0;
Global Vorple_notifarraysinit=0;

[ VorpleInsertNotification str ;
    if (Vorple_notifarraysinit==0) {
        bp_output_stream(3, notifs_array, LEN_NOTIFICATIONS_FALLBACK);
        print "";
        bp_output_stream(-3);
        Vorple_notifarraysinit=1;
    }
    ! TODO: is there really nothing better??
    bp_output_stream(3, notifs_copy, LEN_NOTIFICATIONS_FALLBACK);
    PrintStringOrArray(notifs_array);
    bp_output_stream(-3);
    bp_output_stream(3, notifs_array, LEN_NOTIFICATIONS_FALLBACK);
    PrintStringOrArray(notifs_copy);
    print "[";
    PrintStringOrArray(str);
    print "]^";
    bp_output_stream(-3);
    if (notifs_array->0 >= LEN_NOTIFICATIONS_FALLBACK-1) {
        style bold;
        print "Vorple error: notification table too small.^";
        style roman;
    }
    notifs++;
];


[ VorpleNotificationsFallback  ;
	if (isVorpleSupported() == 0 && notifs > 0) {
		style underline;
                PrintStringOrArray(notifs_array);
		style roman;
	}
        notifs = 0;
	bp_output_stream(3, notifs_array, LEN_NOTIFICATIONS_FALLBACK);
        print "";
        bp_output_stream(-3);
];

#Endif;