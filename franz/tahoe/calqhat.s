	.align	1
	.globl	_calqhat
_calqhat:
	.word	0xffc
	movl	4(fp),r11		# &u[j] into r11
	movl	8(fp),r10		# &v[1] into r10
	cmpl	(r10),(r11)		# v[1] == u[j] ??
	beql	L102			
	# calculate qhat and rhat simultaneously,
	#  qhat in r0
	#  rhat in r1
	emul	(r11),$0x40000000,4(r11),r4 # u[j]b+u[j+1] into r4,r5
	ediv	(r10),r4,r0,r1		# qhat = ((u[j]b+u[j+1])/v[1]) into r0
					# (u[j]b+u[j+1] -qhat*v[1]) into r1
					# called rhat
L101:
	# check if v[2]*qhat > rhat*b+u[j+2]
	emul	r0,4(r10),$0,r2		# qhat*v[2] into r3,r2
	emul	r1,$0x40000000,8(r11),r8 #rhat*b + u[j+2] into r9,r8
	# give up if r3,r2 <= r9,r8, otherwise iterate
	addl2	$0,r2			# explicitly clear carry.
	subl2	r8,r2			# perform r3,r2 - r9,r8
	sbwc	r9,r3
	bleq	L103			# give up if negative or equal
	decl	r0			# otherwise, qhat = qhat - 1
	addl2	(r10),r1		# since dec'ed qhat, inc rhat by v[1]
	jbr	L101
L102:	
	# get here if v[1]==u[j]
	# set qhat to b-1
	# rhat is easily calculated since if we substitute b-1 for qhat in
	# the formula, then it simplifies to (u[j+1] + v[1])
	# 
	addl3	4(r11),(r10),r1		# rhat = u[j+1] + v[1]
	movl	$0x3fffffff,r0		# qhat = b-1
	jbr	L101
	
L103:
	ret

