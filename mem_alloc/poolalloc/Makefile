TH=../th
TH_CFILE=$(TH)/test_helper.c
DBLL=../dbll
DBLL_FILE=$(DBLL)/dbll.c
POOLALLOC_FILE=poolalloc.c

all: pa_test

pa_test: pa_test.c $(POOLALLOC_FILE) $(DBLL_FILE) $(TH_CFILE)
	$(CC) -std=c99 -Wall -g -I $(DBLL) -I . -I $(TH) -O $^ -o $@

pa_test_malloc: pa_test_malloc.c $(POOLALLOC_FILE) $(DBLL_FILE) $(TH_CFILE)
	$(CC) -std=c99 -Wall -g -I $(DBLL) -I . -I $(TH) -O $^ -o $@
