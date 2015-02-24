/*
 *  hoa.process~ header
 *
 *	This header file provides an interface for querying and seting the state of a parent hoa.process~ object.
 *	All communication with a host object should be done using this interface - the hoa.process~.c file has some more information.
 *
 *  based on dynamicdsp suite. Copyright 2010 Alex Harker. All rights reserved.
 *
 */


#ifndef __HOA_PROCESS_SUITE_DEF__
#define __HOA_PROCESS_SUITE_DEF__

#include "ext.h"

#define HoaProcessorIsValid(x) (x && ((((t_symbol *) ob_sym (x)) == gensym("hoa.process~"))))

////////// Structure for passing arguments to patchers when targeting particular patches /////////

typedef struct _args_struct
{
	t_symbol *msg;
	short argc;
	t_atom *argv;
	
	long index;
	
} t_args_struct;

typedef struct _attr_struct
{
	t_symbol *msg;
	short argc;
	t_atom *argv;
	
	long index;
	
} t_attr_struct;

//////////////////////////////////////////////// Object Queries ////////////////////////////////////////////////

// These routines *must* be called these routines at loadbang - they are bogus at any other time

__inline void *Get_HoaProcessor_Object() 
{
	void *HoaProcessor_Object = (void *) gensym("___HoaProcessor~___")->s_thing;

	if (HoaProcessorIsValid(HoaProcessor_Object))
		return HoaProcessor_Object;
	else
		return 0;
}


__inline long Get_HoaProcessor_Patch_Index(void *HoaProcessor_Object) 
{
	if (HoaProcessorIsValid(HoaProcessor_Object))
		return (long) gensym("___HoaProcessorPatchIndex___")->s_thing;
	else
		return 0;
}


//////////////////////////////////////////////// Signal IO Queries ////////////////////////////////////////////////


__inline long HoaProcessor_Get_Declared_Sigins(void *HoaProcessor_Object)
{
	if (HoaProcessorIsValid(HoaProcessor_Object))
		return (long) mess0((t_object *)HoaProcessor_Object, gensym("get_declared_sigins"));
	else
		return 0;
}


__inline long HoaProcessor_Get_Declared_Sigouts(void *HoaProcessor_Object)
{
	if (HoaProcessorIsValid(HoaProcessor_Object))
		return (long) mess0((t_object *)HoaProcessor_Object, gensym ("get_declared_sigouts"));
	else
		return 0;
}


__inline void **HoaProcessor_Get_Sigins(void *HoaProcessor_Object)
{
	if (HoaProcessorIsValid(HoaProcessor_Object))
		return (void**)mess0((t_object *)HoaProcessor_Object, gensym ("get_sigins"));
	else	
		return 0;
}


__inline void ***HoaProcessor_Get_Outptrs_Ptr(void *HoaProcessor_Object, long index)
{
	if (HoaProcessorIsValid(HoaProcessor_Object) && index > 0)
		return (void***)mess1((t_object *)HoaProcessor_Object, gensym("get_outptrs_ptr"), (void *)index);
	else
		return 0;
}

__inline long HoaProcessor_Get_IO_Index(void *HoaProcessor_Object, long patchIndex, t_object* io)
{
	if (HoaProcessorIsValid(HoaProcessor_Object) && patchIndex > 0)
		return (long) mess2((t_object *)HoaProcessor_Object, gensym("get_io_index"), patchIndex, io);
	else
		return 0;
}

//////////////////////////////////////////////// State Queries ////////////////////////////////////////////////


__inline long HoaProcessor_Get_Patch_On (void *HoaProcessor_Object, long index)
{
	if (HoaProcessorIsValid(HoaProcessor_Object) && index > 0)
		return (long) mess1((t_object *)HoaProcessor_Object, gensym("get_patch_on"), (void *)index);
	
	return 0;
}

__inline void HoaProcessor_Set_Patch_On (void *HoaProcessor_Object, long index, long state)
{
	if (HoaProcessorIsValid(HoaProcessor_Object) && index > 0)
		mess2((t_object *)HoaProcessor_Object, gensym("set_patch_on"), (void *)index, (void *)state);
}

__inline long HoaProcessor_Get_Ambisonic_Order (void *HoaProcessor_Object)
{
	if (HoaProcessorIsValid(HoaProcessor_Object))
		return (long) mess0((t_object *)HoaProcessor_Object, gensym("get_ambisonic_order"));
	
	return 0;
}

__inline t_symbol *HoaProcessor_Get_Mode (void *HoaProcessor_Object)
{
	if (HoaProcessorIsValid(HoaProcessor_Object))
		return (t_symbol *) mess0((t_object *)HoaProcessor_Object, gensym("get_mode"));
	
	return 0;
}

__inline long HoaProcessor_Get_Number_Of_Instances (void *HoaProcessor_Object)
{
	if (HoaProcessorIsValid(HoaProcessor_Object))
		return (long) mess0((t_object *)HoaProcessor_Object, gensym("get_number_of_instance"));
	
	return 0;
}

__inline long HoaProcessor_Is_2D (void *HoaProcessor_Object)
{
	if (HoaProcessorIsValid(HoaProcessor_Object))
		return (long) mess0((t_object *)HoaProcessor_Object, gensym("is_2D"));
	
	return 0;
}

__inline long HoaProcessor_Get_PatcherArgs (void *HoaProcessor_Object, long index, long *argc, t_atom **argv)
{
	if (HoaProcessorIsValid(HoaProcessor_Object))
		return (long) mess3((t_object *)HoaProcessor_Object, gensym("get_patcherargs"), (void *) index, argc, argv);
	
	return 0;
}

#endif