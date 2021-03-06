/***************************************
*Created by: Sudeepth Araveti
*Date      : 4/19/2016
*SudokuClass. Puzzle is in the form of
*this object.
*The puzzle is solved by iterations. 
*It is solved by basically narrowing possible
*values for every empty cells.
****************************************/
#include "SudokuClass.h"
#include <math.h>

Sudoku::Sudoku(int siz) : _size(siz)
{
    _sqSize = (int)sqrt((double)siz);
    _puzzle = (int* *)malloc(_size*sizeof(int *));
    for(int i=0; i<_size; ++i)
    {
	    _puzzle[i] = (int *)calloc((size_t)_size, sizeof(int));
    }
}

Sudoku::~Sudoku()
{
    for(int i=0; i<_size; ++i)
    {
    	free(_puzzle[i]);
    }
    free(_puzzle);
}

bool Sudoku::setRow(uint row, int *arr)
{
    if(!arr)
    {
	    return false;
    }
    memcpy(_puzzle[row], arr, _size*sizeof(int));
    return true;
}

bool Sudoku::getRow(uint row, int *arr) const
{
    if(!arr) return false;
    memcpy(arr, _puzzle[row], _size*sizeof(int));
    return true;
}

bool Sudoku::setVal(uint row, uint col, int val)
{
    if((row >= _size) || (col >= _size)) return false;
    _puzzle[row][col] = val;
    return true;
}

bool Sudoku::getVal(uint row, uint col, int &val) const
{
    if((row >= _size) || (col >= _size)) return false;
    val = _puzzle[row][col];
    return true;
}

void Sudoku::printPuzzle()
{
    for(uint i=0; i<_size; ++i)
    {
    	printRow(i);
    }
}

void Sudoku::printRow(uint row)
{
    for(uint i=0; i<_size; ++i)
    {
    	cout<<_puzzle[row][i]<<"\t";
    }
    cout<<endl;
}

//validate given puzzle
bool Sudoku::Valid(bool checkComplete)
{
    //check row, column and zone sanity
    for(int i=0; i<_size; ++i)
    {
        if(!IsValidRow(i, checkComplete) || 
            !IsValidColumn(i, checkComplete) ||
             !IsValidZone(i, checkComplete))
        {
            cout<<"invalid detected at index: "<<(i+1)<<endl;
            return false;
        }
    }
    return true;
}

bool Sudoku::IsValidRow(int row, bool checkComplete)
{
    int tmpArr[] = {0,0,0,0,0,0,0,0,0};
    for(int i=0; i<_size; ++i)
    {
        auto ind = _puzzle[row][i];
        if(ind == 0)
        {
            if(checkComplete)
            {
                return false;
            }
            continue;
        }
        else
        {
            --ind;
        }
        if(tmpArr[ind] != 0)    
        {
            cout<<"invalid row value: ("<<(row+1)<<" , "<<(i+1)<<")"<<endl;
            return false;
        }
        else
        {
            ++tmpArr[ind];
        }
    }
    return true;
}

bool Sudoku::IsValidColumn(int col, bool checkComplete)
{
    int tmpArr[] = {0,0,0,0,0,0,0,0,0};
    for(int i=0; i<_size; ++i)
    {
        auto ind = _puzzle[i][col];
        if(ind == 0)
        {
            if(checkComplete)
            {
                return false;
            }
            continue;
        }
        else
        {
            --ind;
        }        
        if(tmpArr[ind] != 0)
        {
            cout<<"invalid column value: ("<<(i+1)<<" , "<<(col+1)<<")"<<endl;
            return false;
        }
        else
        {
            ++tmpArr[ind];
        }
    }
    return true;
}

bool Sudoku::IsValidZone(int zone, bool checkComplete)
{
    int tmpArr[] = {0,0,0,0,0,0,0,0,0};
    int row = zone/_sqSize;
    int col = zone%_sqSize;
    auto row_st = row*_sqSize;
    auto row_end = row_st + _sqSize-1;
    auto col_st = col*_sqSize;
    auto col_end = col_st + _sqSize-1;
    for(auto i=row_st; i<=row_end; ++i)
    {
        for(auto j=col_st; j<=col_end; ++j)
        {
            auto ind = _puzzle[i][j];
            if(ind == 0)
            {
                if(checkComplete)
                {
                    return false;
                }
                continue;
            }
            else
            {
                --ind;
            }
            if(tmpArr[ind] != 0)
            {
                cout<<"invalid zone value: ("<<(i+1)<<" , "<<(j+1)<<")"<<endl;
                return false;                
            }
            else
            {
                ++tmpArr[ind];
            }
        }
    }
    return true;
}

//the actual puzzle is solved
void Sudoku::FillInitialPossibilities()
{
    for(auto i=0; i<_size; ++i)
    {
        for(auto j=0; j<_size; ++j)
        {
            if(_puzzle[i][j] != 0)
            {
            //its already solved. continue with other cells
                continue;
            }
            _possibValues[(i*_size)+j] = getPossibles(i, j);
        }
    }
}

void Sudoku::PrintPossibilities()
{
    PROBS::iterator it;
    Row::iterator rit;
    for(it = _possibValues.begin(); it != _possibValues.end(); ++it)
    {
        auto val = it->first;
        auto tmpRow = it->second;
        auto tmpind_i = val/_size + 1;
        auto tmpind_j = val%_size + 1;
        cout<<"possibilities for size: "<<tmpRow.size()<<" at ["<<tmpind_i<<" , "<<tmpind_j<<"] -- ";
        for(rit = tmpRow.begin(); rit != tmpRow.end(); ++rit)
        {
            cout<<*rit<<"\t";
        }
        cout<<endl;
    }
}

Row Sudoku::getPossibles(int i, int j)
{
    //              1,2,3,4,5,6,7,8,9
    int tmpArr[] = {0,0,0,0,0,0,0,0,0};
    Row tmpRow;
    if(_puzzle[i][j] != 0)
    {
        return tmpRow;
    }
    //check possibles in row
//    cout<<"values checkd for "<<i<<" , "<<j<<endl;
    for(auto c=0; c<_size; ++c)
    {
        if(_puzzle[i][c] == 0)
        {
            continue;
        }
        ++tmpArr[_puzzle[i][c]-1];
//        cout<<"DEBUG: puzzle["<<i<<" , "<<c<<"] -- "<<_puzzle[i][c]<<endl;
    }
    for(auto r=0; r<_size; ++r)
    {
        if(_puzzle[r][j] == 0)
        {
            continue;
        }
        ++tmpArr[_puzzle[r][j]-1];
//        cout<<"DEBUG: puzzle["<<r<<" , "<<j<<"] -- "<<_puzzle[r][j]<<endl;
    }
    auto row_st = (i/_sqSize)*_sqSize;
    auto row_end = row_st + _sqSize-1;
    auto col_st = (j/_sqSize)*_sqSize;
    auto col_end = col_st + _sqSize-1;
    for(auto r=row_st; r<=row_end; ++r)
    {
        for(auto c=col_st; c<=col_end; ++c)
        {
            if(_puzzle[r][c] == 0)
            {
                continue;
            }
            ++tmpArr[_puzzle[r][c]-1];
//            cout<<"DEBUG: puzzle["<<r<<" , "<<c<<"] -- "<<_puzzle[r][c]<<endl;
        }
    }
//    printTmpArr(tmpArr, 9, i, j);
    for(auto ii = 0; ii<_size; ++ii)
    {
        if(tmpArr[ii] == 0)
        {
            tmpRow.push_back(ii+1);
        }
    }
    return tmpRow;
}

void Sudoku::printTmpArr(int *arr, int size, int i, int j)
{
    cout<<"DEBUG: tmparr for ["<<i<<" , "<<j<<"] "<<endl;
    for(auto i=0; i<size; ++i)
    {
        cout<<arr[i]<<"\t";
    }
    cout<<endl;
}

void Sudoku::UpdatePossibles(int i, int j)
{
    auto ind = 0;
    Row::iterator tmpRowIt;
    bool found = false;
    cout<<"DEBUG: getting into updatePossibles method"<<endl;
    //update possible values in all columns of that row
    for(auto c=0; c<_size; ++c)
    {
        found = false;
        ind = (i*_size) + c;
        auto it = _possibValues.find(ind);
        if(it == _possibValues.end())
        {
        //There is no vector at this ind, so continue
            continue;
        }
        tmpRowIt = find(it->second, _puzzle[i][j]);
        if(tmpRowIt != it->second.end()) it->second.erase(tmpRowIt);
    }
    
    //update possible values in all rows of that column
    for(auto r=0; r<_size; ++r)
    {
        found = false;
        ind = (r*_size)+j;
        auto it = _possibValues.find(ind);
        if(it == _possibValues.end())
        {
        //There is no vector at this ind, so continue
            continue;
        }
        tmpRowIt = find(it->second, _puzzle[i][j]);
        if(tmpRowIt != it->second.end()) it->second.erase(tmpRowIt);
    }

    auto row_st = (i/_sqSize)*_sqSize;
    auto row_end = row_st + _sqSize -1;
    auto col_st = (j/_sqSize)*_sqSize;
    auto col_end = col_st + _sqSize -1;
    for(auto ii = row_st; ii <= row_end; ++ii)
    {
        for(auto jj = col_st; jj <= col_end; ++jj)
        {
            found = false;
            ind = (ii*_size) + jj;
            auto it = _possibValues.find(ind);
            if(it == _possibValues.end())
            {
                continue;
            }
            tmpRowIt = find(it->second, _puzzle[i][j]);
            if(tmpRowIt != it->second.end())  it->second.erase(tmpRowIt);
        }
    }
    cout<<"DEBUG: RETURN from updatePossibles method"<<endl;
}

void Sudoku::Solve()
{
    auto iter = 0;
    auto nextIter = true;
    while(nextIter)
    {
        nextIter = false;
        for(auto it = _possibValues.begin(); it != _possibValues.end();
                ++it)
        {
            if(it->second.size() == 1)
            {
                auto ind = it->first;
                auto row = ind/_size;
                auto col = ind%_size;
                cout<<"DEBUG: ["<<row<<" , "<<col<<"] -- "<<it->second.back()<<endl;
                setVal(row, col, it->second.back());
                nextIter = true;
                _possibValues.erase(it);
                UpdatePossibles(row, col);
            }
        }
        ++iter;
    }
    cout<<"total iterations processed: "<<iter<<endl;
    cout<<"solved puzzle:" <<endl;
}

Row::iterator Sudoku::find(Row &tmpRow, int val)
{
    Row::iterator retVal = tmpRow.end();
    for(auto itr = tmpRow.begin(); itr != tmpRow.end(); ++itr)
    {
        if(*itr == val)
        {
            retVal = itr;
        }
    }
    return retVal;
}
