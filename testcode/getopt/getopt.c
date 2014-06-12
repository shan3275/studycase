#include <stdio.h>
#include <getopt.h>

int do_name, do_gf_name;
char *l_opt_arg;

static const char *shortopts = "l:ng";
struct option longopts[] = {
{"name", no_argument, NULL, 'n'},
{"gf_name", no_argument, NULL, 'g'},
{"love", required_argument, NULL, 'l'},
{0, 0, 0, 0},
};

int main (int argc, char *argv[])
{
int c;

while ((c = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
    {
      switch (c)
   {
   case 'n':
      printf ("My name is LYR.\n");
      break;
   case 'g':
      printf ("Her name is BX.\n");
      break;
   case 'l':
      l_opt_arg = optarg;
      printf ("Our love is %s!\n", l_opt_arg);
      break;
   }
    }
return 0;
}
