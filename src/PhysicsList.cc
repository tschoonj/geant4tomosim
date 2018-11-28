#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "G4SystemOfUnits.hh"
//#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4LossTableManager.hh"
#include "G4PhysicsListHelper.hh"

//Physic lists (contained inside the Geant4 distribution)
#include "G4EmStandardPhysics.hh"

#include "G4EmLivermorePhysics.hh"

#include "G4Gamma.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4eMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4LivermorePhotoElectricModel.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4LivermoreRayleighModel.hh"
#include "G4LivermoreIonisationModel.hh"

//Read/write to a file
#include <iomanip>
#include <fstream>


PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
	//Creates the messenger class
  	PhysicsMessenger = new PhysicsListMessenger(this);

	//Sets the cutvalues
	G4double Cutvalue = 1*mm;
}

PhysicsList::~PhysicsList()
{
  	delete PhysicsMessenger;
}
void PhysicsList::ConstructParticle()
{
	//Define the particles you are going to be using
	G4Gamma::GammaDefinition();
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();
	G4GenericIon::GenericIonDefinition();
}

void PhysicsList::ConstructProcess()
{
	//Add the transportation of the particles
	AddTransportation();
	//Call the method that picks the physics used
	ConstructEM();
}
void PhysicsList::ConstructEM()
{
  	auto particleIterator = GetParticleIterator();
  	particleIterator->reset();

	//Cycles through he different particles	
  	while( (*particleIterator)() )
	{
		//Finds the physics processes for each particle 
  	  	G4ParticleDefinition* particle = particleIterator->value();
    		G4ProcessManager* pmanager = particle->GetProcessManager();
    		G4String particleName = particle->GetParticleName();
		
		//Adds the physics that were turned on via the commands for the gamma particle
		if(particleName == "gamma")
		{
			if(PhysicsPackageCmd == "LivermorePhysics")
			{
				if (PhotoElectricCmd == true)
				{
					G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
					thePhotoElectricEffect->SetEmModel(new G4LivermorePhotoElectricModel());
					pmanager->AddDiscreteProcess(thePhotoElectricEffect); 
					PhysicProcesses.push_back ("Photoelectric effect");
				}
				if (ComptonScatteringCmd == true)
				{
					G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
					theComptonScattering->SetEmModel(new G4LivermoreComptonModel());
					pmanager->AddDiscreteProcess(theComptonScattering); 
					PhysicProcesses.push_back ("Compton scattering");
				}
				if (RayleighScatteringCmd == true)
				{
					G4RayleighScattering* theRayleighScattering = new G4RayleighScattering();
					theRayleighScattering->SetEmModel(new G4LivermoreRayleighModel());
					pmanager->AddDiscreteProcess(theRayleighScattering); 
					PhysicProcesses.push_back ("Rayleigh scattering");
				}
			}
			else if(PhysicsPackageCmd == "StandardPhysics")
			{
				G4cout << G4endl << "STANDARD PHYSICS: ";

				if (PhotoElectricCmd == true)
				{
					G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
					pmanager->AddDiscreteProcess(thePhotoElectricEffect); 
					PhysicProcesses.push_back ("Photoelectric effect");
				}
				if (ComptonScatteringCmd == true)
				{
					G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
					pmanager->AddDiscreteProcess(theComptonScattering); 
					PhysicProcesses.push_back ("Compton scattering");
				}
				if (RayleighScatteringCmd == true)
				{
					G4RayleighScattering* theRayleighScattering = new G4RayleighScattering();
					pmanager->AddDiscreteProcess(theRayleighScattering); 
					PhysicProcesses.push_back ("Rayleigh scattering");
				}
			}
			else
			{
				//Outputs an error saying if the physics inputted is not a valid input and stops the simulation
				G4cout << "\n================================================================================"
	       	       		       << "\n            ERROR - INVALID PHYSICS INPUT: " << PhysicsPackageCmd << G4endl
		                       << "\n            Refer to the README for list of available Physics inputs "
	                               << "\n================================================================================" << G4endl;
				exit(-1);
			}
			if (FluorescenceCmd == true)
			{
				//Relaxtion processes after the photoelctric effect
				G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
  				de->SetFluo(true);
  				de->SetAuger(false);   
  				de->SetPIXE(false);  
  				G4LossTableManager::Instance()->SetAtomDeexcitation(de);
				PhysicProcesses.push_back ("Fluorescence");
			}
		}
	}
	G4cout << G4endl;
}

void PhysicsList::SetCuts(G4double Cutvalue)
	{SetCutValue(Cutvalue, "gamma");}

void PhysicsList::ReadOutInfo(G4String SaveFilePath)
{
	G4cout << "\nTHE FOLLOWING PHYSICS PROCESSES HAVE BEEN REGISTERED\n\n" 
	       << PhysicsPackageCmd << ":";

	for (int element = 0 ; element < PhysicProcesses.size() ; element++)
	{
		G4cout << "\n- " << PhysicProcesses[element];
	}
	G4cout << G4endl;	 

	//Creation of the writing to data file stream
	std::fstream outdata; 

	G4String SettingsName = "OutputLog.txt";

	//Open the file within the path set
	outdata.open(SaveFilePath+SettingsName, std::fstream::app); 
   	
	//Output error if can't open file
	if( !outdata ) 
	{ 	std::cerr << "\nError: " << SettingsName << " file could not be opened from PhysicsList.\n" << std::endl;
      		exit(1);
   	}

	outdata << "\nTHE FOLLOWING PHYSICS PROCESSES HAVE BEEN REGISTERED\n\n" 
	        << PhysicsPackageCmd << ":";

	for (int element = 0 ; element < PhysicProcesses.size() ; element++)
	{
		outdata << "\n- " << PhysicProcesses[element];
	}
	outdata << "\n";	

	outdata.close();      
}

