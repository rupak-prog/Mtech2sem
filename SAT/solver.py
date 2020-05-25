import sys
def unit_propagation(List):
	sorted_list = list(sorted(List, key = len))
	list_of_true_literals = []
	while(len(sorted_list) is not 0 and len(sorted_list[0]) is 1):
		lit = sorted_list[0][0]
		size = len(sorted_list)
		clause = 0
		list_of_true_literals.append(lit)
		while clause < size:
			flag1 = False
			flag2 = False
			size1 = len(sorted_list[clause])
			literal = 0
			while literal < size1:
				if(sorted_list[clause][literal] == lit):
					flag1 = True
					break
				elif(sorted_list[clause][literal] == -lit):
					sorted_list[clause].pop(literal)
					if(len(sorted_list[clause]) == 0): return [0],list_of_true_literals
					break
				literal += 1
			if(flag1 == True):
				sorted_list.pop(clause)
				clause -= 1
				size -= 1
			clause += 1
		sorted_list = list(sorted(sorted_list,key=len))
	if(len(sorted_list)==0): return [],list_of_true_literals
	return sorted_list,list_of_true_literals

def pure_literal_elimination(formula):
	if(len(formula) == 0): return [],[]
	s = set(formula[0])
	for i in range(1,len(formula)):
		s = s.intersection(set(formula[i]))
	to_remove = list(s)
	size = len(formula)
	j = 0
	while j < size:
		formula[j] = [i for i in formula[j] if i not in to_remove]
		if(len(formula[j]) == 0):
			formula.pop(j)
			size -= 1
		else: j += 1
	return formula,to_remove

def findNewFormula(formula,lit):
	size = len(formula)
	clause = 0
	while clause < size:
		flag1 = False
		size1 = len(formula[clause])
		literal = 0
		while literal < size1:
			if(formula[clause][literal] == lit):
				flag1 = True
				break
			elif(formula[clause][literal] == -lit):
				formula[clause].pop(literal)
				if(len(formula[clause]) == 0): return []
				break
			literal += 1
		if(flag1 == True):
			formula.pop(clause)
			clause -= 1
			size -= 1
		clause += 1
	return formula

def dpll_algorithm(formula, result, literal_count):
	formula,result1 = pure_literal_elimination(formula)
	formula,result2 = unit_propagation(formula)
	if(formula == [0]): return []
	result1.extend(result2)
	result.extend(result1)
	if(len(result) == literal_count): return result
	if(len(formula) == 0): return []
	for i in range(len(formula[0])):
		newFormula = formula[1:]
		newResult = result[:]
		newResult.append(formula[0][i])
		newFormula = findNewFormula(newFormula,formula[0][i])
		temp = dpll_algorithm(newFormula, newResult, literal_count)
		if(len(temp) == literal_count): return temp 
		newResult = result[:]
		newResult.append(-formula[0][i])
		newFormula = formula[1:]
		newFormula = findNewFormula(newFormula,-formula[0][i])
		temp = dpll_algorithm(newFormula, newResult, literal_count)
		if(len(temp) == literal_count): return temp
	return []

def preprocessing(formula,literal_count,):
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
	return formula

def clauseLearning():
	pass

#Main program starts here
file1 = open('c5.txt', 'r') 
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
formula = preprocessing(formula,literal_count)
#print(formula)
solution = dpll_algorithm(formula, result, literal_count)
if(solution == []): print("Unsatisfiable")
else:
	print("Satisfiable SAT")
	print(solution)


