from sys import stdin, stdout

while True:
	line = stdin.readline().strip() 

	elements = line.split()

	if len(elements) == 0:
		break

	for ii in range( 0, len(elements) - 1 ):
		output = "%s\t%s\t1\n" % ( elements[ii].lower(), elements[ii+1].lower() )
		stdout.write(output)

