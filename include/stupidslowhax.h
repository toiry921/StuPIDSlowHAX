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

#pragma once

#include <3ds/types.h>

#define KSEMAPHORE_SIZE				(0x2C)
#define KSEMAPHORE_VTABLESIZE	(0x16 * sizeof(void*))
typedef struct KSemaphore {
	void **vtable;
	u32 refCount;
	u32 __a;
	u32 __b;
	u32 __c;
	u32 __ievent[3];
	u32 count;
	u32 maxCount;
	void *owner;
} __attribute__((packed)) KSemaphore;


bool supidslowhax_run(void);
void stupidslowhax_cleanup(void);
void stupidslowhax_cleanup(void);
void stupidslowhax_backdoor(void (*method)(void));