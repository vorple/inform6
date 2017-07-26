#Ifndef VORPLE_TOOLTIPS;

System_file;
Constant VORPLE_TOOLTIPS;

Include "vorple.h";


! this is a horribly hacky way to implement the rules in the rulebook
Object tooltiprule1 "initialize tooltip timers rule" VorpleInterfaceSetup
    with description [; VorpleExecuteJavaScriptCommand("window._v_tooltip_timer=null"); ];
Object tooltiprule2 "initalize prompt tooltip rule" VorpleInterfaceSetup
    with description [; VorpleExecuteJavaScriptCommand("$(haven.prompt.get()).find('#lineinput-prefix').powerTip({manual:true,placement:'sw-alt',smartPlacement:true})"); ];
    

!============================================
! Tooltips that can be activated on request or when the mouse cursor is moved over an element.


! by default, tooltips last for 7 seconds
Global TOOLTIP_DURATION = 7;


!======================================
! Tooltips on mouseover

[ VorpleTooltipMouse text tip    id;
	! generate a unique identifier
	id = UniqueIdentifier();
	! place the text
	VorplePlaceInlineElement(id, text);
	! place the tooltip
	VorpleExecuteJavaScriptCommand(BuildCommand("$('.", id, "').last().attr('title','", VorpleEscape(tip), "').powerTip({smartPlacement:true})"));
];

[ VorpleTooltipMouseWithTextName text classes tip    id ;
	! generate a unique identifier
	id = UniqueIdentifier();
	! place the text
	VorplePlaceInlineElement(id, text);
	! place the tooltip
	VorpleExecuteJavaScriptCommand(BuildCommand("$('.", id, "').last().addClass('", classes, "').attr('title','", VorpleEscape(tip), "').powerTip({smartPlacement:true})"));
];

[ VorpleTooltipAllElementsMouse tip classes;
	VorpleExecuteJavaScriptCommand(BuildCommand("$('.", classes, "').last().attr('title','", VorpleEscape(tip),"').powerTip({smartPlacement:true})"));
];

[ VorpleTooltipElementMouse tip classes;
	VorpleTooltipAllElementsMouse(tip, BuildCommand(classes, ":last"));
];




!=======================================
! Tooltips triggered manually

[ VorpleTooltipElementDuration tip classes delay duration       delay_milli duration_milli ;
        if (duration == 0) { duration = TOOLTIP_DURATION;}
        delay_milli = delay*1000;
        duration_milli = duration*1000;
        if (duration > 0) {
            ! because there are two IntToString working on the same array, we have to do it by hand
            !@output_stream 3 hugehugestr;
            bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
            print "clearTimeout(_v_tooltip_timer);_v_tooltip_timer=setTimeout(function() {var $t=$('.";
            PrintStringOrArray(classes);
            print "').last();$.powerTip.hide();$t.attr('title','";
            PrintStringOrArray(VorpleEscape(tip));
            print "').powerTip({manual:true,smartPlacement:true});$.powerTip.show($t);_v_tooltip_timer=setTimeout(function() {$.powerTip.hide($t,true);$t.removeAttr('title')},";
            PrintStringOrArray(IntToString(duration_milli));
            print ");},";
            PrintStringOrArray(IntToString(delay_milli));
            print ")";
            !@output_stream -3;
            bp_output_stream(-3);
            VorpleExecuteJavaScriptCommand(hugehugestr);
        } else {
            VorpleExecuteJavaScriptCommand(BuildCommand("clearTimeout(_v_tooltip_timer);_v_tooltip_timer=setTimeout(function() {var $t=$('.", classes,"').last();$.powerTip.hide();$t.attr('title','", VorpleEscape(tip), "').powerTip({manual:true,smartPlacement:true});$.powerTip.show($t)},", IntToString(delay_milli), ")"));
        }
];

[ VorpleTooltipElementIndefinitely tip classes delay ;
        VorpleTooltipElementDuration(tip, classes, delay, -1);
];

[ VorpleTooltipElement tip classes delay ;
        VorpleTooltipElementDuration(tip, classes, delay, 7);
];


!=======================================
! Tooltips on the prompt

[ VorpleTooltipPromptDuration tip delay duration        delay_milli duration_milli ;
        if (duration == 0) { duration = TOOLTIP_DURATION; }
        delay_milli = delay*1000;
        duration_milli = duration*1000;
        if (duration >= 0) {
            ! because there are two IntToString working on the same array, we have to do it by hand
            !@output_stream 3 hugehugestr;
            bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
            print "clearTimeout(_v_tooltip_timer);_v_tooltip_timer=setTimeout(function() {var $t=$('#lineinput-prefix');$.powerTip.hide();$t.data('powertip','";
            PrintStringOrArray(VorpleEscape(tip));
            print "');$.powerTip.show($t); _v_tooltip_timer=setTimeout(function() {$.powerTip.hide($t,true);$t.removeAttr('title')},";
            PrintStringOrArray(IntToString(duration_milli));
            print ");},";
            PrintStringOrArray(IntToString(delay_milli));
            print ")";
            !@output_stream -3;
            bp_output_stream(-3);
            VorpleExecuteJavaScriptCommand(hugehugestr);
        } else {
            VorpleExecuteJavaScriptCommand(BuildCommand("clearTimeout(_v_tooltip_timer);_v_tooltip_timer=setTimeout(function() {var $t=$('#lineinput-prefix');$.powerTip.hide();$t.data('powertip','", VorpleEscape(tip), "');$.powerTip.show($t)},", IntToString(delay_milli), ")"));
        }
];

[ VorpleTooltipPrompt tip delay ;
    VorpleTooltipPromptDuration(tip, delay, 7);
];

[ VorpleTooltipPromptIndefinitely tip delay ;
    VorpleTooltipPromptDuration(tip, delay, -1);
];



!=======================================
! Hiding tooltips

[ VorpleTooltipHide delay       delay_milli;  ! TODO: better name ? what does it do? hide last? Hide all?
        if (delay == 0) {
            VorpleExecuteJavaScriptCommand("clearTimeout(_v_tooltip_timer);$.powerTip.hide()");
        } else {
            delay_milli = delay*1000;
            VorpleExecuteJavaScriptCommand(BuildCommand("clearTimeout(_v_tooltip_timer);_v_tooltip_timer=setTimeout(function() {$.powerTip.hide()}, ", IntToString(delay_milli), ")"));
        }
];

#Endif;