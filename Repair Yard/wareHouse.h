#ifndef WAREHOUSE_H_INCLUDED
#define WAREHOUSE_H_INCLUDED
#include <ctime>
#include <random>
#include <list>
#include <iostream>
#include <iomanip>

using namespace std;

class wareHouse
{
public:
    wareHouse(){};
    void fillHouse()
    {
        default_random_engine gen(time(NULL));
        uniform_int_distribution<int> ctoss(0,99);

        p=ctoss(gen);
        for(int i=1; i<18; i++) ///populate warehouse
        {
            p=ctoss(gen);
            if(p<74) ///75% chance of having the part
            {
                pptr=new int(i);
                parts.push_back(pptr);
                p=ctoss(gen);
                if(p<49) ///50% chance of having multiple
                {
                    for(int j=0; j<4; j++) ///Add 4 more parts if multiple parts
                    {
                        pptr=new int(i);
                        parts.push_back(pptr);
                    }

                }
            }
        }

    }
    bool checkForPart(int part) ///returns true if part is found in warehouse
    {
        pitr=parts.begin();
        while(pitr != parts.end())
        {
            if(part==**pitr)
            {
                cout<<"\nPart #"<<**pitr<<" found in warehouse, using to fix vehicle #";
                pitr=parts.erase(pitr);
                return true;
            }
            pitr++;
        }
    }
    void display()
    {
        cout<<"\n\n================================"
            <<"\n     Warehouse parts list       "
            <<"\n================================\n";
        pitr=parts.begin();
        x=0;
        while(pitr != parts.end())
        {
            if(x==11)
            {
                cout<<endl;
                x=0;
            }
            cout<<**pitr<<" ";
            pitr++;
            x++;
        }
        cout<<"\n================================";
    }
    bool orderPart(int id)
    {
        cout<<"\n\nAttempting to order part #"<<id<<"...";
        default_random_engine gen(time(NULL));
        uniform_int_distribution<int> ctoss(0,99);
        p=ctoss(gen);
        p=ctoss(gen);
        if(p<9)
        {
            cout<<"\nPart #"<<id<<" has arrived at warehouse, using to repair vehicle #";
            return true;
        }
        else
        {
            cout<<"\nPart #"<<id<<" has not arrived.\n";
            return false;
        }

    }



private:
    int p=0;
    int x=0;
    int *pptr;
    list <int*> parts;
    list <int*>::iterator pitr;
};


#endif // WAREHOUSE_H_INCLUDED
