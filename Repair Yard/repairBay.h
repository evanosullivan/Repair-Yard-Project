#ifndef REPAIRBAY_H_INCLUDED
#define REPAIRBAY_H_INCLUDED
#include <ctime>
#include <random>
#include "vehicle.h"

class repairBay
{
public:
    repairBay(){occupied=false;}
    repairBay(vehicle *pointer){vptr=pointer;}

    bool get_occupied(){return occupied;}
    int *get_damagedParts(){return vptr->get_damagedParts();}
    int get_numDamaged(){return vptr->get_numDamaged();}
    int get_repairTime(){return repairTime;}
    int get_damagedParts_original(int x){return damagedParts_original[x];}
    int get_numDamaged_original(){return numDamaged_original;}
    void fillBay(vehicle *pointer){vptr=pointer; occupied=true;}
    vehicle *get_vehicle(){return vptr;}
    void set_occupied(bool status){occupied=status;}

    int calculate_repairTime()
    {
        repairTime=0;
        vehicleType=vptr->get_vType();
        num_damagedParts=(vptr->get_numDamaged());
        damPtr=vptr->get_damagedParts();
        for(int i=0; i<num_damagedParts; i++)
        {
            repairTime += *damPtr;
            damagedParts_original[i]=*damPtr;
            numDamaged_original=num_damagedParts;
            damPtr++;
        }
        return repairTime;
    }
    bool runDay() ///returns true if vehicle should be removed and sent to scrapyard
    {
        default_random_engine gen(time(NULL));
        uniform_int_distribution<int> ctoss(0,99);
        p=ctoss(gen);
        if(get_numDamaged()==0)
        {
            repairTime=(repairTime-3);
        }
        else
        {
            days++;
            cout<<"\n\nWarning: vehicle "<<vptr->get_idNum()<<" still has broken parts after "<<days<<" days.";
            if(days==5)
            {
                days=0;
                return true;
            }
        }
        if(repairTime<=0)
        {
            ///occupied=false;
            cout<<"\n\nVehicle #"<<vptr->get_idNum()<<" has been repaired.\nRunning inspection...";
            p=ctoss(gen);
            if(p<89)
            {
                occupied=false;
                cout<<"Passed!";
            }
            else
            {
                cout<<"Vehicle has failed test, vehicle remains in repair bay.";
                repairTime=3;
            }
        }
        return false;
    }
private:
    vehicle *vptr;
    int *damPtr;
    int damagedParts_original[8];
    int numDamaged_original=0;
    int num_damagedParts=0;;
    int repairTime=0;
    int vehicleType=0;
    int days=0;
    int p=0;
    bool occupied=false;
};


#endif // REPAIRBAY_H_INCLUDED
