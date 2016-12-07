#include <3ds.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	osSetSpeedupEnable(true);

	Result res;

	printf("StuPID Slow Hax\n"); //Implementation of StuPID SlowHAX
	//POC - Takes about 2 weeks I think

	res=nsInit();
	printf("nsinit: %08X\n\n",(int)res);

	u64 titleid=0x0004001000024000; //PromoVideo, US O3DS
	//titleid=0x000400102002D500;
	u32 procid; //assume Nintendo is using s32
	u32 launch_flags=0; // -\_(シ)_/-
	//u32 target=0x100
	u32 target=0x7FFFFFFF-1; //(s32 max - 1)
	// PID cut-off. After exiting App then loading HomeBrewMenu, next app should launch with cut-off + 2 based on test with lower target PIDs/
	// In theory should launch 3ds_dsiwarehax_installer by Yellows8("https://github.com/yellows8/3ds_dsiwarehax_installer") with an s32 wrap at PID 0, so 3ds_dsiwarehax_installer has dull SVC access
	// Estimated to take 2 weeks to reach cut-off. Umm I don't have time to test :/
	// Doesn't seem to work as well on N3DS systems, Relaunching systemapps can lead to PID not advancing or Memory Crashes

	for(u32 i=0 ; i<0x80000000 ; i++){
				NS_LaunchTitle(titleid, launch_flags, &procid); //loop until PID is right next to s32 wrap-around
				printf("%08X\n",(int)procid);
				NS_TerminateProcessTID(titleid);
				if(procid>=target)break;
	}
	nsExit();
	printf("\n%08X  Current PID\n",(int)procid);
	printf(  "%08X  Target  PID\n",(int)procid+2);
	printf(  "Start to exit, then run 3ds_dsiwarehax_installer"); //need to launch 3ds_dsiware_installer with an XML to takeover some otherapp or PID will be download play (TOO LOW)

	while (aptMainLoop())
	{

		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;

		if (kDown & KEY_A){

		}

		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}