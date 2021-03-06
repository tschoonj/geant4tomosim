/*
Class to track the progress of a simulation. It does this by
knowing how many events there are, the current event its on,
how many runs there will be and what the current run is.

Author: Oliver Jack Wenman
*/

#ifndef ProgressTracker_h
#define ProgressTracker_h 1

#include <iostream>
#include "G4Timer.hh"
#include "globals.hh"

class ProgressTracker
{
    public:
        ProgressTracker();
        ~ProgressTracker();

        G4Timer Timer;  
    
        //Functions to do with the progress of the simulation
        void Setup(unsigned long long int nEvents, int nRuns){totalparticles = nEvents; totalprojections = nRuns;}
	    void PrintProgress(int CurrentEvent, int CurrentImage);
        void ResetEvents(){projectioncheck = totalprogressCheck = -1;}
        void SetSavingTime(double time){savingtime = time;}
      
        bool print;
        bool singleline;
        bool graphicsOn;
        int  dprogress;     
        double rotationangle;    
      
    private:
        void ProgressBar(int Percent);
	    void PrintTime(double time);
	    double EstimatedTime(int Percent, int CurrentImage);
        
		unsigned long long int totalparticles;
		int totalprojections;
		int projectioncheck;
		double savingtime;

		int totalprogress;
		int totalprogressCheck;

		double remainingTime;
};

#endif

