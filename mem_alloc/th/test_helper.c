#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

FILE *results = NULL;

int th_check(int check, const char *message, ...) {
  int sz = 1024, szr;
  char buffer[sz];
  va_list argp;

  va_start(argp, message);
  szr = vsnprintf(buffer, sz, message, argp);
  va_end(argp);

  if(szr >= sz) {
	fprintf(stderr, "ERROR: Ran out of buffer space in th_check (needed: %d)\n", szr);
    exit(1);
  }

  fprintf(stderr, "%s: %s\n", check ? "PASS" : "FAIL", buffer);
  if(results) {
	fprintf(results, "%s: %s\n", check ? "PASS" : "FAIL", buffer);
	fflush(results);
  }
  return check;
}

__attribute__((constructor)) void write_results_to_file() {
  char *results_file;

  results_file = getenv("TH_RESULTS_FILE");
  if(results_file) {
	results = fopen(results_file, "w");
  }
}
