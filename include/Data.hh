#ifndef Data_h
#define Data_h 1

#include <vector>

//Used to access the function floor 
#include <math.h>      
#include "globals.hh"
#include "G4ThreeVector.hh"

class DataMessenger;

class Data
{
	public:
		Data();
		~Data();
		
		//Class methods
		//inline G4int Quotient(int dividend, int divisor ) {return dividend / divisor;}
		//inline G4int Remainder(int dividend, int divisor) {return dividend % divisor;}

		void SetUpData(int nDetectorsY, int nDetectorsZ, int nImage);

		void SaveFluorescence(double E);		
		inline void SaveHitData(G4int DetectorNumber){++HitDataArray[DetectorNumber];}
		void SaveFullMapping(double E);

		//Get Methods
		std::vector<int> GetHitData(){return HitDataArray;}
		std::vector<double> GetEnergyBins(){return EnergyBins;}
		std::vector<int> GetFluorescence(){return fluorescence;}
		std::vector<std::vector<std::vector<int> > > GetFullMapping(){return fullMappingFluore;}

		G4int GetNoBins(){return NoBins_Cmd;}
		G4int GetNumberOfBins(){return NoBins_Cmd;}
		G4double GetMaxEnergy(){return MaxE;}
		
		bool GetFullMapping_Option(){return fluoreFullMapping;}
		bool GetFluorescence_Option(){return fluoreFullField;}

		//Set Methods
		void SetNumberRows(G4int value){rows = value;}
		void SetNumberColumns(G4int value){columns = value;}

		void SetNoBins(G4int value){NoBins_Cmd = value;}
		void SetMaxEnergy(G4double value){MaxE = value;}

		void SetHalfDetectorDimensions(G4ThreeVector value){halfDetectorDimensions = value;}
		void SetParticlePosition(G4ThreeVector value){ParticlePosition = value;}

		void SetFullFieldFluorescence(bool value)  {fluoreFullField = value;}
		void SetFullMappingFluorescence(bool value){fluoreFullMapping = value;}

	private:	
	    DataMessenger* dataMessenger;

		//Data members
		std::vector<int> HitDataArray;
		std::vector<double> EnergyBins;
		std::vector<int> fluorescence;
		std::vector<std::vector<std::vector<int> > > fullMappingFluore;

		G4int rows;
		G4int columns;	

		G4int NoBins_Cmd;
		G4double MaxE;

		G4ThreeVector ParticlePosition;
		G4ThreeVector halfDetectorDimensions; 

		bool fluoreFullField;
		bool fluoreFullMapping;
};

#endif

		
