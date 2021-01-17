import sys

input_file = open(sys.argv[1], mode = 'r', encoding = 'big5-hkscs')
output_file = open(sys.argv[2], mode = 'w', encoding = 'big5-hkscs')

dic = {}

for line in input_file:
    split = line.split(' ')
    character = split[0]

    ZhuYin = split[1].split('/')
    for zhuyin in ZhuYin:
        if zhuyin[0] in dic:
            dic[zhuyin[0]].append(character)
        else:
            dic[zhuyin[0]] = [character]

for key in sorted(dic.keys()):
    #print (key, "   ", " ".join(dic[key]))
    output_file.write(key + "   " + " ".join(dic[key]) + '\n')
    for char in dic[key]:
        #print (char, "  ", char)
        output_file.write(char + "  " + char + '\n')
