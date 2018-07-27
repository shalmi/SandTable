print("hello")
f = open('starTest.gcode', 'r')
list = []
for line in f:
    parts = line.split()
    if len(parts) == 3:
        if parts[0] == "G01":
            x = parts[1][1:]
            y = parts[2][1:]
            # print(parts[1][1:],parts[2][1:])
            list.append((x,y))
print(list)
# return list