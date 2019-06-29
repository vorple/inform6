#Ifndef VORPLE_MULTIMEDIA;

System_file;
Constant VORPLE_MULTIMEDIA;

Include "vorple.h";

! Displaying images and playing sounds and music.


!===================================
! Images

[ VorpleImageInElement file desc classes      id ;
        if (desc == 0) { desc = ""; }
        if (classes == 0) { classes = ""; }
        if (isVorpleSupported()) {
            id = UniqueIdentifier();
            VorplePlaceDivElement(BuildCommand(id, " vorple-image ", classes), "");
            ! two vorple escapes -> can't use buildcommand (TODO: fix this?)
            bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
            print "$('<img>', {src: vorple.options.resource_paths.images+'/";
            PrintStringOrArray(VorpleEscape(file));
            print "', alt: '";
            PrintStringOrArray(VorpleEscape(desc));
            print "'}).appendTo('.";
            PrintStringOrArray(id);
            print "')";
            bp_output_stream(-3);
            VorpleExecuteJavaScriptCommand(hugehugestr);
        } else {
            print (string) desc;
            print "^";
        }
];

Constant IMAGE_CENTERED = 1;
Constant IMAGE_LEFT_ALIGNED = 2;
Constant IMAGE_RIGHT_ALIGNED = 3;
Constant IMAGE_LEFT_FLOATING = 4;
Constant IMAGE_RIGHT_FLOATING = 5;

[ VorpleImage file desc alignment classes ;
        if (desc == 0) { desc = ""; }
        if (classes == 0) { classes = ""; }
        if (classes == "") {
            switch(alignment) {
		IMAGE_CENTERED: VorpleImageInElement(file, desc, "centered");
		IMAGE_LEFT_ALIGNED: VorpleImageInElement(file, desc, "left-aligned");
		IMAGE_RIGHT_ALIGNED: VorpleImageInElement(file, desc, "right-aligned");
		IMAGE_LEFT_FLOATING: VorpleImageInElement(file, desc, "left-floating");
		IMAGE_RIGHT_FLOATING: VorpleImageInElement(file, desc, "right-floating");
		default: VorpleImageInElement(file, desc, classes);
            }
        } else {
            switch(alignment) {
		IMAGE_CENTERED: VorpleImageInElement(file, desc, BuildCommand("centered", " ", classes));
		IMAGE_LEFT_ALIGNED: VorpleImageInElement(file, desc, BuildCommand("left-aligned", " ", classes));
		IMAGE_RIGHT_ALIGNED: VorpleImageInElement(file, desc, BuildCommand("right-aligned", " ", classes));
		IMAGE_LEFT_FLOATING: VorpleImageInElement(file, desc, BuildCommand("left-floating", " ", classes));
		IMAGE_RIGHT_FLOATING: VorpleImageInElement(file, desc, BuildCommand("right-floating", " ", classes));
		default: VorpleImageInElement(file, classes, desc);
            }
        }
];

[ VorplePreloadImage file ;
	VorpleExecuteJavaScriptCommand(BuildCommand("new Image().src=vorple.options.resource_paths.images+'/", VorpleEscape(file), "';"));
];

[ VorplePreloadImages list     len i ;
	! List must be a table, like page 43 of DM4
	len = list-->0;
	for (i=1: i<=len: i++) { VorplePreloadImage(list-->i); }
];




!===================================
! Audio

Constant SOUND_LOOP = 1;

[ VorplePlaySoundEffect file loop;
    bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
    print "vorple.audio.playSound(vorple.options.resource_paths.audio+'/";
    PrintStringOrArray(VorpleEscape(file));
    print "', {looping: ";
    if (loop == SOUND_LOOP) { print "true"; } else { print "false"; }
    print "})";
    bp_output_stream(-3);
    VorpleExecuteJavaScriptCommand(hugehugestr);
];



Constant MUSIC_LOOP = 1;
Constant MUSIC_FROM_START = 1;

[ VorplePlayMusic file loop from_start;
    bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
    print "vorple.audio.playMusic(vorple.options.resource_paths.audio+'/";
    PrintStringOrArray(VorpleEscape(file));
    print "', {looping: ";
    if (loop == MUSIC_LOOP) { print "true"; } else { print "false"; }
    print ", restart: ";
    if (from_start == MUSIC_FROM_START) { print "true"; } else { print "false"; }
    print "})";
    bp_output_stream(-3);
    VorpleExecuteJavaScriptCommand(hugehugestr);
];

[ VorpleStopMusic ;
    VorpleExecuteJavaScriptCommand("vorple.audio.stopMusic()");
];
[ VorpleStopSoundEffects ;
    VorpleExecuteJavaScriptCommand("$('.vorple-sound').remove()");
];
[ VorpleStopAudio ;
    VorpleExecuteJavaScriptCommand("$('.vorple-sound').remove();vorple.audio.stopMusic()");
];

Constant PLAYLIST_LOOP = 1;
Constant PLAYLIST_SHUFFLE = 1;
Constant PLAYLIST_FROM_START = 1;

[ VorpleStartPlaylist playlist loop from_start shuffle       len i j tmp;
    ! Playlist must be a table, like page 43 of DM4
    
    ! We write the full command in hugehugestr (cant just write the playlist in it because BuildCommand writes in it)
    bp_output_stream(3, hugehugestr, LEN_HUGEHUGESTR);
    print "var pl=";
    
    ! the array
    print "[";
    len = playlist-->0;
    for (i=0: i<len: i++) {
        print "'"; print (string) VorpleEscape(playlist-->i); print "',";
    }
    print "'']";
    
    ! the rest
    print ";pl.pop();vorple.audio.setPlaylist(pl, {looping: ";
    if (loop == PLAYLIST_LOOP) { print "true"; } else { print "false"; }
    print ", restart:";
    if (from_start == PLAYLIST_FROM_START) { print "true"; } else { print "false"; }
    print ", shuffled:";
    if (shuffle == PLAYLIST_SHUFFLE) { print "true"; } else { print "false"; }
    print "})";
    
    bp_output_stream(-3);
    VorpleExecuteJavaScriptCommand(hugehugestr);
];

[ VorpleClearPlaylist ;
    VorpleExecuteJavaScriptCommand("return vorple.audio.clearPlaylist()");
];

[ VorpleIsMusicPlaying ;
    VorpleExecuteJavaScriptCommand("return vorple.audio.isMusicPlaying()");
    return VorpleWhatBooleanWasReturned();
];

[ VorpleIsASoundEffectPlaying ;
    VorpleExecuteJavaScriptCommand("return vorple.audio.isEffectPlaying()");
    return VorpleWhatBooleanWasReturned();
];

[ VorpleIsAudioPlaying ;
    VorpleExecuteJavaScriptCommand("return vorple.audio.isAudioPlaying()");
    return VorpleWhatBooleanWasReturned();
];

[ VorpleIsAudioFilePlaying file ;
    VorpleExecuteJavaScriptCommand(BuildCommand("return vorple.audio.isElementPlaying('.vorple-audio[src=@@92''+vorple.options.resource_paths.audio+'/", VorpleEscape(file), "@@92']')"));
    return VorpleWhatBooleanWasReturned();
];

[ VorpleGetMusicFilePlaying ;
    VorpleExecuteJavaScriptCommand("return vorple.audio.currentMusicPlaying()||''");
    return VorpleWhatTextWasReturned();
];


#Endif;