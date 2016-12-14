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


typedef u32(*backdoor_fn)(u32 arg0, u32 arg1);
u32 svc_7b(void* entry_fn, ...); // can pass up to 2 arguments to entry_fn(...)

Result svcCreateSemaphoreKAddr(Handle *semaphore, s32 initialCount, s32 maxCount, u32 **kaddr);
Result svcGetHandleInfo(s64* out, Handle handle, u32 type);