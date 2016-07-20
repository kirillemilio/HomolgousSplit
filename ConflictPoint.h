#include "BaseClasses.h"

class ConflictPoint
{
private:
    char _CharVal;
    int _Coor;
public:
    
    ConflictPoint(char charVal, int coor)
    {
        _CharVal = charVal;
        _Coor = coor;
    }
    ConflictPoint()
    {
        _CharVal = ' ';
        _Coor = -1;
    }
    static ConflictPoint Empty()
    {
        return ConflictPoint();
    }
    
    int GetCoor()
    {
        return _Coor;
    }
    
    int GetCharVal()
    {
        return _CharVal;
    }
    
    
};

typedef SetOf<ConflictPoint> SetOfConflictPoints;


