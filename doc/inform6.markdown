# Inform 6 Interactive Fiction with Vorple

{% include doc/intro.markdown %}

## Installation and usage

### Getting started

First of all, you need both the [Inform 6 compiler](https://github.com/DavidKinder/Inform6/)
and the [Inform 6 libraries](https://github.com/DavidGriffith/inform6lib). We're
assuming you know the basics, but if not, you can look at the
[Inform Beginner's Guide](http://inform-fiction.org/manual/download_ibg.html) to get you started.

Note that Vorple is also available for [Inform 7](http://inform7.com).

Vorple is structured around a core extension, contained in the "vorple.h" file.
The other extensions provide the following capabilities:

* Vorple Command Prompt Control: Manipulation of the command line and its history.
* Vorple Hyperlinks: Clickable web links and commands.
* Vorple Modal Windows: Pop-up windows.
* Vorple Multimedia: Images, sounds and videos.
* Vorple Notifications: Notifications as text banners.
* Vorple Screen Effects: Text and font effects.
* Vorple Tooltips: Pop-up tooltips, timed or on mouse hover.

### Using Vorple

After downloading the extensions, you can start using them! The rule is simple:
you need to include Vorple twice: once before including the parser, and once
after including the verblib. (Those are usually defined together anyway.) Don't
worry, Vorple will handle everything correctly; it's just that some things need
to be defined before including the parser and others after including it. So your
code would look like:

    Include "vorple.h";
    Include "parser.h";
    Include "verblib.h";
    Include "vorple.h";
    ! and then write your game

If you are using another extension relying on Vorple (say, vorple-notifications.h),
you need to include the extension after including the parser, but don't forget
to include Vorple! So your code will be:

    Include "vorple.h";
    Include "parser.h";
    Include "verblib.h";
    Include "vorple-notifications.h";

In either case, the other thing you need to do is initialise Vorple, which you do
in your Initialise routine:

    [ Initialise ;
        location = room;
        VorpleInitialise();	! this initialises Vorple
    ];


Time to do a quick example: type the following source code:

    Constant Story "My story";
    Include "vorple.h";
    Include "parser.h";
    Include "verblib.h";
    Include "vorple-notifications.h";
    Include "grammar.h";
    
    [ Initialise ;
        location = room;
        VorpleInitialise();
        VorpleNotification("Welcome to Vorple!");
    ];
    
    Object room "The Room"
        with description "You're in a nondescript room.",
    has light;

After compiling it, you should have a .ulx file -- the latest version of Vorple
is not yet compatible with the Z-Machine. If you run it in a standard
interpreter, it likely won't support Vorple -- but it won't crash, and your
game will still be playable. However, we have a web interpreter
that will run the Vorple effects correctly!

In the [download package](/download/#inform6) there is a file called "play.html".
If you edit the file you should see a line that looks like like
 
    parchment.options.default_story = [ "test.ulx" ];

Just replace "test.ulx" by the name of your story file. You then need to
[install a local server]({{site.url}}/doc/localhost) on your computer to be able
to test your game. After this is done, open a web browser and
type `http://localhost:8000/play.html` to the address bar.


### Technical note about I6 stubs

Vorple makes use of the LookRoutine entry point, which is defined as a stub
in the I6 librairies, and is called after each >look command including the
initial one. If you had un-stubbed this routine, the compiler will return
an error. However, Vorple also creates a MyLookRoutine stub, called in the
new LookRoutine; if you define this function before including Vorple,
your code will be executed as if you had defined LookRoutine.

Additionally, Vorple defines the stub "VorpleStartup", that allows you to
do things while Vorple is initialising (i.e. during VorpleInitialise). This
is useful mostly for one thing, which is queueing commands so that they are
parsed and executed before the game even starts. Note that you can also
use the VorpleInterfaceSetup rulebook to add code to be executed at startup.


## Compatibility with non-Vorple interpreters

The Glulx story files that use Vorple are compatible with offline interpreters
and non-Vorple web interpreters. In most cases the Vorple-specific features
just do nothing.

The story file can test whether it's being run on the Vorple interpreter or not
using a call to the Vorple function `isVorpleSupported()`. Like this:

	if (isVorpleSupported() == 0)
		print "Welcome to a boring old interpreter!"
	else
		VorpleNotification("Welcome to Vorple!");

It's a good idea to make sure that a text-only version of the story works
as intended, for accessibility and archiving reasons.


