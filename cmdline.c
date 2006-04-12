/*
  File autogenerated by gengetopt version 2.13.1
  generated with the following command:
  gengetopt --unamed-opts --no-handle-help --set-version=0.3 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopt.h"

#include "cmdline.h"

static
void clear_given (struct gengetopt_args_info *args_info);
static
void clear_args (struct gengetopt_args_info *args_info);

static int
cmdline_parser_internal (int argc, char * const *argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required, const char *additional_error);


static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct gengetopt_args_info *args_info)
{
  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->output_given = 0 ;
  args_info->page_given = 0 ;
  args_info->stdout_given = 0 ;
  args_info->zip_given = 0 ;
}

static
void clear_args (struct gengetopt_args_info *args_info)
{
  args_info->output_arg = NULL;
  args_info->page_arg = 1 ;
}

void
cmdline_parser_print_version (void)
{
  printf ("%s %s\n", CMDLINE_PARSER_PACKAGE, CMDLINE_PARSER_VERSION);
}

void
cmdline_parser_print_help (void)
{
  cmdline_parser_print_version ();
  printf("\n%s\n", "This program converts DVI files to Scalable Vector Graphics (SVG)");
  printf("\nUsage: dvisvgm [OPTIONS]... [FILES]...\n\n");
  printf("%s\n","  -h, --help             Print help and exit");
  printf("%s\n","  -V, --version          Print version and exit");
  printf("%s\n","  -o, --output=filename  Set name of output file");
  printf("%s\n","  -p, --page=number      Choose page to convert  (default=`1')");
  printf("%s\n","  -s, --stdout           Write SVG output to stdout");
  printf("%s\n","  -z, --zip[=level]      Create compressed SVG files (.svgz)");
  
}

void
cmdline_parser_init (struct gengetopt_args_info *args_info)
{
  clear_given (args_info);
  clear_args (args_info);

  args_info->inputs = NULL;
  args_info->inputs_num = 0;
}

void
cmdline_parser_free (struct gengetopt_args_info *args_info)
{
  
  unsigned int i;
  if (args_info->output_arg)
    {
      free (args_info->output_arg); /* free previous argument */
      args_info->output_arg = 0;
    }
  
  for (i = 0; i < args_info->inputs_num; ++i)
    free (args_info->inputs [i]);
  
  if (args_info->inputs_num)
    free (args_info->inputs);
  
  clear_given (args_info);
}

/* gengetopt_strdup() */
/* strdup.c replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = NULL;
  if (!s)
    return result;

  result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

int
cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info)
{
  return cmdline_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
cmdline_parser2 (int argc, char * const *argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required)
{
  int result;

  result = cmdline_parser_internal (argc, argv, args_info, override, initialize, check_required, NULL);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name)
{
  return EXIT_SUCCESS;
}

int
cmdline_parser_internal (int argc, char * const *argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  int error = 0;
  struct gengetopt_args_info local_args_info;

  if (initialize)
    cmdline_parser_init (args_info);

  cmdline_parser_init (&local_args_info);

  optarg = 0;
  optind = 1;
  opterr = 1;
  optopt = '?';

  while (1)
    {
      int option_index = 0;
      char *stop_char;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "output",	1, NULL, 'o' },
        { "page",	1, NULL, 'p' },
        { "stdout",	0, NULL, 's' },
        { "zip",	2, NULL, 'z' },
        { NULL,	0, NULL, 0 }
      };

      stop_char = 0;
      c = getopt_long (argc, argv, "hVo:p:sz::", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          if (local_args_info.help_given)
            {
              fprintf (stderr, "%s: `--help' (`-h') option given more than once%s\n", argv[0], (additional_error ? additional_error : ""));
              goto failure;
            }
          if (args_info->help_given && ! override)
            continue;
          local_args_info.help_given = 1;
          args_info->help_given = 1;
          cmdline_parser_free (&local_args_info);
          return 0;

        case 'V':	/* Print version and exit.  */
          cmdline_parser_print_version ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'o':	/* Set name of output file.  */
          if (local_args_info.output_given)
            {
              fprintf (stderr, "%s: `--output' (`-o') option given more than once%s\n", argv[0], (additional_error ? additional_error : ""));
              goto failure;
            }
          if (args_info->output_given && ! override)
            continue;
          local_args_info.output_given = 1;
          args_info->output_given = 1;
          if (args_info->output_arg)
            free (args_info->output_arg); /* free previous string */
          args_info->output_arg = gengetopt_strdup (optarg);
          break;

        case 'p':	/* Choose page to convert.  */
          if (local_args_info.page_given)
            {
              fprintf (stderr, "%s: `--page' (`-p') option given more than once%s\n", argv[0], (additional_error ? additional_error : ""));
              goto failure;
            }
          if (args_info->page_given && ! override)
            continue;
          local_args_info.page_given = 1;
          args_info->page_given = 1;
          args_info->page_arg = strtol (optarg,&stop_char,0);
          break;

        case 's':	/* Write SVG output to stdout.  */
          if (local_args_info.stdout_given)
            {
              fprintf (stderr, "%s: `--stdout' (`-s') option given more than once%s\n", argv[0], (additional_error ? additional_error : ""));
              goto failure;
            }
          if (args_info->stdout_given && ! override)
            continue;
          local_args_info.stdout_given = 1;
          args_info->stdout_given = 1;
          break;

        case 'z':	/* Create compressed SVG files (.svgz).  */
          if (local_args_info.zip_given)
            {
              fprintf (stderr, "%s: `--zip' (`-z') option given more than once%s\n", argv[0], (additional_error ? additional_error : ""));
              goto failure;
            }
          if (args_info->zip_given && ! override)
            continue;
          local_args_info.zip_given = 1;
          args_info->zip_given = 1;
          if (optarg)
            {
              args_info->zip_arg = strtol (optarg,&stop_char,0);
            }
          break;


        case 0:	/* Long option with no short option */
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */




  cmdline_parser_free (&local_args_info);

  if ( error )
    return (EXIT_FAILURE);

  if (optind < argc)
    {
      int i = 0 ;

      args_info->inputs_num = argc - optind ;
      args_info->inputs =
        (char **)(malloc ((args_info->inputs_num)*sizeof(char *))) ;
      while (optind < argc)
        args_info->inputs[ i++ ] = gengetopt_strdup (argv[optind++]) ;
    }

  return 0;

failure:
  
  cmdline_parser_free (&local_args_info);
  return (EXIT_FAILURE);
}
