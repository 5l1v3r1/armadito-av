#include <libumwsu/scan.h>

#include <stdio.h>
#include <stdlib.h>

void report_print_callback(struct umwsu_report *report, void *callback_data)
{
  FILE *out = (FILE *)callback_data;

  umwsu_report_print(report, out);
}

static void usage(int argc, char **argv)
{
  fprintf(stderr, "usage: %s [-r] FILE|DIR ...\n", argv[0]);
  exit(1);
}

int main(int argc, char **argv)
{
  struct umwsu *u;
  struct umwsu_scan *scan;
  int argp = 1;
  enum umwsu_scan_flags flags = 0;
  
  if (argc < 2)
    usage(argc, argv);

  if (!strcmp(argv[argp], "-r")) {
    argp++;
    flags |= UMWSU_SCAN_RECURSE;
  }

  if (!strcmp(argv[argp], "-t")) {
    argp++;
    flags |= UMWSU_SCAN_THREADED;
  }

  if (argp >= argc)
    usage(argc, argv);

  u = umwsu_open();

  umwsu_set_verbose(u, 1);

  umwsu_print(u);

  scan = umwsu_scan_new(u, argv[argp], flags);

  umwsu_scan_add_callback(scan, report_print_callback, stdout);

  umwsu_scan_start(scan);

  umwsu_scan_finish(scan);

  umwsu_scan_free(scan);

  umwsu_close(u);

  return 0;
}
