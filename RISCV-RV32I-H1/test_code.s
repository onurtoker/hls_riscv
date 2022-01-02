# Use the online simulator
# https://www.kvakil.me/venus/

		li      x1,1020
		sw      x0,0(x1)
loop: 	lw      x2,0(x1)
		addi    x2,x2,1
		sw      x2,0(x1)
		j       loop
