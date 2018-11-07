#ifndef PART_H_INCLUDED
#define PART_H_INCLUDED

class part
{
public:
    part(){isDamaged=false;}
    part(int i){type=i, isDamaged=false;}
    int get_type(){return type;}
    bool get_condition(){return isDamaged;}
    bool get_replacementFound(){return replacementFound;}
    void set_type(int typeNum){type=typeNum;}
    void set_condition(bool condition){isDamaged=condition;}
    void set_replacementFound(bool status){replacementFound=status;}
private:
    int type;
    bool isDamaged=false;
    bool replacementFound=false;
};


#endif // PART_H_INCLUDED
