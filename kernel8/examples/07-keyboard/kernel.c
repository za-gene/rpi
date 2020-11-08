//
// main.c
//
//#include <uspienv.h>
//#include <uspi.h>
#include <uspios.h>
//#include <uspienv/util.h>

//
#include <uspi/uspilibrary.h>
#include <uspi/usbfunction.h>
//#include <uspi/string.h>
#include <uspi/util.h>
#include <uspi/assert.h>


#include <lfb.h>


static const char FromSample[] = "sample";

void say(char* str)
{
	fbputs(str);
}
static void KeyPressedHandler (const char *pString)
{
	//ScreenDeviceWrite (USPiEnvGetScreen (), pString, strlen (pString));
	say("KeyPressedHandler:detected");
}

TUSPiLibrary *s_pLibrary=0;
static TDeviceNameService *s_pThis = 0;

void myDeviceNameService (TDeviceNameService *pThis)
{
	//assert (pThis != 0);
	pThis->m_pList = 0;
	//assert (s_pThis == 0);
	s_pThis = pThis;
}

int myUSPiInitialize (void)
{
	//LogWrite (FromUSPi, LOG_DEBUG, "Initializing " USPI_NAME " " USPI_VERSION_STRING);

	//assert (s_pLibrary == 0);
	s_pLibrary = (TUSPiLibrary *) malloc (sizeof (TUSPiLibrary));
	//assert (s_pLibrary != 0);

	say("1");
	myDeviceNameService (&s_pLibrary->NameService);
	say("1a"); 
	DWHCIDevice (&s_pLibrary->DWHCI);
	s_pLibrary->pEth0 = 0;
	s_pLibrary->pEth10 = 0;

	say("2");
	if (!DWHCIDeviceInitialize (&s_pLibrary->DWHCI)) // problems
	{
		//LogWrite (FromUSPi, LOG_ERROR, "Cannot initialize USB host controller interface");

		_DWHCIDevice (&s_pLibrary->DWHCI);
		_DeviceNameService (&s_pLibrary->NameService);
		free (s_pLibrary);
		s_pLibrary = 0;

		return 0;
	}

	say("3");

	s_pLibrary->pUKBD1 = (TUSBKeyboardDevice *) DeviceNameServiceGetDevice (DeviceNameServiceGet (), "ukbd1", FALSE);

	s_pLibrary->pUMouse1 = (TUSBMouseDevice *) DeviceNameServiceGetDevice (DeviceNameServiceGet (), "umouse1", FALSE);

	s_pLibrary->pMIDI1 = (TUSBMIDIDevice *) DeviceNameServiceGetDevice (DeviceNameServiceGet (), "umidi1", FALSE);

	for (unsigned i = 0; i < MAX_DEVICES; i++)
	{
		TString DeviceName;
		String  (&DeviceName);
		StringFormat (&DeviceName, "umsd%u", i+1);

		s_pLibrary->pUMSD[i] = (TUSBBulkOnlyMassStorageDevice *)
			DeviceNameServiceGetDevice (DeviceNameServiceGet (), StringGet (&DeviceName), TRUE);

		_String  (&DeviceName);
	}

	s_pLibrary->pEth0 = (TSMSC951xDevice *) DeviceNameServiceGetDevice (DeviceNameServiceGet (), "eth0", FALSE);

	s_pLibrary->pEth10 = (TLAN7800Device *) DeviceNameServiceGetDevice (DeviceNameServiceGet (), "eth10", FALSE);

	for (unsigned i = 0; i < MAX_DEVICES; i++)
	{
		TString DeviceName;
		String  (&DeviceName);
		StringFormat (&DeviceName, "upad%u", i+1);

		s_pLibrary->pUPAD[i] = (TUSBGamePadDevice *)
			DeviceNameServiceGetDevice (DeviceNameServiceGet (), StringGet (&DeviceName), FALSE);

		_String  (&DeviceName);
	}

	//LogWrite (FromUSPi, LOG_DEBUG, USPI_NAME " successfully initialized");

	return 1;
}

void USPiEnvClose() {}
int USPiEnvInitialize() { return 1; }

void kernel_main()
{
	lfb_init();
	say("keyobard test started");
	//LogWrite("main", LOG_DEBUG, "Testing to see if LogWrite actually works"); // I think this causes a crash
	say("Let's try");
	//goto finis;

	goto skip1;
	if (!USPiEnvInitialize()) { // seems to cause crash
		say("USPiEnvInitialize:failed");
		goto finis;
	}
	say("USPiEnvInitialize:OK");
	
skip1:
	say("USPiInitialize:try");
	if (!myUSPiInitialize ()) // seems to cause crash
	{
		//LogWrite (FromSample, LOG_ERROR, "Cannot initialize USPi");
		say("Can't initialise USPi");
		USPiEnvClose ();
		goto finis;
	}
	say("USPiInitialize:OK ");
	
	if (!USPiKeyboardAvailable ())
	{
		say("Keyboard not found");
		//LogWrite (FromSample, LOG_ERROR, "Keyboard not found");
		USPiEnvClose ();
		goto finis;
	}

	say("Keyboard setup");
	USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);

	LogWrite (FromSample, LOG_NOTICE, "Just type something!");

	// just wait and turn the rotor
	for (unsigned nCount = 0; 1; nCount++)
	{
		USPiKeyboardUpdateLEDs ();
		//ScreenDeviceRotor (USPiEnvGetScreen (), 0, nCount);
	}

finis:
	while(1);
}
