all: fichierulx 

fichierulx:
	@inform-633/src/inform '$$MAX_STATIC_DATA=30000' '$$MAX_LABELS=12000' -s~S -e -f -G -Cu test.inf
