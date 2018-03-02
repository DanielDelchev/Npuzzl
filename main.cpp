#include <iostream>
#include <stdlib.h>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <climits>
#include <cmath>
#include <functional>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>


typedef unsigned int int_t;

enum class Move:int_t{
  UNEXPECTED=0,
  RIGHT=1,
  DOWN=2,
  LEFT=3,
  UP=4,
};

std::string stringify(Move given){
    switch (static_cast<int_t> (given)){
        case 1:
            return "\nthgir";
            break;
        case 2:
            return "\nnwod";
            break;
        case 3:
            return "\ntfel";
            break;
        case 4:
            return "\npu";
            break;
        default:
            return "\n!evom detcepxenU";
            break;
    }
}

inline std::ostream& operator<<(std::ostream& os, const Move& given){
    switch (static_cast<int_t> (given)){
        case 1:
            os<<"right\n";
            break;
        case 2:
            os<<"down\n";
            break;
        case 3:
            os<<"left\n";
            break;
        case 4:
            os<<"up\n";
            break;
        default:
            os<<"Unexpected move!\n";
            break;
    }
    return os;
}

class BoardConfiguration{

private:
    int_t* currentBoard;
    int_t dimention;
    int_t dimentionSquared;
    int_t Hvalue;

    void copy(const BoardConfiguration& given){
        dimention = given.dimention;
        dimentionSquared = given.dimentionSquared;
        Hvalue = given.Hvalue;
    }

protected:
    inline int_t& getElement(int_t rowIndex,int_t columnIndex){
        return currentBoard[rowIndex*dimention+columnIndex];
    }
    inline int_t getElement(int_t rowIndex,int_t columnIndex)const{
        return currentBoard[rowIndex*dimention+columnIndex];
    }

public:

    BoardConfiguration():currentBoard(nullptr),dimention(0),dimentionSquared(0),Hvalue(INT_MAX/2){}

    //Great 6
    BoardConfiguration(int_t N, int_t* arr = nullptr):currentBoard(nullptr),dimention(sqrt(N+1)),dimentionSquared(N+1),Hvalue(0)
        {
            currentBoard = new int_t [dimentionSquared]();
            if (arr!=nullptr){
                memcpy(reinterpret_cast<void*> (currentBoard),reinterpret_cast<void*> (arr),dimentionSquared*sizeof(int_t));
                updateH();
            }

        };

    BoardConfiguration (const BoardConfiguration& given):currentBoard(nullptr){
        copy(given);
        currentBoard = new int_t [dimentionSquared]();
        if (given.currentBoard!=nullptr){
            memcpy(reinterpret_cast<void*> (currentBoard),reinterpret_cast<void*> (given.currentBoard),dimentionSquared*sizeof(int_t));
        }
    }

    BoardConfiguration (BoardConfiguration&& given):currentBoard(nullptr){
        copy(given);
        currentBoard = given.currentBoard;
        given.currentBoard = nullptr;
    }

    BoardConfiguration& operator= (const BoardConfiguration& given){
        if (this!= &given){
            copy(given);
            if (currentBoard){
                delete [] currentBoard;
            }
            currentBoard = new int_t [dimentionSquared]();
            if (given.currentBoard!=nullptr){
                memcpy(reinterpret_cast<void*> (currentBoard),reinterpret_cast<void*> (given.currentBoard),dimentionSquared*sizeof(int_t));
            }
        }
        return *this;
    }


    BoardConfiguration& operator= (BoardConfiguration&& given){
        if (this!= &given){
            copy(given);
            if (currentBoard){
                delete [] currentBoard;
            }
            currentBoard = given.currentBoard;
            given.currentBoard = nullptr;
        }
        return *this;
    }

    virtual ~BoardConfiguration(){
        delete [] currentBoard;
    }

    int_t getNth(int_t index)const{
        return currentBoard[index];
    }

    inline bool operator==(const BoardConfiguration& other)const{
        return (memcmp( reinterpret_cast<void*> (currentBoard), reinterpret_cast<void*>(other.currentBoard),dimentionSquared*sizeof(int_t))==0);
    }

    // returns an alias of a vector of all possible BoardConfiguration derived from the current one
     inline void getNeighbourBoardConfigurations(std::vector< std::pair<BoardConfiguration,Move> >& result)const{

        result.clear();

        BoardConfiguration temp = (*this);

        int_t index = 0;
        while (currentBoard[index] != 0){
            index++;
        }

        //can slide to the right
        //LEFT
        if ( ((index % (dimention))) != (dimention -1 ) ){
            std::swap(temp.currentBoard[index+1],temp.currentBoard[index]);
            temp.updateH();
            result.push_back({temp,Move::LEFT});
            std::swap(temp.currentBoard[index+1],temp.currentBoard[index]);
        }

        //can slide to the left
        //RIGHT
        if ( (index % dimention) !=0){
            std::swap(temp.currentBoard[index-1],temp.currentBoard[index]);
            temp.updateH();
            result.push_back({temp,Move::RIGHT});
            std::swap(temp.currentBoard[index-1],temp.currentBoard[index]);
        }

        //can slide downwards
        //UP
        if (index < (dimentionSquared-dimention)){
            std::swap(temp.currentBoard[index],temp.currentBoard[index+dimention]);
            temp.updateH();
            result.push_back({temp,Move::UP});
            std::swap(temp.currentBoard[index],temp.currentBoard[index+dimention]);

        }

        //can slide upwards
        //DOWN
        if (index > (dimention-1)){
            std::swap(temp.currentBoard[index-dimention],temp.currentBoard[index]);
            temp.updateH();
            result.push_back({temp,Move::DOWN});
            std::swap(temp.currentBoard[index-dimention],temp.currentBoard[index]);
        }

    }

    //setters & getters
    inline void setHvalue(int_t value){Hvalue = value;}
    inline int_t getHvalue()const{return Hvalue;}
    inline int_t getDimention()const{return dimention;}
    inline int_t getDimentionSquared()const{ return dimentionSquared;}


    inline void updateH(){
        int_t H=0;
        for (size_t index=0;index<dimentionSquared;index++){
                int_t expected = 0;
                if (currentBoard[index] == 0){
                    continue;
                }
                expected = currentBoard[index]-1;
                int_t horizontal = abs((expected / dimention) - (index /dimention));
                int_t vertical = abs((expected % dimention) - (index % dimention)) ;
                H += horizontal + vertical;
        }
        Hvalue = H;
    }

    friend inline std::ostream& operator<<(std::ostream& os,const BoardConfiguration& given){
        for (size_t rowIndex=0;rowIndex<given.dimention;rowIndex++){
            for (size_t columnIndex=0;columnIndex<given.dimention;columnIndex++){
                os<<given.currentBoard[rowIndex*given.dimention+columnIndex]<<" ";
            }
            os<<std::endl;
        }
        return os;
    }

    friend inline std::istream& operator>>(std::istream& is,BoardConfiguration& given){
        for (size_t index=0;index<given.dimentionSquared;index++){
            is>>given.currentBoard[index];
        }
        return is;
    }
};

struct BoardHasher
{
    std::size_t operator()(const BoardConfiguration& given) const{
    int_t result = 0;
    for (int_t index=0;index<(given.getDimentionSquared());index++){
        result += std::hash<int_t>()(given.getNth(index)*index);
    }
        return std::hash<int_t>()(result);
    }
};


std::string findPath(BoardConfiguration start){

    std::unordered_map<BoardConfiguration,int_t,BoardHasher> Gval;
    std::unordered_set<BoardConfiguration,BoardHasher> open;
    std::unordered_set<BoardConfiguration,BoardHasher> closed;

    std::function <bool(const BoardConfiguration&,const BoardConfiguration&)> CMP = [&](const BoardConfiguration& one,const BoardConfiguration& other){
            int_t lhs = (one.getHvalue()+Gval[one]);
            int_t rhs = (other.getHvalue()+Gval[other]);
            return ( lhs > rhs);
        };


    std::unordered_map<BoardConfiguration,std::pair<BoardConfiguration,Move>,BoardHasher> fromCache;
    std::priority_queue<BoardConfiguration,std::vector<BoardConfiguration>,std::function<bool(const BoardConfiguration&,const BoardConfiguration&)>> PQ(CMP);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    Gval[start] = 0;
    start.updateH();

    PQ.push(start);
    open.insert(start);
    Gval.insert({start,0});
    fromCache[start] = {start,Move::UNEXPECTED};

    std::vector< std::pair<BoardConfiguration,Move> > V;

    BoardConfiguration placeholder;

    while (!PQ.empty()){

        //get top of PQ, remove it from grey (open), add it to black (closed), remove it from PQ
        BoardConfiguration current = PQ.top();
        open.erase(current);
        closed.insert(current);
        PQ.pop();

        if (current.getHvalue()==0){
            placeholder = current;
            break; //if goal has been reached, break cycle.
        }

        //get successive states, shuffle them so search is not biased
        current.getNeighbourBoardConfigurations(V);
        shuffle (V.begin(), V.end(), std::default_random_engine(seed));

        //for each neighbour
        for (std::pair<BoardConfiguration,Move>& neighbour : V){

            //skip black (closed) neighbours
            if (closed.find(neighbour.first) != closed.cend()){
                continue;
            }

            //if neighbour is white (not enqued)
            // make it grey (open), assign its Gval,predecessor & direction, & add it to PQ
            if (open.find(neighbour.first) == open.cend()){
                open.insert(neighbour.first);
                Gval[neighbour.first] = Gval[current]+1;
                fromCache[neighbour.first] = {current,neighbour.second};
                PQ.push(neighbour.first);
                continue;
            }

            // if neighbour is grey (opened) check if it is not cheaper to reach it from the current board
            // if so then reassign Gval and predecessor of neighbour
            size_t alternativeGvalue = Gval[current] + 1;

                if (alternativeGvalue < Gval[neighbour.first]){
                    Gval[neighbour.first] = alternativeGvalue;
                    fromCache[neighbour.first] = {current,neighbour.second};
                }
        }
    }


    // restore path from the map ( board : {board,direction} )
    std::string result="";

    int_t lengthCounter = 0;

    while(!(fromCache[placeholder].first == placeholder)){
        result += stringify(fromCache[placeholder].second);
        lengthCounter++;
        placeholder = fromCache[placeholder].first;
    }
    result = (std::to_string(lengthCounter) + "\n" + std::string(result.rbegin(),result.rend()));

    return result;

}


int main(int argc, const char** argv)
{
    size_t N = 0;

    //if provided take N from command line argument
    if (argc == 2){
        N = atoi(*(argv+1));
    }//else get N from stdin
    else{
        std::cin>>N;
    }

    BoardConfiguration start(N);
    std::cin>>start;
    std::cout<<findPath(start);

    return 0;
}



