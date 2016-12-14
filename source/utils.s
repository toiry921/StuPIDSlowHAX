@Copyright (C) 2016 Toiry921
@
@    This program is free software: you can redistribute it and/or modify
@    it under the terms of the GNU General Public License as published by
@    the Free Software Foundation, either version 3 of the License, or
@    (at your option) any later version.
@
@    This program is distributed in the hope that it will be useful,
@    but WITHOUT ANY WARRANTY; without even the implied warranty of
@    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@    GNU General Public License for more details.
@
@    You should have received a copy of the GNU General Public License
@    along with this program.  If not, see <http://www.gnu.org/licenses/>.

.arm
.section .text


@ Based on Memchunkhax2
@ Credits to TuxSH
@ Maki is best girl
.global svcCreateSemaphoreKAddr
.type	svcCreateSemaphoreKAddr, %function
svcCreateSemaphoreKAddr:
		str r0, [sp, #-4]!
		str r3, [sp, #-4]!
		svc 0x15
		ldr r3, [sp], #4
		sub r2, r2, #4
		str r2, [r3]
		ldr r3, [sp], #4
		str r1, [r3]
		bx lr

.global svcGetHandleInfo
.type svcGetHandleInfo, %function
svcGetHandleInfo:
		str r0, [sp, #-0x4]!
		svc 0x29
		ldr r3, [sp], #4
		str r1, [r3]
		str r2, [r3, #4]
		bx lr


@ Debug/Dev uses
.global svc_7b
.type 	svc_7b, %function
svc_7b:
	push {r0, r1, r2}
	mov r3, sp
	add r0, pc, #12
	svc 0x7b
	add sp, sp, #8
	ldr r0, [sp], #4
	bx lr
	cpsid aif
	ldr r2, [r3], #4
	ldmfd r3!, {r0, r1}
	push {r3, lr}
	blx r2
	pop {r3, lr}
	str r0, [r3, #-4]!
	mov r0, #0
	bx lr