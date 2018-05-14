xorstr macro str:req,key:req

	local i,l,p,c

	i = 2
	l sizestr <str>
	c equ <>

	repeat l -2

		p substr <str>,i,1
		p catstr <'>,p,<'>
		c catstr c,<,>,%(p xor key)

		i = i + 1

	endm

	l sizestr c
	c substr c,2,l-1

	exitm c

endm

xormem macro var:req,len:req,key:req

	local i

	i = 0

	repeat (len) / 4

		xor dword ptr var[i],(key shl 24) or (key shl 16) or (key shl 8) or key
		i = i + 4

	endm

	while i lt len

		xor byte ptr var[i],key
		i = i + 1

	endm

endm