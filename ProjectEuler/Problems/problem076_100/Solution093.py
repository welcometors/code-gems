from itertools import permutations, combinations

def getAll(a):
    operators = '+-*/'
    s = set()
    for p in permutations(a):
        for o1 in operators:
            for o2 in operators:
                for o3 in operators:
                    e = p[0]+o1+p[1]+o2+p[2]+o3+p[3]
                    try:
                        x = float(eval(e))
                        if x > 0 and x.is_integer():
                            s.add(x)
                    except Exception as ex:
                        pass
                    e = p[0]+o1+'('+p[1]+o2+p[2]+')'+o3+p[3]
                    try:
                        x = float(eval(e))
                        if x > 0 and x.is_integer():
                            s.add(x)
                    except Exception as ex:
                        pass
                    e = '('+p[0]+o1+p[1]+')'+o2+'('+p[2]+o3+p[3]+')'
                    try:
                        x = float(eval(e))
                        if x > 0 and x.is_integer():
                            s.add(x)
                    except Exception as ex:
                        pass
                    e = '('+p[0]+o1+p[1]+')'+o2+p[2]+o3+p[3]
                    try:
                        x = float(eval(e))
                        if x > 0 and x.is_integer():
                            s.add(x)
                    except Exception as ex:
                        pass
                    e = '('+p[0]+o1+p[1]+o2+p[2]+')'+o3+p[3]
                    try:
                        x = float(eval(e))
                        if x > 0 and x.is_integer():
                            s.add(x)
                    except Exception as ex:
                        pass
                    e = p[0]+o1+'('+p[1]+o2+p[2]+o3+p[3]+')'
                    try:
                        x = float(eval(e))
                        if x > 0 and x.is_integer():
                            s.add(x)
                    except Exception as ex:
                        pass
    return sorted([int(x) for x in s])

def check(a):
    l = getAll(a)
    for i,e in enumerate(l):
        if i+1 != e:
            return i
    return len(l)

ans = []
for p in combinations('123456789', 4):
    s = ''.join(p)
    k = check(s)
    #print(s, k)
    ans.append([s,k])

ans.sort(key=lambda x: x[1])
print(ans[-1])