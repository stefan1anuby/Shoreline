#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

/*
In a programming language of your choice, implement a solution to the bello challenge. Please provide a comprehensive suite of tests and answer all of the discussion questions part of the challenge. 

Discussion
    1.How did you represent the social network ? Why did you choose this representation ?

    - The social network was represented by a dictionary , the key was user's id and the value for that key represents a vector of his friends .  The main benefits for this representation are O(1) accesing time ,
    O(friendships_number) space and the flexibility of key types . Some alternatives were vector of lists ( the key can be only integer type ) and adjacency matrix ( has O(N^2) space complexity , which is worse )  

    2.What algorithm did you use to compute the shortest chain of friends ? What alternatives did you consider ? Why did you choose this algorithm over the alternatives ?

    - I choose to do a BFS starting simultaneously from 2 nodes ( users ) , when 2 nodes of different starting users colide , we found the shortest distance between user A and user B.  Other alternatives
    were doing a simple BFS with only 1 starting node and compute the distances between nodes and starting node , and doing Floyd–Warshall algorithm which computes all the distances between 2 nodes
    ( much more expensive in terms of time and space than BFS ) . The BFS with 2 starting nodes is faster than the other 2 variants because it takes much less node visits to find the shortest distance 
    between user A and user B ( this is valid only when we need to compute the shortest path/distance from 1 starting node to 1 finish node in an undirected and unweighted graph , which is the case for this problem ) .

    3.Please enumerate the test cases you considered and explain their relevance.

    - I did 4 test cases of different sizes in the "tests" folder . 
*/



typedef string user;
/// can change from string to anything else

/// Returns the shortest distance between user A and user B
int BFSfrom2Nodes(unordered_map < user , vector < user > > & Graph , user userA , user userB) {

    queue < user > Queue;

    /// Distance[id]:  returns a pair of ints 
    /// first int is the distance between id and starting point , second int is for knowing which user is the starting point ( = 1 for A , = 2 for B , = 0 for not visited)
    unordered_map < user , pair <int , int > > Distance;

    /// Initialization
    for (auto const& personIter : Graph)
    {
        for (auto const& friendIter : personIter.second)
        {
            Distance[friendIter] = { -INT_MAX , 0 };
        }
    }
    Distance[userA] = { 0, 1 };
    Distance[userB] = { 0, 2 };

    /// push both users in Queue to start BFS
    Queue.push(userA);
    Queue.push(userB);

    /// BFS
    while (!Queue.empty() )
    {
        user personID = Queue.front();

        Queue.pop();

        /// iterate over the personID's friends
        for (auto const& friendID : Graph[personID])
        {
            /// check if the person or his friend comes from A ( has .second = 1 ) and the other comes from B ( has .second = 2 )
            if (Distance[friendID].second + Distance[personID].second == 3) 
                /// found the shortest distance between userA and userB
                return Distance[friendID].first + Distance[personID].first + 1;

            /// check if friend/node was not visited
            if (Distance[friendID].first == -INT_MAX ) {
                /// calculate the distance to friendID
                Distance[friendID].first = Distance[personID].first + 1;
                Distance[friendID].second = Distance[personID].second;
                /// push friendID to queue for iteration
                Queue.push(friendID);
            }
        }
    }

    /// no path from user A to user B
    return -1;
}

int main()
{
    int TOTAL_TESTS = 4 , succ_tests = 0;
    for (int test = 0 ; test < TOTAL_TESTS; test++)
    {

        ifstream fin("tests/in" + to_string(test) + ".txt");
        ifstream exp("tests/exp" + to_string(test) + ".txt");
        
        /// creating the graph for each test
        unordered_map < user, vector < user > >  Graph;

        int M;
        fin >> M;
        for (int i = 0; i < M; i++)
        {
            user a, b;
            fin >> a >> b;
            Graph[a].push_back(b);
            Graph[b].push_back(a);
        }

        user userA , userB;

        fin >> userA >> userB;

        int myAns = BFSfrom2Nodes(Graph, userA, userB);

        int expectedAns;
        exp >> expectedAns;

        if (expectedAns == myAns) succ_tests++;

        cout << "The answer computed for test number " << test << " is " << myAns << ", expected is " << expectedAns << '\n';
    }

    cout << "passed tests : " <<succ_tests << "/" << TOTAL_TESTS ;

    return 0;
}