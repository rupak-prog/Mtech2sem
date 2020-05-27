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

def dpll_algorithm(formula,literal_count,unassigned,result):

    if(len(result) == literal_count): return result
    formula,result1 = unit_propagation(formula)
    result.extend(result1)

    if(len(result) == literal_count): return result
    if(formula == [0]): return []

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
        else:
            #print("not found= {}".format(unassiged[i]))
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

#Main program starts here
file1 = open('c10.txt', 'r') 
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
#formula = preprocessing(formula,literal_count)
#print(formula)
const_formula = []
for clause in formula:
    const_formula.append(tuple(clause))
const_formula = tuple(const_formula)
solution = dpll_algorithm(const_formula, literal_count, unassiged, result)
if(solution == []): print("Unsatisfiable")
else:
    print("Satisfiable SAT")
    print(solution)
    