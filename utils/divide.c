/* divide :: divide a file into parts by byte numbers, not by
   line (as split does)
 */
#include <stdio.h>

#define DEFCHARS 199000

char *prefix = "dv";	/* prefix char for created files */
int linemode = 0;	/* always break on line boundaries */
int chars = DEFCHARS;	/* number of chars per output file */

int d1 = 0, d2 = 0;
main(argc,argv)
int argc;
char **argv;
{
    int n, next, nbytes, outfile, S ;
    char filename[30];
    int processed = 0;
    char *coreloc;

    argv++;
    
    while(--argc)
    {
	if(argv[0][0] == '-')
	switch(argv[0][1])
	{
	    case '\0':
	    	process(0);	/* process standard input */
		processed++;
		break;
		
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
	    case '8':
	    case '9':
	        chars = atoi(&argv[0][1]);
		if(chars  < 1)
		{
		    fprintf(stderr,"bad char count %d\n",chars);
		    exit(-1);
		}
		break;
	   case 'l':
	   	linemode++;
		break;
	   case 'p':
	   	if(argv[0][2]) prefix = &argv[0][2];
		break;
	   default:
	   	fprintf(stderr,"Bad option %c\n",argv[0][1]);
		exit(-1);
        }
	else
	{
	   process(argv[0]);
	   processed++;
	}
	argv++;
    }
    if(!processed) process(0);	/* if none given, process standard input */
    exit(0);
}

process(name)
char *name;
{
    int infile, outfile, nbytes, retval;
    char filename[100];
    char *coreloc;
    
    if(!name)
    {
        infile = 0;	/* standard input */
	name = "standard input";
    }
    else if((infile = open(name,0)) < 0)
    {
	perror(name);
	exit(-1);
    }

    if((coreloc = (char *) malloc(chars)) <= 0)
    {
	fprintf(stderr,"Allocation of %d bytes failed\n",chars);
    }

    while((retval = fillbuf(coreloc,infile,chars)) > 0)
    {
	if(d2 >= 26) { d1++ ; d2=0;};
	sprintf(filename,"%s%c%c",prefix,
			(int) 'a' + d1,
			(int) 'a' + d2++);
	if((outfile = creat(filename,0666)) < 0)
	{
	    perror(filename);
	    exit(-1);
	}
	if(write(outfile,coreloc,retval) != retval)
	{
	    perror(name);
	    exit(-1);
	}
	if(linemode)
	{
	    while(1)
	    {
		if(read(infile,coreloc,1) > 0)
		{
		    write(outfile,coreloc,1);
		    if(*coreloc == '\n') break;
		}
		else break;
	    }
	}
	close(outfile);
    }
    if(retval < 0)
    {
	perror(name);
	exit(-1);
    }
    free(coreloc);	/* give back space */

}

fillbuf(buffer,filenum,size)
char *buffer;
int filenum,size;
{
    int grab,got;
    char *ptr;
    
    grab = size;
    ptr = buffer;
    while((got = read(filenum,ptr,grab)) > 0)
    {
	ptr += got;
	grab -= got;
	if(grab <= 0) break;
	if((got == 0) && (grab == size)) break;
    }
    if((got <= 0) && (grab == size)) return(got); /* error or eof */
    else return(size-grab);	/* else return amount grabbed */
}


