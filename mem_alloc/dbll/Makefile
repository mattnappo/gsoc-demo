TH=../th
TH_CFILE=$(TH)/test_helper.c
DBLL_FILE=dbll.c

all: dbll_test

dbll_test: dbll_test.c $(DBLL_FILE) $(TH_CFILE)
	$(CC) -std=c99 -Wall -g -I . -I $(TH) -O $^ -o $@

dbll_test_asan: dbll_test_asan.c $(DBLL_FILE) $(TH_CFILE)
	$(CC) -std=c99 -fsanitize=address -O1 -Wall -g -I . -I $(TH) -O $^ -o $@

dbll_test_malloc: dbll_test_malloc.c $(DBLL_FILE) $(TH_CFILE)
	$(CC) -std=c99 -Wall -g -I . -I $(TH) -O $^ -o $@

