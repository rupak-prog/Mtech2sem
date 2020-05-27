import enum
import random
class RetVal(enum.Enum):
    r_satisfied = 0
    r_unsatisfied = 1
    r_normal = 2

class SATSolverCDCL:
    literals = []
    literal_list_per_clause = []
    literal_frequency = []
    literal_polarity = []
    original_literal_frequency = []
    literal_count = None
    clause_count = None
    kappa_antecedent = None
    literal_decision_level = []
    literal_antecedent = []
    assigned_literal_count = None
    already_unsatisfied = None #boolean
    pick_counter = None

    def initialize(self):
        file1 = open('c1.txt', 'r') 
        Lines = file1.readlines()   
        count = 0 
        self.literal_count = 0
        self.clause_count = 0
        self.assigned_literal_count = 0
        self.kappa_antecedent = -1
        self.pick_counter = 0
        self.already_unsatisfied = False
        for line in Lines: 
            if(count == 0):
                l = line.split()
                literal_count = int(l[2])
                clause_count = int(l[3])
                self.literals = [-1] * literal_count
                self.literal_frequency = [0] * literal_count
                self.literal_polarity = [0] * literal_count
                self.literal_antecedent = [-1] * literal_count
                self.literal_decision_level = [-1] * literal_count
                self.literal_list_per_clause = [[] for _ in range(clause_count)]
                ##print(self.literal_list_per_clause)
            else:
                literal_count_in_clause = 0
                clause = list(map(int, line.split()))
                for literal in clause:
                    if(literal > 0):
                        self.literal_list_per_clause[count-1].append(literal)
                        self.literal_frequency[literal - 1] += 1
                        self.literal_polarity[literal - 1] += 1
                    elif (literal < 0):
                        self.literal_list_per_clause[count-1].append(literal)
                        self.literal_frequency[-1 - literal] += 1
                        self.literal_polarity[-1 - literal] -= 1
                    else:
                        if(literal_count_in_clause == 0):
                            self.already_unsatisfied = True
                        break
                    literal_count_in_clause += 1
            count+=1
        ##print(self.literal_frequency)
        ##print(self.literal_polarity)
        ##print(self.literal_list_per_clause)
        self.original_literal_frequency = self.literal_frequency

    def CDCL(self):
    	#print("inside cdcl")
        decision_level = 0
        if(self.already_unsatisfied):
            return RetVal.r_unsatisfied
        unit_propagate_result = self.unit_propagate(decision_level)
        #print("unit{}".format(unit_propagate_result))
        #print(RetVal.r_normal == RetVal.r_normal)
        if unit_propagate_result == RetVal.r_unsatisfied:
            return unit_propagate_result
        count = 0
        while not self.all_variables_assigned():
            picked_variable = count
            #print(picked_variable)
            decision_level += 1
            self.assign_literal(picked_variable, decision_level, -1)
            if(count == 3): break
            while True:
                unit_propagate_result = self.unit_propagate(decision_level)
                #print(unit_propagate_result)
                if(unit_propagate_result == RetVal.r_unsatisfied):
                    if(decision_level == 0):
                        return unit_propagate_result
                    decision_level = self.conflict_analysis_and_backtrack(decision_level)
                else:
                    break
            count += 1
        return RetVal.r_satisfied

    def literal_to_variable_index(self,variable):
		if variable > 0:
		    return  variable - 1
		return -variable - 1
    

    def unit_propagate(self,decision_level):
    	#print("inside unit_propagate")
        unit_clause_found = False
        false_count = 0
        unset_count = 0
        literal_index = 0
        satisfied_flag = False
        last_unset_literal = -1

        while True:
            unit_clause_found = False
            for i in range(len(self.literal_list_per_clause)):
                if(not unit_clause_found):
                    false_count = 0
                    unset_count = 0
                    satisfied_flag = False

                    for j in range(len(self.literal_list_per_clause[i])):
                        literal_index = self.literal_to_variable_index(self.literal_list_per_clause[i][j])
                        if self.literals[literal_index] == -1:
                            unset_count += 1
                            last_unset_literal = j
                        elif ((self.literals[literal_index]==0 and self.literal_list_per_clause[i][j]>0) or (self.literals[literal_index] == 1 and self.literal_list_per_clause[i][j] < 0)):
                            false_count += 1
                        else:
                            satisfied_flag = False
                            break
                    if(satisfied_flag):
                        continue
                    if(unset_count == 1):
                        self.assign_literal(self.literal_list_per_clause[i][last_unset_literal], decision_level, i)
                        unit_clause_found = True
                        break
                    elif (false_count == len(self.literal_list_per_clause[i])):
                        self.kappa_antecedent = i
                        return RetVal.r_unsatisfied
            if(not unit_clause_found):
                break
        self.kappa_antecedent = -1
        return RetVal.r_normal


    def assign_literal(self,variable, decision_level, antecedent):
        #print("inside assign_literal")
        literal = self.literal_to_variable_index(variable)
        value = int(variable > 0)
        self.literals[literal] = value
        self.literal_decision_level[literal] = decision_level
        self.literal_antecedent[literal] = antecedent
        self.literal_frequency[literal] = -1
        self.assigned_literal_count += 1

    def unassign_literal(self,literal_index):
    	#print("inside unassign_literal")
        self.literals[literal_index] = -1               
        self.literal_decision_level[literal_index] = -1 
        self.literal_antecedent[literal_index] = -1     
        self.literal_frequency[literal_index] = self.original_literal_frequency[literal_index] 
        self.assigned_literal_count -= 1


    def conflict_analysis_and_backtrack(self,decision_level):
        #print("inside conflict_analysis_and_backtrack")
        learnt_clause = self.literal_list_per_clause[self.kappa_antecedent]
        conflict_decision_level = decision_level
        this_level_count = 0
        resolver_literal = 0
        literal = 0
        while True:
            this_level_count = 0
            for i in range(len(learnt_clause)):
                literal = self.literal_to_variable_index(learnt_clause[i])
                if (self.literal_decision_level[literal] == conflict_decision_level):
                    this_level_count += 1
                if (self.literal_decision_level[literal] == conflict_decision_level and self.literal_antecedent[literal] != -1):
                    resolver_literal = literal
            if this_level_count == 1:
                break
            learnt_clause = self.resolve(learnt_clause, resolver_literal)
        self.literal_list_per_clause.append(learnt_clause)

        for i in range(len(learnt_clause)):
            literal_index = self.literal_to_variable_index(learnt_clause[i])
            update = 0
            if learnt_clause[i] > 0:
                update = 1
            else: update = -1
            self.literal_polarity[literal_index] += update

            if self.literal_frequency[literal_index] != -1:
                self.literal_frequency[literal_index] += 1
            self.original_literal_frequency[literal_index] += 1
        self.clause_count += 1

        backtracked_decision_level = 0
        for i in range(len(learnt_clause)):
            literal_index = self.literal_to_variable_index(learnt_clause[i])
            decision_level_here = self.literal_decision_level[literal_index]

            if (decision_level_here != conflict_decision_level and decision_level_here > backtracked_decision_level):
                backtracked_decision_level = decision_level_here

        for i in range(len(literals)):
            if (self.literal_decision_level[i] > backtracked_decision_level):
                self.unassign_literal(i)

        return backtracked_decision_level

    def resolve(self,input_clause,literal):
    	##print("inside resolve")
        second_input = self.literal_list_per_clause[self.literal_antecedent[literal]]
        input_clause.extend(second_input)
        for i in range(len(input_clause)):
            if((input_clause[i] == literal + 1) or (input_clause[i] == -literal - 1)):
                input_clause.pop(i)
                i -= 1
        input_clause = sorted(input_clause)
        input_clause = list(set(input_clause))
        return input_clause

    def pick_branching_variable(self):
    	#print("inside pick_branching_variable")
        random_value = random.randint(1,10)
        too_many_attempts = False
        attempt_counter = 0

        while True:
            if (random_value > 4 or self.assigned_literal_count < self.literal_count / 2 or too_many_attempts):
                self.pick_counter += 1;
                ##print(type(self.pick_counter),type(self.literal_count))
                if (self.pick_counter == 20 * self.literal_count):
                    for i in range(len(self.literals)):
                        self.original_literal_frequency[i] /= 2
                        if(self.literal_frequency[i] != -1):
                            self.literal_frequency[i] /= 2
                    self.pick_counter = 0

                variable = self.literal_frequency.index(max(self.literal_frequency))

                if( self.literal_polarity[variable] >= 0):
                    return variable + 1
                return -variable - 1
                ##print("not returned") 
            else:
                while attempt_counter < 10 * self.literal_count:
                    variable = random.randint(0,self.literal_count-1)
                    if(self.literal_frequency[variable] != -1):
                        if(self.literal_polarity[variable] >= 0):
                            return variable + 1
                        return -variable - 1
                    attempt_counter += 1
                too_many_attempts = True
            if(not too_many_attempts): break

    def all_variables_assigned(self):
        return self.literal_count == self.assigned_literal_count

    def show_result(self,result_status):
        if result_status == RetVal.r_satisfied:
            print("SAT")
            for i in range(len(self.literals)):
                if i != 0:
                    print(" ")
                if(self.literals[i] != -1):
                    print(pow(-1, (literals[i] + 1)) * (i + 1))
                else:
                    print(i + 1)
            print(" 0")
        else:
            print("UNSAT")

    def solve(self):
        result_status = self.CDCL()
        show_result(result_status)


obj = SATSolverCDCL()
obj.initialize()
obj.solve()

