#include <iostream>
#include <string>
#include <vector>
#include <random>

struct solution
{
    std::string word;
    int fitness = 0;
};

int popSize = 1000;
int generations = 1000000;

std::string targetPhrase = "hellothisisaverybigwordiwillusetodemonstrate";
std::vector<solution> solutions;
std::vector<solution> children;
int bestFit = 0;
std::string bestWord = "";

int prevBest = 0;
std::string prevWord = "";


std::random_device dev;
std::mt19937 seed(dev());
std::uniform_int_distribution<> letter(97, 122);
std::uniform_int_distribution<> randParent(0, popSize / 2 - 1);
std::uniform_int_distribution<> mutaton(0, 100);


void setUp()
{
    //intializing vector of solutions
    //creating random words in vector
    for (int i = 0; i < popSize; i++)
    {
        std::string word = "";
        for (int j = 0; j < targetPhrase.size(); j++)
        {
            word.push_back((char)letter(seed));
        }

        solutions.push_back(solution() = { word, 0 });
    }
}

int evalFitness(std::string prediction)
{
    int fitness = 0;

    for (int i = 0; i < targetPhrase.size(); i++)
    {
        if (targetPhrase[i] == prediction[i])
        {
            fitness++;
        }
    }

    return fitness;
}

int main()
{
    setUp();
    std::cout << "Started Algorithm\n";
    std::cout << "Target Phrase: " << targetPhrase << "\n\n";

    for (int z = 0; z < generations; z++)
    {
        //evaluating fitness
        for (int i = 0; i < popSize; i++)
        {
            solutions[i].fitness = evalFitness(solutions[i].word);
        }

        //sorting by fitness
        std::sort(solutions.begin(), solutions.end(), [](const solution& c1, const solution& c2)
            {
                return c1.fitness > c2.fitness;
            });

        bestFit = solutions[0].fitness;
        bestWord = solutions[0].word;

        if (bestFit == targetPhrase.size())
        {
            break;
        }

        //check if best candidate in this generation did better than the best candidate ever
        if (bestFit > prevBest)
        {
            prevBest = bestFit;
            prevWord = bestWord;
            std::cout << bestWord << "\n";
        }


        //eliteism, 10% of elite move on
        for (int i = 0; i < popSize * 0.10; i++)
        {
            children.push_back(solutions[i]);
        }

        //crossover and selection
        for (int i = 0; i < popSize - (popSize * 0.10) / 2; i++)
        {
            std::string child = "";
            std::string child2 = "";

            //selection
            int par1 = randParent(seed);
            int par2 = randParent(seed);

            //crossover
            for (int j = 0; j < targetPhrase.size() / 2; j++)
            {
                //10 percent chance
                if (mutaton(seed) < 10)
                {
                    child.push_back((char)letter(seed));
                    child2.push_back((char)letter(seed));
                }
                else
                {
                    child.push_back(solutions[par1].word[j]);
                    child2.push_back(solutions[par2].word[j]);
                }
            }

            for (int j = targetPhrase.size() / 2; j < targetPhrase.size(); j++)
            {
                //10 percent chance
                if (mutaton(seed) < 10)
                {
                    child.push_back((char)letter(seed));
                    child2.push_back((char)letter(seed));
                }
                else
                {
                    child.push_back(solutions[par2].word[j]);
                    child2.push_back(solutions[par1].word[j]);
                }
            }

            children.push_back(solution() = { child, 0 });
            children.push_back(solution() = { child2, 0 });
        }

        solutions = children;
        children.clear();
    }

    std::cout << "\n" << "--- Best Solution ---\n";
    std::cout << bestFit << "\n";
    std::cout << bestWord << "\n";
    
    std::cout << "\n" << "--- Phrase vs Solution ---\n";
    std::cout << "P:" << targetPhrase << "\n";
    std::cout << "S:" << bestWord << "\n";


    while (getchar() == (int)'\n') {}

}