# Inform 6 Interactive Fiction with Vorple


## Introduction to Vorple

Vorple is a custom web interpreter and a set of extensions for
[Inform 6](http://inform-fiction.org) that lets you do things that so
far haven't been possible such as displaying images, playing sounds,
adding tooltips and notifications and much more.
Take a look at the [demo page](/demo/#inform6) for some examples.

For an explanation on how the system works, see chapter
[Vorple in depth](#vorple-in-depth) below.


### Known limitations

Vorple is experimental software and in early stages of development.
There are a number of known issues that will be fixed in later releases:

* Command history (browsing old commands with the up arrow key) isn't available.
* Undoing is not reliable if the browser communicates with the story file using
  hidden commands.
* Saving and restoring stories is likely to work erratically.
* The function KeyCharPrimitive() (to wait for a key) is not supported.

At the moment only Z-machine is supported. Glulx support is on the works.


## Installation and usage

### Getting started

First of all, you need both the [Inform 6 compiler](https://github.com/DavidKinder/Inform6/)
and the [Inform 6 librairies](https://github.com/DavidGriffith/inform6lib). We're
assuming you know the basics, but if not, you can look at the
[Inform Beginner's Guide](http://inform-fiction.org/manual/download_ibg.html) to get you started.

Note that Vorple is also available for [Inform 7](http://inform7.com), as well as [Undum](http://undum.com).

Vorple is structured around a core extension, contained in the "vorple.h" file.
The other extensions provide the following capabilities:

* Vorple Hyperlinks: Clickable web links and commands
* Vorple Multimedia: Images, sounds and videos
* Vorple Notifications: Notifications as text banners
* Vorple Screen Effects: Text and font effects. Roughly the equivalent of
  the built-in Basic Screen Effects extension.
* Vorple Tooltips: Pop-up tooltips, timed or on mouse hover

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

After compiling it, you should have a .z5 (or .z8) file -- remember that
Vorple doesn't work with Glulx for now. If you run it in a standard
interpreter, it likely won't support Vorple -- but it won't crash, and your
game will still be playable. However, we have a web interpreter
that will run the Vorple effects correctly!

In the [download package](/download/#inform6) there is a file called "play.html".
If you edit the file you should see a line that looks like like
 
    parchment.options.default_story = [ "test.z5" ];

Just replace "test.z5" by the name of your story file, and you're good to go!
To publish your work, upload the "play.html" file and the "interpreter"
directory along with its contents to a web server.


### Technical note about I6 stubs

Vorple makes use of a few I6 entry points that are defined as stubs in the
I6 librairies (usually at the end of your language grammar definition file).
Those stubs are ParserError (called every time the parser didn't understand
the sentence) and LookRoutine (called after each >look command including the
initial one).

You might have defined (un-stubbed) those routines in your code, before
including "grammar"; if that is the case, the I6 compiler will return an
error. However Vorple creates 2 stubs with the names "MyParserError" and
"MyLookRoutine", that are called in those routines. Hence, you simply need
to rename your "ParserError" routine in "MyParserError" (and same for
LookRoutine), and move the definition of those routines to *before including
a vorple file for the second time*. And everything should be fine! (See the
"How to II" example for sample code.)

Additionally, Vorple defines the stub "VorpleStartup", that allows you to
do things while Vorple is initialising (i.e. during VorpleInitialise). This
is useful mostly for one thing, which is queueing commands so that they are
parsed and executed before the game even starts.


### A note about Internet Explorer

Vorple is compatible with Internet Explorer, but __only if played online__.
If you try to open the interpreter directly from the hard drive (as you do
when you open the online interpreter using the "browse" link as discussed above)
the page will crash in the loading screen.

The simple solution to this problem is to use some other browser, like Chrome
or Firefox. If the "browse" link opens the interpreter in Internet Explorer,
copy the address from the address bar, open the other browser, and paste it in
the other browser's address bar instead.


## Compatibility with non-Vorple interpreters

The Z-code story files that use Vorple are compatible with offline interpreters
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


## Vorple in depth

Back in the days when home computers first entered consumer markets there
were many competing companies selling computers that weren't compatible with
each other. To maximize the amount of potential customers
[Infocom](http://en.wikipedia.org/wiki/Infocom) had to solve the problem of
programming and distributing their games to a wide array of computers with
varying specifications and limitations.

The solution was to create a virtual machine that would harmonize the
differences between computer systems. The game file would always be the same
for each system and there would be a system-specific interpreter program that
would translate the game file into instructions the computer understood.
This way the game could be programmed and compiled only once and it would work
on any system that had an interpreter written for it.

![Virtual machine inside operating system]({{site.url}}/media/image/doc/virtual-machine.png)


The virtual machine was called _Z-machine_ after Zork, the first game
that used it. Decades later Inform 6 still compiles to Z-machine (and Glulx,
the contemporary virtual machine that works basically the same way).

To a modern consumer of interactive fiction the virtual machine model has
other benefits in addition to being able to play the stories in a wide
selection of devices. The virtual machine is effectively a _sandbox_
that limits what story files are allowed to do. They can't, for example,
delete files from the computer, install malicious software or access your
webcam. When you download an interactive fiction story file you can be certain
that it isn't a virus and it can't do anything harmful.

To an author of interactive fiction the sandbox can sometimes feel rather
limiting. We've come a long way since the early days of Infocom and the
things we now casually do with computers is far more than anyone could have
dreamed of 30 years ago. Yet interactive fiction is still confined to
streaming text, displaying pictures, playing sounds and performing some
limited file operations.

Cue the Internet age. Parchment is an online Z-machine/Glulx interpreter
that lets you play interactive fiction on any computer with Internet access
and a web browser. A modern web browser is also a sandbox, but with quite
a lot of more capabilities (but still with restrictions in place so that in
theory you can visit any web page and be sure that you can't catch anything
malicious).

![Virtual machine inside web browser inside operating system]({{site.url}}/media/image/doc/parchment.png)

Parchment was a small revolution in itself and turned the community focus
from downloadable story files to Internet play, but Parchment is still
"only" an implementation of existing virtual machines. It restricts the
story files to the same sandbox as offline interpreters do.

This is where Vorple comes in. It makes a small addition to the interpreter
so that the story files can break free of the Z-machine sandbox and
communicate with the browser running the interpreter.

![Vorple creating a bridge between the virtual machine and the browser]({{site.url}}/media/image/doc/vorple-bridge.png)

With this bridge in place the story file can do pretty much whatever it
wants with the user interface and the story text&mdash;even story text that
has already been printed.

Additionally Vorple provides a JavaScript library that makes it easier to
add features specifically to interactive fiction stories (as opposed to
usual web pages). The Inform 6 extensions are mainly interfaces to the
underlying JavaScript library.

To recap, Vorple for Inform consists of three main features:

* JavaScript library for building and handling user interface elements
* Inform 6 extensions that can use the library
* Customized interpreter that lets the extensions access the library

The project's headquarters are at [vorple-if.com](http://vorple-if.com) where
you'll find all the official materials and downloads. JavaScript developers who
want to dig into the innards of the system can find the source code at
[GitHub](http://github.com/juhana/vorple).

The library and all official materials are open source and free for anyone
to use for any purpose. (Mandatory footnote: Do still read all the licenses
if you want to use it commercially or as a part of a closed system.
Some third party themes, for example, are subject to slightly more
restrictive licenses.)


## Support

The best place to look for assistance is the community forum at
[intfiction.org](http://intfiction.org/forum). Vorple's author
frequents the forum, as do other experienced authors who are happy to
answer civilized help requests about authoring with Inform, Vorple or other
systems.

Bugs can be reported at the project's
[GitHub pages](https://github.com/juhana/vorple/issues) or by
sending mail to [bugs@vorple-if.com](mailto:bugs@vorple-if.com).
