#include <iostream>
#include <list>
#include <random>
#include <ctime>
#include "vehicle.h"
#include "repairBay.h"
#include "wareHouse.h"

using namespace std;



int main()
{
    int newCars=0;
    int day=1;
    int totalVehicles=0;
    int goodCars=0;
    int badCars=0;
    int goodCars_today=0;
    int badCars_today=0;
    int *damPtr=NULL;
    bool repairable=false;
    string nextDay=" ";
    vehicle *vptr;
    wareHouse house;
    list <vehicle*> arrivals;
    list <vehicle*> scrapYard;
    list <vehicle*>::iterator aitr;
    list <vehicle*>::iterator sitr;
    repairBay bays[3];
    default_random_engine gen(time(NULL));
    normal_distribution<float> cdist(3,1);

    house.fillHouse(); ///populate warehouse initially

    do{
    newCars=cdist(gen); ///seed read
    newCars=cdist(gen);
    goodCars_today=0;
    badCars_today=0;

    if(newCars != 0)
    {
    for(int i=0; i<int(newCars); i++)
    {
        vptr=new vehicle(totalVehicles,((totalVehicles%5)+1));
        vptr->generateParts();
        repairable=vptr->yardMaster();
        if(repairable==true)
        {
            arrivals.push_back(vptr);
            goodCars++;
            goodCars_today++;
            ///cout<<"\nREPAIRABLE";
        }
        else if(repairable==false)
        {
            scrapYard.push_back(vptr);
            badCars++;
            badCars_today++;
            ///cout<<"\nNOT REPAIRABLE";
        }
        else{cout<<"\nError";}

        totalVehicles++;
    }
    sitr=scrapYard.begin();
    aitr=arrivals.begin();
    if(!arrivals.empty())
    {
    cout<<"\nRepairable vehicles in que:\n";
    for(int i=0; i<(goodCars); i++)
    {
        (*aitr)->display();
        aitr++;
    }
    }
    if(!scrapYard.empty())
    {
    cout<<"\nScrap vehicles:\n";
    for(int i=0; i<(badCars); i++)
    {
        (*sitr)->display();
        sitr++;
    }
    }
    cout<<"\n"<<newCars<<" new car(s) today, "<<goodCars_today<< " repairable vehicles, "
    <<badCars_today<<" vehicles sent to scrapyard, "<<totalVehicles<<" total vehicles.";
    }
    else{cout<<"\nNo new cars today.";}

    ///Begin managing of repair bays
    aitr=arrivals.begin();
    if(goodCars != 0 && bays[0].get_occupied() == false)
    {
        bays[0].fillBay(*aitr);
        bays[0].calculate_repairTime();
        cout<<"\nVehicle #"<<(*aitr)->get_idNum()<<" sent to repair bay 0."; // @suppress("Method cannot be resolved")
        aitr=arrivals.erase(aitr);
        goodCars--;
    }
    if(goodCars != 0 && bays[1].get_occupied() == false)
    {
        bays[1].fillBay(*aitr);
        bays[1].calculate_repairTime();
        cout<<"\nVehicle #"<<(*aitr)->get_idNum()<<" sent to repair bay 1."; // @suppress("Method cannot be resolved")
        aitr=arrivals.erase(aitr);
        goodCars--;
    }
    if(goodCars != 0 && bays[2].get_occupied() == false)
    {
        bays[2].fillBay(*aitr);
        bays[2].calculate_repairTime();
        cout<<"\nVehicle #"<<(*aitr)->get_idNum()<<" sent to repair bay 2."; // @suppress("Method cannot be resolved")
        aitr=arrivals.erase(aitr);
        goodCars--;
    }
    ///display repair bays status
    cout<<"\n\n=================================="
        <<"\nBeginning of day repair bay status"
        <<"\n==================================";
    for(int i=0; i<3; i++)
    {
        if(bays[i].get_occupied()==true)
        {
            cout<<"\nRepair bay "<<i<<" status:\nOccupied: YES"<<"\nVehicle contained: #";
            vptr=bays[i].get_vehicle();
            cout<<vptr->get_idNum();
            cout<<"\nDamaged parts: ";
            for(int k=0; k<bays[i].get_numDamaged_original(); k++)
            {
                cout<<bays[i].get_damagedParts_original(k)<<" ";
            }
            cout<<"\nTime remaining: "<<bays[i].get_repairTime();
            cout<<"\n==================================";
        }
    }
    ///Show warehouse stock
    house.display();
    ///Check for parts in scrap yard
    cout<<"\n\nChecking for parts in scrapyard...";
    for(int x=0; x<3; x++)
    {
        if(bays[x].get_occupied()==true)
        {
            vptr=bays[x].get_vehicle();
            damPtr=(vptr)->get_damagedParts();
            for(int i=0; i<bays[x].get_numDamaged_original(); i++)
            {
                sitr=scrapYard.begin();
                while(sitr != scrapYard.end())
                {
                    ///cout<<"\nVehicle in bay "<<x<<" still has "<<(vptr)->get_numDamaged()<<" broken parts";
                    if((vptr)->get_numDamaged()>0 && (*damPtr != 0))
                    {
                    if((*sitr)->hasPart(*damPtr))
                    {
                        cout<<vptr->get_idNum();
                        (vptr)->set_numDamaged((vptr)->get_numDamaged()-1);
                        i--; ///decrement counter if part is found
                        *damPtr=0;
                    }
                    else
                    {
                        cout<<"\nNo match found for part #"<<*damPtr<<" for vehicle #"<<(vptr)->get_idNum()<<" in vehicle #"<<(*sitr)->get_idNum()<<" in scrapyard";
                    }
                        }
                        sitr++;
                }
                damPtr++;
            }
        }
    }
    ///otherwise check warehouse
    cout<<"\n\nChecking for parts in warehouse...";
    for(int x=0; x<3; x++)
    {
        if(bays[x].get_occupied()==true)
        {
            vptr=bays[x].get_vehicle();
            damPtr=(vptr)->get_damagedParts();
            for(int i=0; i<bays[x].get_numDamaged_original(); i++)
            {
                    ///cout<<"\nVehicle in bay "<<x<<" still has "<<(vptr)->get_numDamaged()<<" broken parts";
                    if((vptr)->get_numDamaged()>0 && (*damPtr != 0))
                    {
                    if(house.checkForPart(*damPtr))
                    {
                        cout<<vptr->get_idNum();
                        (vptr)->set_numDamaged((vptr)->get_numDamaged()-1);
                        ///i--; ///decrement counter if part is found
                        *damPtr=0;
                    }
                    else if(house.orderPart(*damPtr))
                    {
                        cout<<vptr->get_idNum();
                        (vptr)->set_numDamaged((vptr)->get_numDamaged()-1);
                        ///i--; ///decrement counter if part is found
                        *damPtr=0;
                    }
                    else
                    {
                        cout<<"\nNo match found for part #"<<*damPtr<<" for vehicle #"<<(vptr)->get_idNum()<<" in warehouse";
                    }
                    }
                damPtr++;
            }
        }
    }

    ///simulate a day, check to see if vehicle should be removed
    for(int i=0; i<3; i++)
    {
        if (bays[i].get_occupied()==true)
        {
            if(bays[i].runDay()==true)
            {
                vptr=bays[i].get_vehicle();
                scrapYard.push_back(vptr);
                bays[i].set_occupied(false);
            }
        }
    }
    cout<<"\n\nEnd of day "<<day<<", Press any character key then ENTER to start next day.\nType exit then ENTER to quit.\n";

    ///repairBay testBay(vptr);
    ///cout<<"\nRepair time for last car: "<<testBay.calculate_repairTime();
    cin>>nextDay;
    day++;


        }
        while(nextDay != "exit");
            //house.fillHouse();
            //house.display();
            //cout<<endl<<house.checkForPart(5);
            //house.display();

    return 0;
}
