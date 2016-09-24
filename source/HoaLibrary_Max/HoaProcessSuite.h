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

#include "HoaCommon.max.h"

#define HoaProcessorIsValid(x) (x && ((((t_symbol *) object_classname((t_object*)x)) == gensym("hoa.process~"))))

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
	void *proc_obj = (void *)gensym("___HoaProcessor~___")->s_thing;

	if (HoaProcessorIsValid(proc_obj))
		return proc_obj;
	else
		return nullptr;
}


__inline long Get_HoaProcessor_Patch_Index(void *proc_obj) 
{
	if (HoaProcessorIsValid(proc_obj))
		return (long)gensym("___HoaProcessorPatchIndex___")->s_thing;
	else
		return 0l;
}


//////////////////////////////////////////////// Signal IO Queries ////////////////////////////////////////////////


__inline long HoaProcessor_Get_Declared_Sigins(void *proc_obj)
{
	if (HoaProcessorIsValid(proc_obj))
		return (long)object_method((t_object *)proc_obj, gensym("get_declared_sigins"));
	else
		return 0l;
}


__inline long HoaProcessor_Get_Declared_Sigouts(void *proc_obj)
{
	if (HoaProcessorIsValid(proc_obj))
		return (long)object_method((t_object *)proc_obj, gensym("get_declared_sigouts"));
	else
		return 0l;
}


__inline void** HoaProcessor_Get_Sigins(void *proc_obj)
{
	if (HoaProcessorIsValid(proc_obj))
        return (void**)object_method((t_object*)proc_obj, gensym("get_sigins"));
	else	
		return nullptr;
}


__inline void ***HoaProcessor_Get_Outptrs_Ptr(void *proc_obj, long index)
{
	if (HoaProcessorIsValid(proc_obj) && index > 0)
		return (void***)object_method((t_object *)proc_obj, gensym("get_outptrs_ptr"), (void*)index);
	else
		return nullptr;
}

__inline long HoaProcessor_Get_IO_Index(void *proc_obj, long patchIndex, t_object* io)
{
	if (HoaProcessorIsValid(proc_obj) && patchIndex > 0)
		return (long)object_method((t_object *)proc_obj, gensym("get_io_index"), &patchIndex, io);
	else
		return 0l;
}

//////////////////////////////////////////////// State Queries ////////////////////////////////////////////////


__inline long HoaProcessor_Get_Patch_On (void *proc_obj, long index)
{
	if (HoaProcessorIsValid(proc_obj) && index > 0)
		return (long)object_method((t_object *)proc_obj, gensym("get_patch_on"), (void *)index);
	
	return 0l;
}

__inline void HoaProcessor_Set_Patch_On (void *proc_obj, long index, long state)
{
	if (HoaProcessorIsValid(proc_obj) && index > 0)
		object_method((t_object *)proc_obj, gensym("set_patch_on"), (void *)index, (void *)state);
}

__inline long HoaProcessor_Get_Ambisonic_Order (void *proc_obj)
{
	if (HoaProcessorIsValid(proc_obj))
		return (long)object_method((t_object *)proc_obj, gensym("get_ambisonic_order"));
	
	return 0l;
}

__inline t_symbol *HoaProcessor_Get_Mode (void *proc_obj)
{
	if (HoaProcessorIsValid(proc_obj))
		return (t_symbol *)object_method((t_object *)proc_obj, gensym("get_mode"));
	
	return nullptr;
}

__inline long HoaProcessor_Get_Number_Of_Instances (void *proc_obj)
{
	if (HoaProcessorIsValid(proc_obj))
        return (long)object_method((t_object*)proc_obj, gensym("get_number_of_instance"));
	
	return 0l;
}

__inline long HoaProcessor_Is_2D (void *proc_obj)
{
	if (HoaProcessorIsValid(proc_obj))
		return (long)object_method((t_object *)proc_obj, gensym("is_2D"));
	
	return 0l;
}

__inline long HoaProcessor_Get_PatcherArgs(void *proc_obj, long index, long *argc, t_atom **argv)
{
	if (HoaProcessorIsValid(proc_obj))
        return object_method_direct(long, (t_object*, long, long*, t_atom**),
                                    (t_object*)proc_obj,
                                    gensym("get_patcherargs"), index, argc, argv);
	
	return 0l;
}

#endif