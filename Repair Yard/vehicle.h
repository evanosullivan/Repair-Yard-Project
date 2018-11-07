#ifndef VEHICLE_H_INCLUDED
#define VEHICLE_H_INCLUDED
#include <list>
#include <iostream>
#include <ctime>
#include <random>
#include "part.h"

using namespace std;

class vehicle
{
public:
    vehicle(){}
    vehicle(int i, int j){idNum=i, vType=j;}
    int get_idNum(){return idNum;}
    int get_vType(){return vType;}
    int get_numParts(){return numParts;}
    int get_numDamaged(){return numDamaged;}
    int *get_damagedParts(){return damaged_partArray;}
    bool get_partFound(){return partFound;}
    void set_idNum(int id){idNum=id;}
    void set_vType(int type){vType=type;}
    ///void set_partFound(bool status){partFound=status;}
    void set_numDamaged(int num){numDamaged=num;}

    bool hasPart(int partType)
    {
        pitr=partList.begin();
        for(int i=0; i<numParts; i++)
        {
            if((*pitr)->get_type()==partType && (*pitr)->get_condition()==0)
            {
                cout<<"\nPart #"<<(*pitr)->get_type()<<" found in scrapyard, using to fix vehicle #";
                (*pitr)->set_condition(true);
                return true;
            }
            pitr++;
        }
        return false;
    }

    void generateParts() ///Populates partList with appropriate damaged and working parts
    {
        default_random_engine gen(time(NULL));

        switch (vType)
        {
        case 1 :
            numParts=4;
            partArray[0]=1; partArray[1]=2; partArray[2]=3; partArray[3]=4;
            break;
        case 2 :
            numParts=5;
            partArray[0]=2; partArray[1]=4; partArray[2]=6; partArray[3]=8; partArray[4]=10;
            break;
        case 3 :
            numParts=6;
            partArray[0]=3; partArray[1]=5; partArray[2]=7; partArray[3]=9; partArray[4]=11; partArray[5]=13;
            break;
        case 4 :
            numParts=7;
            partArray[0]=10; partArray[1]=11; partArray[2]=12; partArray[3]=13; partArray[4]=14; partArray[5]=15;
            partArray[6]=16;
            break;
        case 5 :
            numParts=8;
            partArray[0]=1; partArray[1]=2; partArray[2]=3; partArray[3]=5; partArray[4]=7; partArray[5]=11;
            partArray[6]=13; partArray[7]=17;
            break;
        default :
            cout<<"Error, invalid vehicle type received.\n";
        }
///Now put parts in a list
        pitr=partList.begin();
        for(int i=0; i<numParts; i++)
        {
           pptr=new part(partArray[i]);
           partList.push_back(pptr);
           pitr++;
        }
///This section ensures 1 damaged part, ensures 1 working part, and sets distribution of damaged parts.
        uniform_int_distribution<int> pdist(0,(numParts-1)); ///subtract 1 to make it an index
        uniform_int_distribution<int> ctoss(0,99);
        p=pdist(gen);  ///disposable "seed read"

///Ensure damaged part
        p=pdist(gen);
        prevP=p; ///used later to ensure required damaged not overwritten
        pitr=partList.begin();
        for(int i=0; i<p; i++)
        {
            pitr++;
        }
        (*pitr)->set_condition(true);
///Distribute damaged parts
        p=ctoss(gen);
        for(int i=0; i<numParts; i++)
        {
            pitr=partList.begin();
            p=ctoss(gen);
            if(p>=63)
            {
                for(int j=0; j<i; j++)
                {
                    pitr++;
                }
                (*pitr)->set_condition(true);
            }
        }
///Ensure working part
        p=pdist(gen);
        while(p==prevP)
        {
            p=pdist(gen);
        }

        pitr=partList.begin();
        for(int i=0; i<p; i++)
        {
            pitr++;
        }
        (*pitr)->set_condition(false);

    }

    void display()
    {
        pitr=partList.begin();
        cout<<"\nVehicle type: "<<vType<<"\nID number: #"<<idNum<<"\nParts:\n";
        while(pitr != partList.end())
        {
            cout<<(*pitr)->get_type()<<" Status: "<<(*pitr)->get_condition()<<endl;
            pitr++;
        }
    }

    bool yardMaster()
    {
        p=0;
        pitr=partList.begin();
        for(int i=0; i<numParts; i++)
        {
            if((*pitr)->get_condition()==false)
            {
                p++;
            }
            else
            {
                numDamaged++;
                damaged_partArray[numDamaged-1]=(*pitr)->get_type();
            }
            pitr++;
        }

        if(((double(p)/double(numParts))>=0.5))
        {
            ///cout<<endl<<"REPAIRABLE!";
            return true;
        }
        else
        {
            ///cout<<endl<<"BROKEN!";
            return false;
        }

    }


private:
    int idNum;
    int vType;
    int numParts;
    int partArray[8];
    int damaged_partArray[8];
    int prevP;
    int p;
    int numDamaged=0;
    bool isRepairable=false;
    bool partFound=false;
    part *pptr;
    list <part*> partList;
    list <part*>::iterator pitr;
};
#endif // VEHICLE_H_INCLUDED
