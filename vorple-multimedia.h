Include "vorple.h";
System_file;

! Displaying images, playing sounds and music, and showing videos from YouTube.


!===================================
! Images

[ VorpleImageInElement file classes       id ;
	id = UniqueIdentifier();
	VorplePlaceDivElement(BuildCommand(id, " ", classes, " vorple-image"));
	VorpleExecuteJavaScriptCommand(BuildCommand("$('.", id, "').html(vorple.media.image('", VorpleEscape(file), "'))"));
];

Constant IMAGE_CENTERED = 1;
Constant IMAGE_LEFT_ALIGNED = 2;
Constant IMAGE_RIGHT_ALIGNED = 3;
Constant IMAGE_LEFT_FLOATING = 4;
Constant IMAGE_RIGHT_FLOATING = 5;

[ VorpleImage file alignment ;
	switch(alignment) {
			IMAGE_CENTERED: VorpleImageInElement(file, "centered");
			IMAGE_LEFT_ALIGNED: VorpleImageInElement(file, "left-aligned");
			IMAGE_RIGHT_ALIGNED: VorpleImageInElement(file, "right-aligned");
			IMAGE_LEFT_FLOATING: VorpleImageInElement(file, "left-floating");
			IMAGE_RIGHT_FLOATING: VorpleImageInElement(file, "right-floating");
			default: VorpleImageInElement(file, "");
	}
];

[ VorplePreloadImage file ;
	VorpleExecuteJavaScriptCommand(BuildCommand("vorple.media.preloadImage('", VorpleEscape(file), "');"));
];

[ VorplePreloadImages list     len i ;
	! List must be a table, like page 43 of DM4
	len = list-->0;
	for (i=0: i<len: i++) { VorplePreloadImage(list-->i); }
];



!===================================
! Audio

Constant LOOP_SOUND = 1;
Constant LOOP_MUSIC = 1;

[ VorplePlaySound file loop;
    if (loop == LOOP_SOUND) {VorpleExecuteJavaScriptCommand(BuildCommand("vorple.media.playSound('", VorpleEscape(file), "',{loop:true})")); }
    else {VorpleExecuteJavaScriptCommand(BuildCommand("vorple.media.playSound('", VorpleEscape(file), "',{loop:false})")); }
];

[ VorplePlayMusic file loop;
    if (loop == LOOP_MUSIC) {VorpleExecuteJavaScriptCommand(BuildCommand("vorple.media.playMusic('", VorpleEscape(file), "',{loop:true})")); }
    else {VorpleExecuteJavaScriptCommand(BuildCommand("vorple.media.playMusic('", VorpleEscape(file), "',{loop:false})")); }
];

[ VorpleStopMusic ;
	VorpleExecuteJavaScriptCommand("vorple.media.stopMusic()");
];
[ VorpleStopSounds ;
	VorpleExecuteJavaScriptCommand("vorple.media.stopSounds()");
];
[ VorpleStopAudio ;
	VorpleExecuteJavaScriptCommand("vorple.media.stopAll()");
];


!===================================
! Video (Youtube)


[ VorplePlayYoutubeVideo youtubeid      containerid;
  containerid = UniqueIdentifier();
  VorplePlaceDivElement(BuildCommand(containerid," youtube"),"");
  VorpleExecuteJavaScriptCommand(BuildCommand("$('.", containerid, "').html(vorple.media.youtube('", youtubeid, "',{width:600}))"));
];
