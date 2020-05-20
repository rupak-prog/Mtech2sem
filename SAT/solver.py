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
					if(len(sorted_list[clause]) == 0): return [],list_of_true_literals
					break
				literal += 1
			if(flag1 == True):
				sorted_list.pop(clause)
				clause -= 1
				size -= 1
			clause += 1
			print(sorted_list)
		sorted_list = list(sorted(sorted_list,key=len))
	if(len(sorted_list)==0): return [],list_of_true_literals
	return sorted_list,list_of_true_literals

def pure_literal_elimination(formula):
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
	return formula

def dpll_algorithm(formula):
	pass

#Main program starts here
file1 = open('c1.txt', 'r') 
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

formula = pure_literal_elimination(formula)
formula,result = unit_propagation(formula)
print(result)
print(formula)
print('literal_count = {} clause_count = {}'.format(literal_count,clause_count))

