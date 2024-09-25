from random import randint
from random import random
from random import sample

# Genetic Algorithm for 0/1 Knapsack Problem (tournament selection and uniform crossover)

#knapfile = 'trivial3.txt'
#global_optimum = 13

#knapfile = 'easier7.txt'
#global_optimum = 200

knapfile = 'easy20.txt'
#global_optimum = 726

#knapfile = 'hard200.txt'

with open(knapfile, 'r') as kfile:
    lines = kfile.readlines()
n = int(lines[0]) # Number of items
c = int(lines[n+1]) # Knapsack capacity
items = {int(line.split()[0]) : tuple(map(float, line.split()))[1:] for line in lines[1:n+1]} # Dict of possible items

# GA parameters
max_generations = 50
K = 2 # tournament size
pX = 0.85 # Overall crossover rate
pU = 0.5 # Uniform crossover rate
pM = 0.1 # Mutation rate
P = 10 # Population size
E = 2 # number of Elites

def roulette_selection(pop, max_pieces=10):
    """
    Takes a population list of integer lists (where each number represents the quantity of items chosen for each object)
    and returns an individual selected via roulette wheel selection. The probability of selection is proportional to the fitness.
    """
    fitnesses = [vFitness(individual) for individual in pop]
    total_fitness = sum(fitnesses)
    selection_probs = [fitness / total_fitness for fitness in fitnesses]
    
    # Simulate the roulette wheel
    spin = random() * total_fitness
    cumulative_prob = 0.0
    
    for i, fitness in enumerate(fitnesses):
        cumulative_prob += fitness
        if cumulative_prob >= spin:
            return pop[i]

def initialize_population(n, P, max_pieces=10):
    """
    Generates a list of integer lists (each representing the quantity of items chosen for each object, from 0 to max_pieces).
    """
    popS = set()
    while len(popS) < P:
        b0weight = c + 1
        while b0weight > c:  # the starting population only includes valid solutions
            # Generate a random solution with quantities from 0 to max_pieces for each item
            b0 = tuple(randint(0, max_pieces) for _ in range(n))
            b0weight = wFitness(b0)
        popS.add(b0)
    return [list(elem) for elem in list(popS)]  # Converts a set of tuples into a list of lists

def mutate(human, max_pieces=10, pM=0.1):
    """
    Takes an integer list (representing quantities of items) and mutates quantities at a probability of pM.
    Mutation results in a random value between 0 and max_pieces for each item.
    """
    xman = human[:]
    for i in range(n):
        if pM > random():
            xman[i] = randint(0, max_pieces)  # Random quantity between 0 and max_pieces
    return xman

def unifXover(parentA, parentB):
    """
    Takes 2 binary lists and with probablity pX performs uniform crossover at probability pU to produce a list of 2 new binary lists.
    """
    childA = parentA[:]
    childB = parentB[:]
    if pX > random():
        for i in range(n):
            #if (1/float(n)) > random():
            if pU > random():
                childA[i] = parentB[i]
                childB[i] = parentA[i]
    return [childA, childB]
        
def packing_info(b):
    """
    Accepts a binary list denoting packed items and returns a list of their index numbers, total value and total weight.
    """
    indexes = []
    total_value = 0
    total_weight = 0
    for idx, val in enumerate(b):
        if val == 1:
            indexes.append(idx+1)
            total_value += items[idx+1][0]
            total_weight += items[idx+1][1]               
    return [indexes, total_value, total_weight]

def vFitness(b):
    """
    Accepts a list denoting the quantities of packed items and returns their total value.
    """
    total_value = 0
    for idx, quantity in enumerate(b):
        total_value += items[idx + 1][0] * quantity  # Multiply value by the quantity of items
    return total_value

def wFitness(b):
    """
    Accepts a list denoting the quantities of packed items and returns their total weight.
    """
    total_weight = 0
    for idx, quantity in enumerate(b):
        total_weight += items[idx + 1][1] * quantity  # Multiply weight by the quantity of items
    return total_weight


def tournament_selection(pop, K):
    """
    Takes population list of binary lists and tournament size and returns a winning binary list.
    """
    tBest = 'None'
    for i in range(K):
        contestant = pop[randint(0, P-1)]
        if (tBest == 'None') or vFitness(contestant) > vFitness(tBest):
            tBest = contestant[:]
    return tBest

def initialize_population():
    """
    Generates a list of binary lists each representing a valid packing selection.
    """
    popS = set()
    while len(popS) < P:
        b0weight = c+1
        while b0weight > c: # the starting population only includes valid solutions
            b0 = tuple(randint(0,1) for _ in range(n))
            b0weight = wFitness(b0)
        popS.add(b0)
    return [list(elem) for elem in list(popS)] # Converts a set of tuples into a list of lists

def select_elites():
    """
    Selects the E best solutions from the population and returns them as a list of binary lists.
    """
    elites = []
    while len(elites) < E: # Keep choosing elites until there are E of them
        new_elites = popD[max(popD)] # These are the binary lists with the best fitness
        # If adding all the elites with this fitness would be too many, then discard the surplus at random
        while len(new_elites) > (E - len(elites)):
            new_elites.remove(sample(new_elites, 1)[0])
        elites.extend(new_elites)
        popD.pop(max(popD), None) # Remove the key with the value just added from popD{}
    return elites

def popMean():
    """
    Calculate the mean fitness of the current generation
    """
    t = 0
    for i in popR:
        t += i[0]
    return t/P

def report():
    return "max: "+str(max(popD))+", mean: "+str(popMean())+", min: "+str(min(popD))

def updateBest():
    return [max(popD), popD[max(popD)][0], s]

def rankedList():
    # Make a list of each binary list and its fitness
    return [(vFitness(i), i) for i in popL]
   
def rankedDict():
    # Make a dictionary where keys are fitness and values are tuples of the binary lists with that fitness
    popD = {}
    for item in popR:
        key = item[0]
        popD.setdefault(key, []).append(item[-1])
    return popD

# Create an initial population
popL = initialize_population()
popR = rankedList()
popD = rankedDict()
s = 0 # the generation counter
bestResults = updateBest()
print("Starting population - " + str(report()))

while True:
    s += 1
    popR = rankedList()
    popD = rankedDict()

    # Update current best
    if max(popD) > bestResults[0]:
        bestResults = updateBest()

    # Stop if optimum attained (optimum only known if enumeration was possible)    
    if 'global_optimum' in globals():
        if bestResults[0] == global_optimum:
            print ("Global Optimum reached after "+str(s)+" generations.")
            break

    # Stop if time is up
    if s == max_generations:
        print ("Stopped after "+str(s)+" generations.")
        break
        
    # Give an update every 10% of total progress
    if s % (max_generations / 10) == 0:
        print ("Best: "+str(bestResults[0])+", "+str(max_generations - s)+" generations remaining. Current population - "+str(report()))
    
    # Start the child generation with E elites (direct copies from current generation)
    nextGen = select_elites()
        
    # Fill the next generation to size P, same size as the previous one
    while len(nextGen) < P:
        parentA = tournament_selection(popL, K) # Selection
        parentB = tournament_selection(popL, K)
        childrenAB = unifXover(parentA, parentB) # Crossover
        mutatedChildA = mutate(childrenAB[0]) # Mutation
        mutatedChildB = mutate(childrenAB[1])
        if (wFitness(mutatedChildA) <= c) and (wFitness(mutatedChildB) <= c): # Discard infeasible solutions
            nextGen.extend([mutatedChildA, mutatedChildB])
               
    popL = nextGen[:]

packing_info = packing_info(bestResults[1])
print ("\nBest feasible solution found (on generation "+str(bestResults[2])+"/"+str(max_generations)+"): value="+str(packing_info[1])+", weight="+str(packing_info[2])+"/"+str(c)+"\n"+str(len(packing_info[0]))+"/"+str(n)+" items: "+str(packing_info[0]))