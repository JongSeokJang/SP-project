from sys import stdin, stdout
dic = {}

while True:
	line = stdin.readline().strip() 

	if line == "":
		break

	elements = line.split()

	# key is elements[0], element[1]
	key = elements[0] + ' ' + elements[1]
	cnt = int(elements[2])

	if key in dic:
		dic[key] += cnt
	else:
		dic[key] = cnt

sorted_dic = sorted( dic.items() )

#for key in dic:
for key in sorted_dic:
	output = "( %s ) %d\n" %(key[0], key[1])
	stdout.write(output)
