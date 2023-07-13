#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>
#include <cstdlib>
#include <time.h>

using namespace std;
using namespace std::chrono;

const int MAX_NODES = 100;
int MAX_COLOR = 10;
const int POPULATION_SIZE = 100;
const int MAX_GENERATIONS = 100;
const int MUTATION_RATE = 10; // percentage

int num_nodes;
vector<vector<int>> graph(MAX_NODES);

vector<int> final_chromosome;


// returns the number of colors used in the choromosome
int colors_used(vector<int> &chromosome)
{

    unordered_map<int, int> map;
    for (auto i : chromosome)
        map[i]++;
    return map.size();
}

// return the max colors used in a chromosome
int max_color_used(vector<int> &chromosome)
{

    int max_color = 0;
    for (auto i : chromosome)
        max_color = max(max_color, i);
    return max_color;
}

// generating a population vector of vector in which each vector will represent the random order of coloring of nodes there are total population_size number of vectors
vector<vector<int>> generate_population(int size)
{
    vector<vector<int>> population;
    for (int i = 0; i < size; i++)
    {
        vector<int> chromosome;
        for (int j = 0; j < num_nodes; j++)
        {
            chromosome.push_back(rand() % MAX_COLOR);
        }
        population.push_back(chromosome);
    }
    return population;
}


// calculating that how many neighbours have the same color as the nodes
int evaluate_chromosome(vector<int> &chromosome)
{
    int conflicts = 0;

    for (int i = 0; i < num_nodes; i++)
    {
        for (int j = 0; j < graph[i].size(); j++)
        {
            if (chromosome[i] == chromosome[graph[i][j]])
            {
                conflicts++;
            }
        }
    }
    return conflicts;
}

// comparing the given population vectors we created randomly and sorting them in the way of minimum number of collisions
static bool compare_chromosomes(vector<int> &a, vector<int> &b)
{

    int fitness_A = evaluate_chromosome(a);
    
    int fitness_B = evaluate_chromosome(b);

    if (fitness_A < fitness_B)
        return true;
    else if (fitness_A > fitness_B)
        return false;
    else
    {
        
        int colors_used_in_A = colors_used(a);
        int colors_used_in_B = colors_used(b);

        if (colors_used_in_A < colors_used_in_B)
            return true;
        else
            return false;
        
    }
}

// selecting randomly two chromsomes from the top half of our vector of vector of populations
vector<vector<int>> select_parents(vector<vector<int>> population)
{
    sort(population.begin(), population.end(), compare_chromosomes);
    int p1 = rand() % (POPULATION_SIZE / 2);
    int p2 = rand() % (POPULATION_SIZE / 2);
    return {population[p1], population[p2]};
    // return population[p1];
}

// creating the crossover between the vectors we get from above function i.e. making a new set of values
vector<int> crossover(vector<int> p1, vector<int> p2)
{
    int crossover_point = rand() % num_nodes;
    vector<int> child;
    for (int i = 0; i < num_nodes; i++)
    {
        if (i < crossover_point)
        {
            child.push_back(p1[i]);
        }
        else
        {
            child.push_back(p2[i]);
        }
    }
    return child;
}

// mutating the vector returned from the crossover to make sure that the answer does not get stored in the local minimum
vector<int> mutate(vector<int> chromosome)
{
    vector<int> mutated = chromosome;
    for (int i = 0; i < num_nodes; i++)
    {
        if (rand() % 100 < MUTATION_RATE)
        {
            mutated[i] = rand() % MAX_COLOR;
        }
    }
    return mutated;
}

// updating the max_color possible for the graph and updating the final_chromosome vector if possible
void update_max_color(vector<int> child)
{

    
    if (evaluate_chromosome(child) == 0)
    {
        if (final_chromosome.size() == 0)
            final_chromosome = child;
        else
        {
            if (colors_used(child) < colors_used(final_chromosome))
                final_chromosome = child;
            else if (colors_used(child) == colors_used(final_chromosome))
            {
                if (max_color_used(child) <= max_color_used(final_chromosome))
                {
                    final_chromosome = child;
                }
            }
        }
        
        int max_val_of_color = -1;
        for(int i =0;i<num_nodes;i++) max_val_of_color= max(max_val_of_color,child[i]);
        MAX_COLOR=min(MAX_COLOR,max_val_of_color);
    }
}

void new_child_in_population(vector<int> &child, vector<vector<int>> &population)
{

    vector<int> last_chromosome = population[POPULATION_SIZE - 1];
    if (evaluate_chromosome(child) < evaluate_chromosome(last_chromosome) || ((evaluate_chromosome(child) == evaluate_chromosome(last_chromosome)) && (colors_used(child) < colors_used(last_chromosome))))
    {
        population.pop_back();
        population.push_back(child);
    }
}

int main()
{

    srand(time(0));

    //     #ifndef ONLINE_JUDGE

    //     // For getting input from input.txt file
    //     freopen("input1.txt", "r", stdin);

    //     // Printing the Output to output.txt file
    //     freopen("output1.txt", "w", stdout);

    // #endif

    // initialize number of nodes in graph
    
    cin >> num_nodes;

    // the maximum number of colors in the graph is the number of nodes itself
    MAX_COLOR = num_nodes;

    // taking the input of the graph
    for (int i = 0; i < num_nodes; i++)
    {
        int num_neighbors;
        cin >> num_neighbors;
        for (int j = 0; j < num_neighbors; j++)
        {
            int neighbor;
            cin >> neighbor;
            graph[i].push_back(neighbor);
        }
    }

    auto start = high_resolution_clock::now();

    // generating the initial population
    vector<vector<int>> population = generate_population(POPULATION_SIZE);

    for (int generation = 0; generation < MAX_GENERATIONS; generation++)
    {
            vector<vector<int>> parents = select_parents(population);
            vector<int> child = crossover(parents[0], parents[1]);

            update_max_color(child);
            new_child_in_population(child, population);
            child = mutate(child);
            update_max_color(child);

            new_child_in_population(child, population);
        
      //  cout << generation << '\n';
    }

    for (auto i : final_chromosome)
        cout << i << " ";
    

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    // To get the value of duration use the count()
    // member function on the duration object
    cout << duration.count() << endl;


    return 0;
}
