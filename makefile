yourcompilerpath=@inform-633/src/inform
yourlibfolders=contributions/i6-extensions

all: ulxfile

ulxfile:
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) test.inf

buildexamples:
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Clarification\ Helper.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Petting\ Zoo.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Click\ to\ Learn\ More.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Port\ Royal\ Reggae.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Click\ to\ Retry.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Score\ Notifications.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Convenience\ Store.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Scrambled\ Eggs.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/How\ To\ II.inf 
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Serinette.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/How\ To\ I.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Sonnet\ Carousel.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Ibid\ \(2\).inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/The\ Grandfather\ Clock.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Let\ Me\ Show\ You.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/The\ Greeter.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Letters\ from\ a\ Madman.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/The\ Manchurian\ Candidate.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Medical\ Dictionary.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/The\ Sum\ of\ Human\ Knowledge.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Monty\ Hall.inf 
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Version\ Popup.inf
	$(yourcompilerpath) '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu +include_path=$(yourlibfolders) examples/Mood\ Swings.inf
	mv -t examples Clarification\ Helper.ulx Petting\ Zoo.ulx Click\ to\ Learn\ More.ulx Port\ Royal\ Reggae.ulx Click\ to\ Retry.ulx Score\ Notifications.ulx Convenience\ Store.ulx Scrambled\ Eggs.ulx How\ To\ II.ulx Serinette.ulx How\ To\ I.ulx Sonnet\ Carousel.ulx Ibid\ \(2\).ulx The\ Grandfather\ Clock.ulx Let\ Me\ Show\ You.ulx The\ Greeter.ulx Letters\ from\ a\ Madman.ulx The\ Manchurian\ Candidate.ulx Medical\ Dictionary.ulx The\ Sum\ of\ Human\ Knowledge.ulx Monty\ Hall.ulx Version\ Popup.ulx Mood\ Swings.ulx