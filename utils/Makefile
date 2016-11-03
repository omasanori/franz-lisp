# $Header: /usr/src/local/franz/utils/RCS/Makefile,v 1.3 88/10/14 14:25:41 sklower Exp $
# $Locker:  $

LibDir = /usr/lib/lisp
AllSrc = Makefile append.c tackon.c divide.c

AllObj = ${LibDir}/append ${LibDir}/tackon ${LibDir}/divide

all: ${LibDir}/append ${LibDir}/tackon ${LibDir}/divide

${LibDir}/append: append.c
	cc -O -o ${LibDir}/append  -I../franz/h append.c

${LibDir}/tackon: tackon.c
	cc -O -o ${LibDir}/tackon -I../franz/h tackon.c

${LibDir}/divide: divide.c
	cc -O -o ${LibDir}/divide -I../franz/h divide.c

copysource: ${AllSrc}
	(tar cf - ${AllSrc} | (cd ${CopyTo} ; tar xf -))


scriptcatall: ${AllSrc}
	@../scriptcat . utils ${AllSrc}


clean:
	rm ${AllObj}
