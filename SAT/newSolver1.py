import sys

"""
function = removeEmpty
argument = boolean formula
return = formula containing no empty clauses
"""
def removeEmpty(formula):
    size = len(formula)
    i = 0
    while(i < size):
        if(len(formula[i]) == 0):   # check if clause is empty
            formula.pop(i)          # pop clause at index i from the formula
            i -= 1                  # decrement value of i
            size -= 1               # decrement size of the formula
        i += 1
    if(size == 0):                  # if all the clause are empty
        return []                   # formula is satisfiable
    return formula

"""
function = unit_propagation
argument = tuple of boolean formula
return = formula containing no unit clause (clause containing only one literal is set to true and removed) and 
         list of true literals
"""

def unit_propagation(formula,result,unassigned):
    formula = toList(formula)                               # convert formula from tuple to list

    if(formula == [0]): return [0]                      # [0] means formula is unsatifiable return [0]
    formula = removeEmpty(formula)                          
    if(formula == []): return []
    formula = list(sorted(formula, key = len))              # sort the formula based on length of clauses
    #list_of_true_literals = []                      
    while(len(formula) is not 0 and len(formula[0]) is 1):  # loop until length of formula is 0 or len of clause at index 0 is 1   
        lit = formula[0][0]                                 
        size = len(formula)
        clause = 0
        result.append(lit)
        unassigned.remove(abs(lit))
        while clause < size:
            flag1 = False
            flag2 = False
            size1 = len(formula[clause])
            if(size1 == 0): falg1 = True
            literal = 0
            while literal < size1:
                if(formula[clause][literal] == lit):        # if lit is found in clause  then delete that clause
                    flag1 = True
                    break
                elif(formula[clause][literal] == -lit):     # if -lit is found in clause then 
                    formula[clause].pop(literal)            # delete that literal form the clause
                    if(len(formula[clause]) == 0): return [0]    # if that clause become empty than formula is unsatisfiable
                    break
                literal += 1
            if(flag1 == True):
                formula.pop(clause)                         # delete the clause
                clause -= 1
                size -= 1
            clause += 1
        formula = removeEmpty(formula)
        formula = list(sorted(formula,key=len))
    if(len(formula)==0): return []
    return formula

"""
function = findNewFormula
argument = boolean formula and literal which is to be set to true
return = new formula in which lit is assigned true
"""

def findNewFormula(formula,lit):

    newFormula = toList(formula)
    size = len(newFormula)
    clause = 0
    while clause < size:                                    # loop for each clause in the formula
        flag1 = False                                       
        size1 = len(newFormula[clause])
        if(size1 == 0): flag1 = True
        literal = 0
        while literal < size1:                              #loop for each literal in formula
            if(newFormula[clause][literal] == lit):         # lit is found than set flag1 to true
                flag1 = True
                break
            elif(newFormula[clause][literal] == -lit):      # if -lit is found than 
                newFormula[clause].pop(literal)             # remove -lit from the clause
                if(len(newFormula[clause]) == 0): return [0]
                break
            literal += 1
        if(flag1 == True):
            newFormula.pop(clause)                          # if flag1 is true remove clause from the formula 
            clause -= 1
            size -= 1
        clause += 1
    return newFormula

"""
fuction = find_in_formula
arguments = boolean formula and lit (We want to find literal is present in formula or not)
return = boolean(True means literal is present in formula otherwise False)
"""

def find_in_formula(formula,lit):
    formula = toList(formula)
    for clause in formula:
        for literal in clause:
            if(literal == lit or literal == -lit): return True   # check if literal is lit or -lit in the clause return True if found
    return False                                                 # return false

"""
function = toList
argument = boolean formula represented in tuple form [example = ((1,2),(2,),(2,3,4))]
return = boolean formula represented in list form [[1,2],[2],[2,3,4]]
"""

def toList(formula):
    newFormula = []
    for clause in formula:
        newFormula.append(list(clause))
    return newFormula

"""
function = toTuple
This function exactly reverse of toList
return = formula represented in Tuple
"""

def toTuple(formula):
    const_formula = []
    for clause in formula:
        const_formula.append(tuple(clause))
    const_formula = tuple(const_formula)
    return const_formula

"""
function = found_in_result
arguments = list and literal
return = True if lit is present in list
"""

def found_in_result(result,lit):
    for literal in result:
        if(literal==lit or literal==-lit): return True
    return False

"""
functinon = pure_literal_elimination (pure literal means the literal which are present in all the clauses and only one
            assingment is possible for that)
arguments = boolean formula(Tuple) , list of unassigned literal(No true or false value is assigned yet), result contain
            all the literals which are assined true
returns   = boolean formula containing no pure literal
"""

def pure_literal_elimination(formula,unassigned,result):
    if(formula == []): return []
    formula = toList(formula)
    
    # loop for each literal in the unassigned list of literals
    for lit in unassigned:
        check = 0
        pos = False
        neg = False

        # loop for each literal of each clause in the formula
        for clause in formula:
            for literal in clause:

                # if lit is found in clause than set pos to true
                if(literal == lit):
                    pos = True

                # if -lit is found in clause than set neg to true
                if(literal == -lit):
                    neg = True


        if(pos==True and neg==True): continue  # if both pos and neg are true than this literal is not pure literal
        elif(pos==True): check = lit            # if pos is true than lit is pure 
        elif(neg==True): check = -lit           # if neg is true than -lit is pure
        else:                                   # if both pos and neg are false than this literal is not part of formula
            result.append(lit)                  # so we remove from unassigned and append in result
            unassigned.remove(lit)
            continue

        # Now remove pure literal from the clauses
        size = len(formula)
        i = 0
        while i < size:
            size1 = len(formula[i])
            j = 0
            while j < size1:
                if(formula[i][j] == check):  # check is the pure literal so if check is found than 
                    formula[i].pop(j)           # remove that literal from the clause
                    j -= 1
                    size1 -= 1
                    break
                j += 1
            if(size1 == 0):                 # if size of clause become empty
                formula.pop(i)              # remove that clause from the formula
                size -= 1
                i -= 1
            i += 1
        result.append(check)
        unassigned.remove(lit)
    if(len(formula) == 0): return []
    return formula
    
"""
function = dpll_algorithm(backtracking algorithm for boolean satisfability)
arguments = boolean formula(Tuple), literal_count, unassigned literal, result(literals which are assigned true)
returns =  [] if formula is unsatisfiable , list of true literals if formula is satisfiable
"""

def dpll_algorithm(formula,literal_count,unassigned,result):

    if(formula == [0]): return []
    formula = pure_literal_elimination(formula, unassigned, result)     # remove pure literal
    if(formula == []): return result
    formula = toTuple(formula)

    if(len(result) == literal_count): return result  # if length of result is equals to literal count than return result
    formula = unit_propagation(formula,result,unassigned) # remove unit clauses from the formula
    if(formula == [0]): return []               # if returned value from unit propagation is [0] means there is conflict so return
    if(len(result) == literal_count): return result

    formula = toTuple(formula)
    if(len(formula) == 0): return result
    size_of_unassigned = len(unassigned)

    # loop for each literal in the unassigned list
    for i in range(size_of_unassigned):
        
        # if unassigned[i] is present in formula
        if(find_in_formula(formula,unassigned[i])):

            newUnassigned = unassigned[:]       # copy unassiged list to new list 
            newResult = result[:]               # copy result list to new list
            newResult.append(unassigned[i])     # append unassigned[i] to newresult list
            newUnassigned.pop(i)

            newFormula = findNewFormula(formula,unassigned[i])  # find new formula in which unassigned[i] is set to true and -unassigned[i] is set to false
            temp = []
            if(newFormula is not [0]):      # if conflict is not found 
                newFormula = toTuple(newFormula)
                temp = dpll_algorithm(newFormula, literal_count, newUnassigned, newResult)      # recursive call to dpll algo with new formula
            
            if(len(temp) == literal_count): # if result found than return result
                result = temp
                return result

            newUnassigned = unassigned[:]
            newResult = result[:]
            newResult.append(-unassigned[i])
            newUnassigned.pop(i)

            newFormula = findNewFormula(formula,-unassigned[i]) # find new formula in which -unassigned[i] is set to true and unassigned[i] is set to false
            if(newFormula is not [0]): 
                newFormula = toTuple(newFormula)
                temp = dpll_algorithm(newFormula, literal_count, newUnassigned, newResult)    # recursively call dpll algo        

            if(len(temp) == literal_count): 
                result = temp
                return result
        elif (not found_in_result(result,unassigned[i])):   # if unassinged is not found in result and not found in formula
    
            newResult = result[:]
            newResult.append(unassigned[i])         # this literal may take any value true or flase so add this to result
            newUnassigned = unassigned[:]
            newUnassigned.pop(i)
            temp = dpll_algorithm(formula, literal_count, newUnassigned, newResult) # call to dpll again
            if(len(temp) == literal_count): 
                result = temp
                return result
    return []

"""
function  = preprocessing
Description = If clause contains more than one literal of same type than it either remove that literal or 
arguments = boolean formula(Tuple), Number of literal in the formula
returns = boolean formula 
"""

def preprocessing(formula,literal_count):
    formula = toList(formula)
    size = len(formula)
    clause = 0
    while clause < size:
        count = []
        i = 0

        # set count of each literal to 0
        for i in range(literal_count):
            count.append(0)
        formula[clause] = list(set(formula[clause]))
        size1 = len(formula[clause])
        literal = 0

        # if literal is found either positive or negative than increment count
        while literal < size1:
            if(formula[clause][literal] < 0):
                count[abs(formula[clause][literal])-1] += 1
            else: count[formula[clause][literal]-1] += 1
            literal += 1
        flag = False

        # if count of literal is more than one than remove that clause
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

"""
function = parseInput
argument = file name which in DIMAC cnf format
returns = list containing clauses 
"""

def parseInput(fileName):
    file1 = open(fileName, 'r') 
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
    return literal_count,clause_count,formula


# program starts here
literal_count,clause_count,formula = parseInput(sys.argv[1]) # parse the file store into the list
unassiged = []
result = []

# create list of unassigned clauses 1 to literal count
for i in range(1,literal_count+1):
    unassiged.append(i)

# remove 0 from each clause and convert it into tuple
const_formula = []
flag = False
for clause in formula:
    if(len(tuple(clause[:-1])) == 0): 
        flag = True
        break
    const_formula.append(tuple(clause[:-1]))
const_formula = tuple(const_formula)

# if none of the clause is empty
if(flag == False):
    const_formula = preprocessing(const_formula,literal_count) # preprocess the formula
    #print(const_formula)
    solution = dpll_algorithm(const_formula, literal_count, unassiged, result) # apply dpll algo
    if(solution == []): print("Unsatisfiable")
    else: 
        print("Satisfiable SAT")
        print(solution)
else: # if at least one of the clause is empty initially
    print("Unsatisfiable")