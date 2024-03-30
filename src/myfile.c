#include"argp.h"
#include <stdio.h>
#include "operation.h"

const char *argp_program_version = "Test";

/* Program documentation. */
static char doc[] = "Programme de test.";

/* A description of the arguments we accept. */
static char args_doc[] = "COMMAND [OPTIONS...]";

/* The options we understand. */
static struct argp_option options[] = {
{"format", 'f', "PARTITION", 0, "Formater la partition spécifiée"},
{"open", 'o', "FICHIER", 0, "Ouvrir le fichier spécifié"},
{"write", 'w', "FICHIER BUFFER TAILLE", 0, "Écrire le tampon spécifié dans le fichier spécifié"},
{"read", 'r', "FICHIER COMPTEUR_DE_BYTES", 0, "Lire COMPTEUR_DE_BYTES octets à partir du fichier spécifié"},
{"seek", 's', "FICHIER DÉCALAGE BASE", 0, "Déplacer le pointeur de fichier vers le décalage spécifié par rapport à la base"},
{"close", 'c', "FICHIER", 0, "Fermer le fichier spécifié"},
{ 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *partition;
  char *file;
  char *buffer;
  int bytes;
  int offset;
  int base;
  int command;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'f':
      arguments->command = 1;
      arguments->partition = arg;
      break;

    case 'o':
      arguments->command = 2;
      arguments->file = arg;
      break;

    case 'w':
      arguments->command = 3;
      arguments->file = strtok (arg, " ");
      arguments->buffer = strtok (NULL, " ");
      arguments->bytes = atoi (strtok (NULL, " "));
      break;

    case 'r':
      arguments->command = 4;
      arguments->file = arg;
      arguments->bytes = atoi (strtok (NULL, " "));
      break;

    case 's':
      arguments->command = 5;
      arguments->file = arg;
      arguments->offset = atoi (strtok (NULL, " "));
      arguments->base = atoi (strtok (NULL, " "));
      break;

    case 'c':
      arguments->command = 6;
      arguments->file = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1)
        argp_usage (state);
      arguments->file = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv)
{
  struct arguments arguments;

  /* Set the default values. */
  arguments.command = 0;
  arguments.partition = NULL;
  arguments.file = NULL;
  arguments.buffer = NULL;
  arguments.bytes = 0;
  arguments.offset = 0;
  arguments.base = 0;

  /* Parse the command-line arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  /* Call the appropriate library function based on the user's
     command. */
  switch (arguments.command)
    
    {
    case 1:
    if (myFormat(arguments.partition) != 0)
    {
        fprintf(stderr, "Error: partition format failed\n");
        return 1;
    }
    break;

    case 2:
    if (myOpen(arguments.file) == NULL)
    {
        fprintf(stderr, "Error: failed to open file %s\n", arguments.file);
        return 1;
    }
    break;

    case 3:
    if (myWrite(myOpen(arguments.file), arguments.buffer, arguments.bytes) != arguments.bytes)
    {
        fprintf(stderr, "Error: failed to write to file %s\n", arguments.file);
        return 1;
    }
    break;

    case 4:
    if (myRead(myOpen(arguments.file), arguments.buffer, arguments.bytes) != arguments.bytes)
    {
        fprintf(stderr, "Error: failed to read from file %s\n", arguments.file);
        return 1;
    }
    break;

    case 5:
    mySeek(myOpen(arguments.file), arguments.offset, arguments.base);
    break;

    case 6:
    if (myClose(myOpen(arguments.file)) != 0)
    {
        fprintf(stderr, "Error: failed to close file %s\n", arguments.file);
        return 1;
    }
    break;

    default:
    fprintf(stderr, "Error: invalid command %d\n", arguments.command);
    return 1;
    }

    return 0;
}