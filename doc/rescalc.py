import math

resistance = [8200, 4700, 2200, 1200]
baseRes = 2700

def calculateRatio(base, resistances, switchArr):
    out = 0
    for i in range(0, len(resistances)):
        out = out + (switchArr[i]/(resistances[i]))
    #print(out)
    out = 1/out
    out = base/(out + base)
    return(out)

print(math.floor(calculateRatio(baseRes, resistance, [1,0,0,0]) * 1023))

rescodes = []

for i in range (2**len(resistance)):
    code = [int(x) for x in bin(i)[2:]]
    rescodes.append(code)

print(rescodes)