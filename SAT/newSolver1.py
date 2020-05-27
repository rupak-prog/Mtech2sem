import sys
def removeEmpty(formula):
    size = len(formula)
    i = 0
    while(i < size):
        if(len(formula[i]) == 0):
            formula.pop(i)
            i -= 1
            size -= 1
        i += 1
    if(size == 0):
        return []
    return formula

def unit_propagation(formula):
    formula = toList(formula)

    if(formula == [0]): return [0],[]
    formula = removeEmpty(formula)
    if(formula == []): return [],[]
    formula = list(sorted(formula, key = len))
    list_of_true_literals = []
    while(len(formula) is not 0 and len(formula[0]) is 1):
        lit = formula[0][0]
        size = len(formula)
        clause = 0
        list_of_true_literals.append(lit)
        while clause < size:
            flag1 = False
            flag2 = False
            size1 = len(formula[clause])
            if(size1 == 0): falg1 = True
            literal = 0
            while literal < size1:
                if(formula[clause][literal] == lit):
                    flag1 = True
                    break
                elif(formula[clause][literal] == -lit):
                    formula[clause].pop(literal)
                    if(len(formula[clause]) == 0): return [0],[]
                    break
                literal += 1
            if(flag1 == True):
                formula.pop(clause)
                clause -= 1
                size -= 1
            clause += 1
        formula = removeEmpty(formula)
        formula = list(sorted(formula,key=len))
    if(len(formula)==0): return [],list_of_true_literals
    return formula,list_of_true_literals

def findNewFormula(formula,lit):

    newFormula = []
    for clause in formula:
        newFormula.append(list(clause))

    size = len(newFormula)
    clause = 0
    while clause < size:
        flag1 = False
        size1 = len(newFormula[clause])
        if(size1 == 0): flag1 = True
        literal = 0
        while literal < size1:
            if(newFormula[clause][literal] == lit):
                flag1 = True
                break
            elif(newFormula[clause][literal] == -lit):
                newFormula[clause].pop(literal)
                if(len(newFormula[clause]) == 0): return [0]
                break
            literal += 1
        if(flag1 == True):
            newFormula.pop(clause)
            clause -= 1
            size -= 1
        clause += 1
    return newFormula

def find_in_formula(formula,lit):
    formula = toList(formula)
    for clause in formula:
        for literal in clause:
            if(literal == lit or literal == -lit): return True
    return False

def toList(formula):
    newFormula = []
    for clause in formula:
        newFormula.append(list(clause))
    return newFormula

def toTuple(formula):
    const_formula = []
    for clause in formula:
        const_formula.append(tuple(clause))
    const_formula = tuple(const_formula)
    return const_formula

def found_in_result(result,lit):
    for literal in result:
        if(literal==lit or literal==-lit): return True
    return False

def pure_literal_elimination(formula,unassigned,result):
    formula = toList(formula)
    for lit in unassigned:
        check = 0
        pos = False
        neg = False
        for clause in formula:
            for literal in clause:
                if(literal == lit):
                    pos = True
                if(literal == -lit):
                    neg = True
        if(pos==True and neg==True): continue
        elif(pos==True): check = lit
        elif(neg==True): check = -lit
        else:
            result.append(lit)
            unassigned.remove(lit)
            continue

        size = len(formula)
        for i in range(size):
            size1 = len(formula[i])
            for j in range(size1):
                if(formula[i][j] == check):
                    formula[i].pop(j)
                    j -= 1
                    size1 -= 1
                    break
            if(size1 == 0):
                formula.pop(i)
                size -= 1
                i -= 1
        result.append(check)
        unassigned.remove(lit)
    if(len(formula) == 0): return []
    return formula
    

def dpll_algorithm(formula,literal_count,unassigned,result):

    formula = pure_literal_elimination(formula, unassigned, result)
    if(formula == []): return result
    formula = toTuple(formula)

    #print(formula,unassigned,result)
    #return []

    if(len(result) == literal_count): return result
    formula,result1 = unit_propagation(formula)
    result.extend(result1)

    if(formula == [0]): return []
    if(len(result) == literal_count): return result

    formula = toTuple(formula)
    if(len(formula) == 0): return result
    unassigned  = list(set(unassigned).difference(set(result1)))
    size_of_unassigned = len(unassigned)

    #print("unassigned = {}".format(unassigned))

    for i in range(size_of_unassigned):
        
        if(find_in_formula(formula,unassigned[i])):

            #print("forward move {}{}".format(i,unassigned[i]))
            #print(formula)

            newUnassigned = unassigned[:]
            newResult = result[:]
            newResult.append(unassigned[i])
            newUnassigned.pop(i)

            newFormula = findNewFormula(formula,unassigned[i])
            temp = []
            if(newFormula is not [0]): 
                newFormula = toTuple(newFormula)
                temp = dpll_algorithm(newFormula, literal_count, newUnassigned, newResult)
            
            #print("temp = {}".format(temp))
            
            if(len(temp) == literal_count): 
                result = temp
                return result
            #print("backward move {}{}".format(i,-unassigned[i]))
            #print(formula)

            newUnassigned = unassigned[:]
            newResult = result[:]
            newResult.append(-unassigned[i])
            newUnassigned.pop(i)

            newFormula = findNewFormula(formula,-unassigned[i])
            if(newFormula is not [0]): 
                newFormula = toTuple(newFormula)
                temp = dpll_algorithm(newFormula, literal_count, newUnassigned, newResult)
            
            #print("temp = {}".format(temp))            
            
            if(len(temp) == literal_count): 
                result = temp
                return result
        elif (not found_in_result(result,unassigned[i])):
            
            #print("not found= {}".format(result))
            newResult = result[:]
            newResult.append(unassigned[i])
            newUnassigned = unassigned[:]
            newUnassigned.pop(i)
            temp = dpll_algorithm(formula, literal_count, newUnassigned, newResult)
            if(len(temp) == literal_count): 
                result = temp
                return result
            #print("temp = {}".format(temp))
    return []

def preprocessing(formula,literal_count):
    formula = toList(formula)
    size = len(formula)
    clause = 0
    while clause < size:
        count = []
        i = 0
        for i in range(literal_count):
            count.append(0)
        formula[clause] = list(set(formula[clause]))
        size1 = len(formula[clause])
        literal = 0
        while literal < size1:
            if(formula[clause][literal] < 0):
                count[abs(formula[clause][literal])-1] += 1
            else: count[formula[clause][literal]-1] += 1
            literal += 1
        flag = False
        for j in range(literal_count):
            if(count[j] > 1):
                flag = True
                break
        if(flag == True):
            size -= 1
            formula.pop(clause)
            clause -= 1
        clause += 1
    return toTuple(formula)


#Main program starts here
file1 = open('test1/a1.cnf', 'r') 
Lines = file1.readlines()   
count = 0 
literal_count = 0
clause_count = 0
formula = []
for line in Lines: 
    if(count == 0):
        l = line.split()
        literal_count = int(l[2])
        clause_count = int(l[3])
    else:
        formula.append(list(map(int, line.split())))
    count+=1
unassiged = []
result = []
for i in range(1,literal_count+1):
    unassiged.append(i)
#print(formula)
const_formula = []
flag = False
for clause in formula:
    if(len(tuple(clause[:-1])) == 0): 
        flag = True
        break
    const_formula.append(tuple(clause[:-1]))
const_formula = tuple(const_formula)

if(flag == False):
    const_formula = preprocessing(const_formula,literal_count)
    #print(const_formula)
    solution = dpll_algorithm(const_formula, literal_count, unassiged, result)
    if(solution == []): print("Unsatisfiable")
    else: 
        print("Satisfiable SAT")
        print(solution)
else:
    print("Unsatisfiable")