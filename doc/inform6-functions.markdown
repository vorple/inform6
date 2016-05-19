# Inform 6 Interactive Fiction with Vorple


## List of Vorple functions

Here's a handy list of the functionalities that Vorple can provide,
and what the corresponding functions are.

### Vorple.h

This provides the basic functionality of Vorple:

* `VorpleInitialise()`: initialises Vorple ; call once, in your Initialise routine.
* `isVorpleSupported()`: returns 0 if Vorple isn't supported, 1 if it is.
Note: calling the functions from the Vorple extensions will not create any side-effects
if the interpreter doesn't support Vorple; rather, Vorple will fail silently. But this
function could be interesting if you wanted to change what your game says or does depending
on this!

Other functions are available:

* `VorpleExecuteJavaScriptCommand(str)`: the javascript command `str` will be sent to the interpreter,
which will execute it.
* Placing HTML elements:
  * `VorplePlaceElement(elt, name, text)`: create a HTML tag of type `elt` (this could be `<tag>`,
`<div>`, anything!), with the name `name` and containing the text `text`. The shorthands
`VorplePlaceSpanElement(name,txt)` (to place `<span>`) or `VorplePlaceDivElement(name,txt)` (for `<div>`)
are also available.
  * `VorplePutContentInAllElements(name, text)`: put the text `text` in all HTML elements that have
the name `name`. You can put it only in the last (or only) one created with this name using
`VorplePutContentInElement(name,text)`.

* Queuing commands:
  * `VorpleQueueCommand(cmd, flagDontShow)`: make it so that at the next turn the command `cmd`
(of the 'go north' type) will be sent to the parser; call with `flagDontShow=1` if you don't want the
player to see the command that was sent, but if she should see the game's reply.
  * `VorpleQueueSilentCommand(cmd)`: at the next turn, the command `cmd` will be sent to the parser
without the player noticing; that is to say, the command will not be displayed, but neither
will the game's reply.
  * `VorpleQueuePrimaryCommand(cmd, flagDontShow)` and `VorpleQueueSilentCommand(cmd)`: the same, except
primary commands have their own queue, which is dealt with before the standard command queue that
the other commands above use. Basically, if you want to have communication between the interpreter
and the story file between two commands, put the first in the primary command and the other one in
the standard one.

* `VorpleTypeOfTurn(type)`: Vorple distinguishes several types of turn: `"normal"` (by default), `"meta"`
(for actions that shouldn't take a turn), and `"error"` (for parser errors, that will be hidden
from the display). Call this function with one of those arguments to set the current type of turn;
for instance, call `VorpleTypeOfTurn("error")` in a verb definition at the point where your code
can't make sense of the player's command.

As well as utility functions:

* `UniqueIdentifier()`: generates a 12-digit unique identifier; you can use that for the names of your
HTML elements.
* `IntToString(i)`: returns an array representing the integer i.
* `PrintStringOrArray(txt)`: displays the given string or buffer array.
* `BuildCommand(str1, ..., str7)`: usage of this function is discouraged! It is used internally to
concatenate strings, in almost every call to the Vorple interface; if you use it several times in a
row, you overwrite hugehugestr, and you lose information. If you need a function to concatenate things,
declare an array that's not hugehugestr and adapt the code of this function.

Finally, write your own `VorpleStartup()` routine to do things while Vorple is starting up; for instance
you can queue silent commands, that will be executed by Vorple before the Initialise routine is done.

Relevant examples: Convenience Store, Spy Games, Scrambled Eggs, The Sum of Human Knowledge (note that
this one uses its own CSS sheet), and all the other ones.

### Vorple-hyperlinks.h

* `VorpleLinkWebsiteText(url, text, flag)`: this allows you to create a hypertext link on the
text pointing to the specified url; the optional flag can be set to `SAME_WINDOW` if you want the page
to open in the same window (for instance, if your game is over).
* `VorpleLinkWebsite(url, sameWindow)`: similar, but the link will read the explicit url.

* `VorpleLinkCommandText(cmd, text, dontShow)`: this creates a hyperlink showing the text such that
when the player clicks on it, the command cmd is sent to the parser; use this for example to have
clickable directions, by putting a link to "go north" on the word "north". The optional argument
dontShow can take the following values: `0` by default (show the command and the response), or any
of `DONT_SHOW_COMMAND`, `DONT_SHOW_RESPONSE` or `DONT_SHOW_COMMAND_OR_RESPONSE`.
* `VorpleLinkCommand(cmd, dontShow)`: similar, but the link will read the explicit command.

Relevant examples: Click to Learn More, Click to Retry.

### Vorple-multimedia.h

In order to use this extension, you have to point the interpreter to the folder where your multimedia
files are. This is done by modifying the play.html file. For example, change

    vorple.media.defaults.audioPath = 'examples/resources/';

to set the folder in which Vorple will look for your audio files.

* `VorpleImage(file, alignment)`: display the image in the file, with an optional argument for the
alignmnent; the values you can specify are `IMAGE_CENTERED`, `IMAGE_LEFT_ALIGNED`, `IMAGE_RIGHT_ALIGNED`,
`IMAGE_LEFT_FLOATING`, and `IMAGE_RIGHT_FLOATING`.
* `VorplePreloadImage(file)`: preload the image, i.e. have the interpreter load the image before it
is displayed (for instance, to avoid loading times).
* `VorplePreloadImages(list)`: preload all images in the table.

* `VorplePlaySound(file, loop)`: play the specified sound; put the second argument to `LOOP_SOUND` (or `1`)
to loop it.
* `VorplePlayMusic(file, loop)`: play the specified music; put the second argument to `LOOP_MUSIC` (or `1`)
to loop it.
* `VorpleStopMusic()`: stop all music.
* `VorpleStopSounds()`: stop all sounds.
* `VorpleStopAudio()`: stop all music and sounds.
* `VorplePlayYoutubeVideo(youtubeID)`: play the YouTube video of your choice by specifying its YouTube ID;
Vorple will play the video at the URL https://www.youtube.com/watch?youtubeID

Relevant examples: Serinette, Port Royal Reggae, The Trampoline.

### Vorple-notifications.h

Note: you can only have 16 notifications at the same time on the screen; should you need more, you need
to modify the `VORPLE_MAX_NUMBER_NOTIFS` variable.

* `VorpleNotification(text, pos)`: show the notification reading the text; the optional argument specifies
where the notification should be displayed. It can take the following values: `VORPLE_POSITION_TOP_BANNER`,
`VORPLE_POSITION_BOTTOM_BANNER`, `VORPLE_POSITION_TOP_LEFT`, `VORPLE_POSITION_TOP_CENTER`,
`VORPLE_POSITION_TOP_RIGHT`, `VORPLE_POSITION_CENTER_LEFT`, `VORPLE_POSITION_SCREEN_CENTER`,
`VORPLE_POSITION_CENTER_RIGHT`, `VORPLE_POSITION_BOTTOM_LEFT`, `VORPLE_POSITION_BOTTOM_CENTER`,
`VORPLE_POSITION_BOTTOM_RIGHT`.
* `VorpleAlert(text)`: display an alert, that is to say a pop-up dialog box in the browser.
* `VorpleHideNotifications()`: hide all notifications.
* `VorpleDefaultNotifPosition(pos)`: all notifications will be displayed at a specific position by
default.

Relevant examples: How to I, Score Notifications.

### Vorple-screen-effects.h

* `VorpleStyle(style)`: from then on, write the letters in a specific style; the values are `STYLE_CURSIVE`,
`STYLE_EMPHASIS`, `STYLE_FANTASY`, `STYLE_MONOSPACE`, `STYLE_NOWRAP`, `STYLE_STRIKETHROUGH`, `STYLE_STRONG`,
`STYLE_TRANSIENT`, `STYLE_UNDERLINE`, `STYLE_XXSMALL`, `STYLE_XSMALL`, `STYLE_SMALL`, `STYLE_LARGE`, `STYLE_XLARGE`,
`STYLE_XXLARGE`.
* `VorpleEndStyle()`: disable the latest style.
* `VorpleEndAllStyles()`: disable all text styles.

* `VorpleColorLetters(color)`: from then on, write the letters in a different color.
* `VorpleColorLettersBackground(color)`: change the color of the space around the letters.
* `VorpleForegroundColor(color)`: change foreground color.
* `VorpleBackgroundColor(color)`: change background color.
* `VorpleDefaultStyle()`: return to default values.
The color values are as follows: `BLACK`, `RED`, `GREEN`, `YELLOW`, `BLUE`, `VIOLET`, `CYAN`, `WHITE`.

* `VorpleCenterText(text)`: print the text with fixed width and centered.
* `VorpleRightAlign(text)`: print the text with right alignment.
* `VorplePlaceBlockquoteElement(name text)`: print the text in a blockquote HTML element with the given
name and showing the text.
* `VorpleHideLastBlockquote()`: hides the last displayed blockquote.
* `VorpleHideAllBlockquotes()`: hides all blockquotes.

* `VorpleClearScreen(window)`: clear all the screen (`0`), the status bar (`1`) or the main window (`2`).
* `VorplePause()`: wait for a key press then clear the screen.

Relevant examples: Letters from a Madman, Monty Hall (note that it uses its own CSS sheet).

### Vorple-tooltips.h

* `VorpleTooltipMouse(text, tip)`: writes the text, and opens a tooltip showing the tip when the player
points their mouse to the text.
* `VorpleTooltipAllElementsMouse(tip, name)`: if the player hovers over any HTML element named `name`
in the page, there will be a tooltip.
* `VorpleTooltipElementMouse(tip, name)`: the same but for the latest of the elements named `'name'`.

* `VorpleTooltipElement(tip, name, n)`: after n tenths of second, a tooltip with the tip will display on
the element named `name`.
* `VorpleTooltipAllElements(tip, name, n)`: the same, but on all elements named `name`.
* `VorpleTooltipPrompt(tip, n)`: after n tenths of second, a tooltip with the tip will display on the
prompt.

Relevant examples: Medical Dictionary, How to II, Ibid (2).