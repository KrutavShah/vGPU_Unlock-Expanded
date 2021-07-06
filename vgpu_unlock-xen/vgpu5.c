#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <syslog.h>

#define QUERY_GPU	0xC020462A

#define OP_PCI          0x20801801
#define OP_ENABLE_VGPU  0x800289
#define OP_LICENSE      0xA0820104

typedef struct vgpu_request {
	uint32_t unk1;
	uint32_t unk2;
	uint32_t op;
	uint32_t padding;
	void*    result;
	uint32_t unk4;
	uint32_t status;
} vgpu_request;

typedef struct subdevice_get_bus_id {
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t unknown;
	uint16_t subsystem_id;
} subdevice_get_bus_id;

typedef struct enable_vgpu {
	uint64_t capability;
} enable_vgpu;

extern char *program_invocation_name;
static int (*ioctl_real)(int fd, unsigned long request, void *argp) = NULL;

int ioctl(int fd, unsigned long request, void *argp) {

	ioctl_real = dlsym(RTLD_NEXT, "ioctl");

	if ( request != QUERY_GPU ) return ioctl_real(fd, request, argp);

	// Begin the instrumentation

	int ioctl_response           = ioctl_real(fd, request, argp);
	vgpu_request* ioctl_request  = (struct vgpu_request*)argp;

	// syslog(LOG_INFO, "ioctl_request-> op is 0x%x\n", ioctl_request->op);

	subdevice_get_bus_id* subdevice_result;
	enable_vgpu*          vgpu_capability;
	switch (ioctl_request->op) {
		case OP_PCI:
			subdevice_result = (struct subdevice_get_bus_id*) ioctl_request->result;
			// Stolen from https://github.com/DualCoder/vgpu_unlock/blob/master/vgpu_unlock_hooks.c
			switch (subdevice_result->device_id) {
				case 0x1340 ... 0x13bd:
				case 0x174d ... 0x179c:
					subdevice_result->device_id = 0x13bd; /* Tesla M10 */
					break;

				/* Maxwell 2.0 */
				case 0x13c0 ... 0x1436:
				case 0x1617 ... 0x1667: /* GM204 */
				case 0x17c2 ... 0x17fd: /* GM200 */
					subdevice_result->device_id = 0x13f2; /* Tesla M60 */
					break;
		
				/* Pascal */
				case 0x15f0 ... 0x15f1: /* GP100GL */
				case 0x1b00 ... 0x1d56:
				case 0x1725 ... 0x172f: /* GP100 */
					subdevice_result->device_id = 0x1b38; /* Tesla P40 */
					break;

				/* Volta GV100 */
				case 0x1d81: /* Titan V 16GB */
				case 0x1dba: /* Quadro GV100 32GB */
					subdevice_result->device_id = 0x1db6; /* Tesla V100 32GB PCIE */
					break;

				/* Turing */
				case 0x1e02 ... 0x1ff9:
				case 0x2182 ... 0x21d1: /* TU116 */
					subdevice_result->device_id = 0x1e30; /* Quadro RTX 6000 */
					break;
	
				/* Ampere */
				case 0x2200 ... 0x2600: 
					subdevice_result->device_id = 0x2230; /* RTX A6000 */
					break;
			}

			break;

		case OP_ENABLE_VGPU:
			vgpu_capability = (struct enable_vgpu*) ioctl_request->result;
			vgpu_capability->capability = 0x3;
			break;

		case OP_LICENSE:
			ioctl_request->status = 0x0;
			break;
	}

	return ioctl_response;

}
