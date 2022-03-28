/*********************/
/* par.c             */
/* for Par 3.20      */
/* Copyright 1993 by */
/* Adam M. Costello  */
/*********************/

/* This is ANSI C code. */


#include "errmsg.h"
#include "buffer.h"    /* Also includes <stddef.h>. */
#include "reformat.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h> //gets getopt
#include <ctype.h>

#undef NULL
#define NULL ((void *) 0)


const char * const progname = "par";
const char * const version = "3.20";


static int digtoint(char c)

/* Returns the value represented by the digit c,   */
/* or -1 if c is not a digit. Does not use errmsg. */
{
  return c == '0' ? 0 :
         c == '1' ? 1 :
         c == '2' ? 2 :
         c == '3' ? 3 :
         c == '4' ? 4 :
         c == '5' ? 5 :
         c == '6' ? 6 :
         c == '7' ? 7 :
         c == '8' ? 8 :
         c == '9' ? 9 :
         -1;

  /* We can't simply return c - '0' because this is ANSI  */
  /* C code, so it has to work for any character set, not */
  /* just ones which put the digits together in order.    */
}


static int strtoudec(const char *s, int *pn)

/* Puts the decimal value of the string s into *pn, returning */
/* 1 on success. If s is empty, or contains non-digits,       */
/* or represents an integer greater than 9999, then *pn       */
/* is not changed and 0 is returned. Does not use errmsg.     */
{
  int n = 0;

  if (!*s) return 0;

  do {
    if (n >= 1000 || !isdigit(*s)) return 0;
    n = 10 * n + digtoint(*s);
  } while (*++s);

  *pn = n;

  return 1;
}


static void parseopt(int argc, char **argv,
  const char *opt, int *pwidth, int *pprefix,
  int *psuffix, int *phang, int *plast, int *pmin
)
/* Parses the single option in opt, setting *pwidth, *pprefix,     */
/* *psuffix, *phang, *plast, or *pmin as appropriate. Uses errmsg. */
{
  static int versionFlag;
  const char *saveopt = opt; //for error messages
  char oc;//the actual option
  int n, r; //n stores the option decimal
  //r will be storing the value in the arg
  static struct option longOptions[] =
  {
    //flags
    {"version", no_argument,       &versionFlag, 1},
    //the actual options
    {"width",   required_argument, 0, 'w'},
    {"prefix",  required_argument, 0, 'p'},
    {"suffix",  required_argument, 0, 's'},
    {"hang",  no_argument, 0, 'h'},
    {"last",    no_argument, 0, 'l'},
    {"min",    no_argument, 0, 'm'},
    {0, 0, 0, 0}
  };
  int optionIndex =0;
  int option;
  int i = 0;
  while((option = getopt_long(argc, argv, "w:p:s:h::l::m::", longOptions, &optionIndex)) != -1){

      int dummy = 0;
      saveopt = argv[i];

      switch (option){
      case 0:
        fprintf(stderr, "%s %s\n", progname, version);
        exit(EXIT_SUCCESS);
        break;
      case 'w':
        if(optarg != NULL){ 
          if(strtoudec(optarg,&dummy) == 0){  //if the copy is bad
            goto badopt;
          }
          *pwidth = dummy;
          continue;
        }else{
            goto badopt;
        }
            

      case 'p':

         if(optarg != NULL){ 
           
          if(strtoudec(optarg,&dummy) == 0){  //if the copy is bad
          
            goto badopt;
          }
          *pprefix = dummy;
          continue;
        }else{
          
            goto badopt;
        }
      case 's':
        if(optarg != NULL){ 
           
          if(strtoudec(optarg,&dummy) == 0){  //if the copy is bad
             
            goto badopt;
          }
          *psuffix = dummy;
          continue;
        }else{
            goto badopt;
        }
      case 'h':
        if(optarg != NULL){ 
           
          if(strtoudec(optarg,&dummy) == 0){  //if the copy is bad
            goto badopt;
          }

          *phang = dummy;
          continue;
        }else{
            goto badopt;
        }
      case 'l':

        if(optarg != NULL){ 
           
          if(strtoudec(optarg,&dummy) == 0){  //if the copy is bad
            goto badopt;
          }
          if(dummy != 0 || dummy != 1){  //copy is good, is this good arguemnt?
            goto badopt;
          }
          *plast = dummy;
          continue;
        }else{
            goto badopt;
        }
      case 'm':
        if(optarg != NULL){ 
           
          if(strtoudec(optarg,&dummy) == 0){  //if the copy is bad

            goto badopt;
          }
          if(dummy != 0 || dummy != 1){  //copy is good, is this good arguemnt?
            
            goto badopt;
          }
          *pmin = dummy;
          continue;
        }else{
            goto badopt;
        }
      case '?':
            goto badopt;
      default:
            goto badopt;
      }
      
      i++; //increments

  }
  //if (*opt == '-') ++opt;  if a short is detected then we go and increment to get the juicy stuff
/*  IF its equal to version then print version but we alreayd got that done.
  if (!strcmp(opt, "version")) {  
    //sprintf(errmsg, "%s %s\n", progname, version); 
    return;
  }
  */
 // oc = *opt;  sets variable oc to whats in argv so now we do the checks
/*
//if oc is the argument for the option (aka is digit)
  if (isdigit(oc)) {
    if (!strtoudec(opt, &n)) goto badopt; //copy the char into a digit n and then set into prefix or 
    if (n <= 8) *pprefix = n; //??? If width is >= 9 then we dont need 
    //but yet we only set width when width is >=9 which shouldnt have a w.
    else *pwidth = n;
    //if width is 9 or more then we dont need - w flag
    //if ntohing is specified then it defaults to 72 
    //lets say we got this..bin/par 6 then this means that this gets set to prefix
  }
  else {
    if (!oc) goto badopt;
    n = 1;
    r = strtoudec(opt + 1, &n);
    if (opt[1] && !r) goto badopt;

    if (oc == 'w' || oc == 'p' || oc == 's') {
      if (!r) goto badopt;
      if      (oc == 'w') *pwidth  = n;
      else if (oc == 'p') *pprefix = n;
      else                *psuffix = n;
    }
    else if (oc == 'h'){
      *phang = n;
    }
    else if (n <= 1) {
      if      (oc == 'l') *plast = n;
      else if (oc == 'm') *pmin = n;
    }
    else{ goto badopt;}
  }
*/

  if(optind < argc){
    saveopt = argv[optind];
    goto badopt;
  }

  clear_error();
  return;


badopt: ;
  FILE *stream;

	char *buf;

	size_t len;

	stream = open_memstream(&buf, &len);  
	
	if (stream == NULL){
    set_error("Stream unable to be opened");
		/* handle error */;  
  }
	fprintf(stream, "Bad option: %.149s\n", saveopt);  

	fflush(stream);  
  //sprintf(errmsg, "Bad option: %.149s\n", saveopt);
  fclose(stream);
  set_error(buf);
free(buf);
}


static char **readlines(void)

/* Reads lines from stdin until EOF, or until a blank line is encountered, */
/* in which case the newline is pushed back onto the input stream. Returns */
/* a NULL-terminated array of pointers to individual lines, stripped of    */
/* their newline characters. Uses errmsg, and returns NULL on failure.     */
{
  struct buffer *cbuf = NULL, *pbuf = NULL;
  int c, blank;
  char ch, *ln, *nullline = NULL, nullchar = '\0', **lines = NULL;

  cbuf = newbuffer(sizeof (char));
  if (is_error() == 1) goto rlcleanup;
  pbuf = newbuffer(sizeof (char *));
  if (is_error() == 1) goto rlcleanup;

  for (blank = 1;  ; ) {
    c = getchar();
    if (c == EOF) break;
    if (c == '\n') {
      if (blank) {
        ungetc(c,stdin);
        break;
      }
      additem(cbuf, &nullchar);
      if (is_error() == 1) goto rlcleanup;
      ln = copyitems(cbuf);
      if (is_error() == 1) goto rlcleanup;
      additem(pbuf, &ln);
      if (is_error() == 1) goto rlcleanup;
      clearbuffer(cbuf);
      blank = 1;
    }
    else {
      if (!isspace(c)) blank = 0;
      ch = c;
      additem(cbuf, &ch);
      if (is_error() == 1) goto rlcleanup;
    }
  }

  if (!blank) {
    additem(cbuf, &nullchar);
    if (is_error() == 1) goto rlcleanup;
    ln = copyitems(cbuf);
    if (is_error() == 1) goto rlcleanup;
    additem(pbuf, &ln);
    if (is_error() == 1) goto rlcleanup;
  }

  additem(pbuf, &nullline);
  if (is_error() == 1) goto rlcleanup;
  lines = copyitems(pbuf);

rlcleanup:

  if (cbuf) freebuffer(cbuf);
  if (pbuf) {
    if (!lines)
      for (;;) {
        lines = nextitem(pbuf);
        if (!lines) break;
        free(*lines);
      }
    freebuffer(pbuf);
  }

  return lines;
}


static void setdefaults(
  const char * const *inlines, int *pwidth, int *pprefix,
  int *psuffix, int *phang, int *plast, int *pmin
)
/* If any of *pwidth, *pprefix, *psuffix, *phang, *plast, *pmin are     */
/* less than 0, sets them to default values based on inlines, according */
/* to "par.doc". Does not use errmsg because it always succeeds.        */
{
  int numlines;
  const char *start, *end, * const *line, *p1, *p2;

  if (*pwidth < 0) *pwidth = 72;
  if (*phang < 0) *phang = 0;
  if (*plast < 0) *plast = 0;
  if (*pmin < 0) *pmin = *plast;

  for (line = inlines;  *line;  ++line);
  numlines = line - inlines;

  if (*pprefix < 0){
    if (numlines <= *phang + 1){
      *pprefix = 0;
    }
    else {
      start = inlines[*phang];
      for (end = start;  *end;  ++end);
      for (line = inlines + *phang + 1;  *line;  ++line) {
        for (p1 = start, p2 = *line;  p1 < end && *p1 == *p2;  ++p1, ++p2);
        end = p1;
      }
      *pprefix = end - start;
    }
}

  if ((*psuffix < 0)){
    if (numlines <= 1){
      *psuffix = 0;
    }
    else {
      start = *inlines;
      for (end = start;  *end;  ++end);
      for (line = inlines + 1;  *line;  ++line) {
        for (p2 = *line;  *p2;  ++p2);
        for (p1 = end;
             p1 > start && p2 > *line && p1[-1] == p2[-1];
             --p1, --p2);
        start = p1;
      }
      while (end - start >= 2 && isspace(*start) && isspace(start[1])) ++start;
      *psuffix = end - start;
    }
  }
}


static void freelines(char **lines)
/* Frees the strings pointed to in the NULL-terminated array lines, then */
/* frees the array. Does not use errmsg because it always succeeds.      */
{
/*  char *line;

  for (line = *lines;  *line;  ++line)
    free(line);
*/

  int i=0;
  while(lines[i]){
    free(lines[i]);
    i++;
  }
  free(lines);
}


int original_main(int argc, char **argv)
{
  int width, widthbak = -1, prefix, prefixbak = -1, suffix, suffixbak = -1,
      hang, hangbak = -1, last, lastbak = -1, min, minbak = -1, c;
  char *parinit, *picopy = NULL, *opt, **inlines = NULL, **outlines = NULL,
       **line;
  const char * const whitechars = " \f\n\r\t\v";
  opterr = 0;


  parinit = getenv("PARINIT");
  if (parinit) {
    picopy = malloc((strlen(parinit) + 1) * sizeof (char));
    char** newArgv = malloc((strlen(parinit) + 1) * sizeof (char));
    if (!newArgv) {
      set_error(outofmem);
      goto parcleanup;
    }else{
      newArgv[strlen(parinit)] = '\0';
    }
    if (!picopy) {
      set_error(outofmem);
      goto parcleanup;
    }
    strcpy(picopy,parinit);
    opt = strtok(picopy,whitechars);
    int counter =0;
    while (opt != NULL) {
      newArgv[counter] = opt;
      opt = strtok(NULL,whitechars);
      counter++;
    }
    newArgv = realloc(newArgv, counter+2 * sizeof(char));
    if (!newArgv) {
      set_error(outofmem);
      goto parcleanup;
    }else{
      newArgv[counter+1] = '\0';
    }
    parseopt(argc, newArgv, opt, &widthbak, &prefixbak,
              &suffixbak, &hangbak, &lastbak, &minbak);
    if (is_error() == 1) goto parcleanup;
    opt = strtok(NULL,whitechars);
    free(newArgv);
    newArgv = NULL;
    free(picopy);
    picopy = NULL;
  }

  // while (*++argv != NULL) {
  //   parseopt(argc, argv, *argv, &widthbak, &prefixbak,
  //            &suffixbak, &hangbak, &lastbak, &minbak);
  //   if (is_error() == 1) goto parcleanup;
  // }
  parseopt(argc, argv, *argv, &widthbak, &prefixbak,
             &suffixbak, &hangbak, &lastbak, &minbak);
  if (is_error() == 1) goto parcleanup;

  for (;;) {
    for (;;) {
      c = getchar();
      if (c != '\n') break;
      putchar(c);
    }
    if(c == EOF){
      break;
    }
    ungetc(c,stdin);

    inlines = readlines();
    if (is_error() == 1) goto parcleanup;
    if (!*inlines) {
      free(inlines);
      inlines = NULL;
      continue;
    }

    width = widthbak;  prefix = prefixbak;  suffix = suffixbak;
    hang = hangbak;  last = lastbak;  min = minbak;

    setdefaults((const char * const *) inlines,
                &width, &prefix, &suffix, &hang, &last, &min);

    outlines = reformat((const char * const *) inlines,
                        width, prefix, suffix, hang, last, min);
    if (is_error() == 1) goto parcleanup;

    freelines(inlines);
    inlines = NULL;

    for (line = outlines;  *line;  ++line)
      puts(*line);

    freelines(outlines);
    outlines = NULL;
  }

parcleanup:

  if (picopy) free(picopy);
  if (inlines) freelines(inlines);
  //if (outlines) freelines(outlines);

  if (is_error() == 1) {

    //fprintf(stderr, "%.163s", errmsg);
    report_error(stderr);
    clear_error();
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}