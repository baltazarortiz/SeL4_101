/* Include config variables. */
#include <autoconf.h>

/* Basics */
#include <stdio.h>
#include <assert.h>

#include <sel4/sel4.h>
#include <simple/simple.h>
#include <simple-default/simple-default.h>

#include <vka/object.h>

#include <allocman/allocman.h>
#include <allocman/bootstrap.h>
#include <allocman/vka.h>

#include <sel4platsupport/platsupport.h>
#include <sel4platsupport/serial.h>
#include <sel4platsupport/bootinfo.h>

#include <platsupport/chardev.h>
#include <platsupport/serial.h>

/* global environment variables */

/* seL4_BootInfo defined in bootinfo.h */
seL4_BootInfo *info;

/* simple_t defined in simple.h */
simple_t simple;

/* vka_t defined in vka.h */
vka_t vka;

/* allocman_t defined in allocman.h */
allocman_t *allocman;

serial_objects_t serial_objects;

/* static memory for the allocator to bootstrap with */
#define ALLOCATOR_STATIC_POOL_SIZE (BIT(seL4_PageBits) * 10)
UNUSED static char allocator_mem_pool[ALLOCATOR_STATIC_POOL_SIZE];

int main(void)
{
	// from dynamic-1
	int error;
	info = platsupport_get_bootinfo();
	assert(info != NULL);

	simple_default_init_bootinfo(&simple, info);

	allocman = bootstrap_use_current_simple(&simple, ALLOCATOR_STATIC_POOL_SIZE, &allocator_mem_pool);
	assert(allocman != NULL);

	allocman_make_vka(&vka, allocman);

	// get space
	seL4_CPtr cspace_cap;
	cspace_cap = simple_get_cnode(&simple);

	// get vspace
	seL4_CPtr vspace_cap;
	vspace_cap = simple_get_pd(&simple);

	// from seL4test
	// needed?
	//sel4platsupport_init_default_serial_caps(&vka, (vspace_t*)&vspace_cap, &simple, &serial_objects);
	platsupport_serial_setup_simple((vspace_t*)&vspace_cap, &simple, &vka);

	// Just a test to see if we can access the global variables in chardev.c
	void *test = &dev_defn;
	//ps_chardevice_t devserial;
	//uart_init(&dev_defn[0], io_ops, devserial);
	//int res = serial_ready(&devserial);
	//uart_getchar(&devserial);

	printf("Salut, Monde!\n");
	return 0;
}
