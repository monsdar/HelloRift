



#include <iostream>
#include <conio.h>

#include "OVR.h"

OVR::Ptr<OVR::DeviceManager>	pManager;
OVR::Ptr<OVR::HMDDevice>		pHMD;
OVR::Ptr<OVR::SensorDevice>		pSensor;
OVR::SensorFusion				FusionResult;
OVR::HMDInfo					Info;
bool							InfoLoaded;


void Init()
{
	OVR::System::Init();
	pManager = *OVR::DeviceManager::Create();
	pHMD = *pManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

	if (pHMD)
    {
        InfoLoaded = pHMD->GetDeviceInfo(&Info);
		pSensor = *pHMD->GetSensor();
	}
	else
	{
	   pSensor = *pManager->EnumerateDevices<OVR::SensorDevice>().CreateDevice();
	}

	if (pSensor)
	{
	   FusionResult.AttachToSensor(pSensor);
	}
}


void Clear()
{
	if (pSensor)
	{
		pSensor.Clear();
		pHMD.Clear();
		pManager.Clear();
	}

	OVR::System::Destroy();
}

void Output()
{
	std::cout << "----- Oculus Console -----" << std::endl;

	if (pHMD)
	{
		std::cout << " [x] HMD Found" << std::endl;
	}
	else
	{
		std::cout << " [ ] HMD Not Found" << std::endl;
	}

	if (pSensor)
	{
		std::cout << " [x] Sensor Found" << std::endl;
	}
	else
	{
		std::cout << " [ ] Sensor Not Found" << std::endl;
	}

	std::cout << "--------------------------" << std::endl;

	if (InfoLoaded)
    {
		std::cout << " DisplayDeviceName: " << Info.DisplayDeviceName << std::endl;
		std::cout << " ProductName: " << Info.ProductName << std::endl;
		std::cout << " Manufacturer: " << Info.Manufacturer << std::endl;
		std::cout << " Version: " << Info.Version << std::endl;
		std::cout << " HResolution: " << Info.HResolution<< std::endl;
		std::cout << " VResolution: " << Info.VResolution<< std::endl;
		std::cout << " HScreenSize: " << Info.HScreenSize<< std::endl;
		std::cout << " VScreenSize: " << Info.VScreenSize<< std::endl;
		std::cout << " VScreenCenter: " << Info.VScreenCenter<< std::endl;
		std::cout << " EyeToScreenDistance: " << Info.EyeToScreenDistance << std::endl;
		std::cout << " LensSeparationDistance: " << Info.LensSeparationDistance << std::endl;
		std::cout << " InterpupillaryDistance: " << Info.InterpupillaryDistance << std::endl;
		std::cout << " DistortionK[0]: " << Info.DistortionK[0] << std::endl;
		std::cout << " DistortionK[1]: " << Info.DistortionK[1] << std::endl;
		std::cout << " DistortionK[2]: " << Info.DistortionK[2] << std::endl;
		std::cout << "--------------------------" << std::endl;
    }

	std::cout << std::endl << " Press ENTER to continue" << std::endl;

	std::cin.get();

	while(pSensor)
	{
		OVR::Quatf quaternion = FusionResult.GetOrientation();

		float yaw, pitch, roll;
		quaternion.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &pitch, &roll);

		std::cout << " Yaw: " << OVR::RadToDegree(yaw) << 
			", Pitch: " << OVR::RadToDegree(pitch) << 
			", Roll: " << OVR::RadToDegree(roll) << std::endl;

		Sleep(50);

		if (_kbhit()) exit(0);
	}
}


int main()
{
	std::cout << "Initing the Rift..." << std::endl;
	Init();

	Output();

	std::cout << "Destructing the Rift..." << std::endl;
	Clear();

	return 0;
}

