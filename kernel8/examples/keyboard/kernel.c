//
// main.c
//
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/util.h>

#include <lfb.h>


static const char FromSample[] = "sample";

static void KeyPressedHandler (const char *pString)
{
	ScreenDeviceWrite (USPiEnvGetScreen (), pString, strlen (pString));
}

void say(char* str)
{
	lfb_print(90, 90, str);
}

void kernel_main()
{
	lfb_init();
	lfb_print(80, 80, "Hello World!");
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
	if (!USPiInitialize ()) // seems to cause crash
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
		ScreenDeviceRotor (USPiEnvGetScreen (), 0, nCount);
	}

finis:
	while(1);
}
