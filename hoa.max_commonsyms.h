/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __DEF_HOA_MAX_COMMONSYMS__
#define __DEF_HOA_MAX_COMMONSYMS__

extern "C"
{
#include <ext.h>
}

// Hoa
static t_symbol* hoa_sym_hoa_getinfos				= gensym("hoa_getinfos");

// Objects
static t_symbol* hoa_sym_hoa_2d_vector				= gensym("hoa.2d.vector~");
static t_symbol* hoa_sym_hoa_gain					= gensym("hoa.gain~");
static t_symbol* hoa_sym_hoa_2d_meter				= gensym("hoa.2d.meter~");
static t_symbol* hoa_sym_hoa_3d_meter				= gensym("hoa.3d.meter~");
static t_symbol* hoa_sym_hoa_2d_scope				= gensym("hoa.2d.scope~");
static t_symbol* hoa_sym_hoa_3d_scope				= gensym("hoa.3d.scope~");
static t_symbol* hoa_sym_hoa_dac					= gensym("hoa.dac~");
static t_symbol* hoa_sym_dac						= gensym("dac~");
static t_symbol* hoa_sym_hoa_pi						= gensym("hoa.pi");
static t_symbol* hoa_sym_hoa_pi_tilde               = gensym("hoa.pi~");


// notify
static t_symbol* hoa_sym_nothing 					= gensym("");
static t_symbol* hoa_sym_notify 					= gensym("notify");
static t_symbol* hoa_sym_modified 					= gensym("modified");
static t_symbol* hoa_sym_attr_modified 				= gensym("attr_modified");

// Methods
static t_symbol* hoa_sym_set 						= gensym("set");
static t_symbol* hoa_sym_bang 						= gensym("bang");
static t_symbol* hoa_sym_float 						= gensym("float");
static t_symbol* hoa_sym_int 						= gensym("int");
static t_symbol* hoa_sym_list 						= gensym("list");
static t_symbol* hoa_sym_anything					= gensym("anything");
static t_symbol* hoa_sym_dspchain					= gensym("dspchain");
static t_symbol* hoa_sym_dynlet_begin 				= gensym("dynlet_begin");
static t_symbol* hoa_sym_dynlet_end 				= gensym("dynlet_end");
static t_symbol* hoa_sym_start						= gensym("start");
static t_symbol* hoa_sym_stop 						= gensym("stop");
static t_symbol* hoa_sym_front 						= gensym("front");
static t_symbol* hoa_sym_iterate 					= gensym("iterate");

// common messages
static t_symbol* hoa_sym_done						= gensym("done");
static t_symbol* hoa_sym_2d							= gensym("2d");
static t_symbol* hoa_sym_3d							= gensym("3d");

// hoa.process
static t_symbol* hoa_sym_getassoc					= gensym("getassoc");
static t_symbol* hoa_sym_setassoc 					= gensym("setassoc");
static t_symbol* hoa_sym_HoaProcessor 				= gensym("___HoaProcessor~___");				// Capitals must stay here
static t_symbol* hoa_sym_HoaProcessorPatchIndex 	= gensym("___HoaProcessorPatchIndex___");		// Capitals must stay here
static t_symbol* hoa_sym_sigin 						= gensym("hoa.in~");
static t_symbol* hoa_sym_sigout 					= gensym("hoa.out~");
static t_symbol* hoa_sym_in 						= gensym("hoa.in");
static t_symbol* hoa_sym_out 						= gensym("hoa.out");
static t_symbol* hoa_sym_args 						= gensym("args");
static t_symbol* hoa_sym_process_mode_no			= gensym("no");
static t_symbol* hoa_sym_process_mode_post 			= gensym("post");
static t_symbol* hoa_sym_process_mode_out 			= gensym("out");
static t_symbol* hoa_sym_ambisonics					= gensym("ambisonics");
static t_symbol* hoa_sym_harmonics					= gensym("harmonics");
static t_symbol* hoa_sym_planewaves					= gensym("planewaves");
static t_symbol* hoa_sym_extra						= gensym("extra");
static t_symbol* hoa_sym_comment					= gensym("comment");
static t_symbol* hoa_sym_out_message 				= gensym("out_message");

// Trigo
static t_symbol* hoa_sym_cartesian 					= gensym("cartesian");
static t_symbol* hoa_sym_car 						= gensym("car");
static t_symbol* hoa_sym_polar 						= gensym("polar");
static t_symbol* hoa_sym_pol 						= gensym("pol");
static t_symbol* hoa_sym_mute 						= gensym("mute");
static t_symbol* hoa_sym_abscissa 					= gensym("abscissa");
static t_symbol* hoa_sym_ordinate 					= gensym("ordinate");
static t_symbol* hoa_sym_height 					= gensym("height");
static t_symbol* hoa_sym_radius 					= gensym("radius");
static t_symbol* hoa_sym_azimuth 					= gensym("azimuth");
static t_symbol* hoa_sym_elevation 					= gensym("elevation");
static t_symbol* hoa_sym_angle 						= gensym("angle");
static t_symbol* hoa_sym_angles						= gensym("angles");
static t_symbol* hoa_sym_wide						= gensym("wide");
static t_symbol* hoa_sym_directivities				= gensym("directivities");

// Paint
static t_symbol* hoa_sym_rect 						= gensym("rect");
static t_symbol* hoa_sym_bgcolor 					= gensym("bgcolor");
static t_symbol* hoa_sym_bdcolor 					= gensym("bdcolor");

// Patchers
static t_symbol* hoa_sym_patcher 					= gensym("patcher");
static t_symbol* hoa_sym_bpatcher 					= gensym("bpatcher");
static t_symbol* hoa_sym_jpatcher 					= gensym("jpatcher");
static t_symbol* hoa_sym_noedit 					= gensym("noedit");
static t_symbol* hoa_sym_vis 						= gensym("vis");
static t_symbol* hoa_sym_invis 						= gensym("invis");
static t_symbol* hoa_sym_pound_P 					= gensym("#P");
static t_symbol* hoa_sym_pound_B 					= gensym("#B");
static t_symbol* hoa_sym_selectbox 					= gensym("selectbox");
static t_symbol* hoa_sym_insertboxtext 				= gensym("insertboxtext");
static t_symbol* hoa_sym_gettextptr 				= gensym("gettextptr");
static t_symbol* hoa_sym_firstview 					= gensym("firstview");
static t_symbol* hoa_sym_connect 					= gensym("connect");
static t_symbol* hoa_sym_selectedboxes				= gensym("selectedboxes");
static t_symbol* hoa_sym_startdrag					= gensym("startdrag");

// Map
static t_symbol* hoa_sym_source 					= gensym("source");
static t_symbol* hoa_sym_group 						= gensym("group");
static t_symbol* hoa_sym_trajectory_parameters 		= gensym("trajectory_parameters");
static t_symbol* hoa_sym_slots_parameters 			= gensym("slots_parameters");
static t_symbol* hoa_sym_sources_parameters 		= gensym("sources_parameters");
static t_symbol* hoa_sym_groups_parameters 			= gensym("groups_parameters");
static t_symbol* hoa_sym_slot 						= gensym("slot");
static t_symbol* hoa_sym_trajectory 				= gensym("trajectory");
static t_symbol* hoa_sym_remove 					= gensym("remove");
static t_symbol* hoa_sym_description 				= gensym("description");
static t_symbol* hoa_sym_color 						= gensym("color");
static t_symbol* hoa_sym_source_preset_data			= gensym("_source_preset_data_");
static t_symbol* hoa_sym_group_preset_data			= gensym("_group_preset_data_");

// recomposer
static t_symbol* hoa_sym_channels					= gensym("channels");
static t_symbol* hoa_sym_fixe						= gensym("fixe");
static t_symbol* hoa_sym_fisheye					= gensym("fisheye");

// optim
static t_symbol* hoa_sym_basic                      = gensym("basic");
static t_symbol* hoa_sym_maxRe						= gensym("maxRe");
static t_symbol* hoa_sym_inPhase					= gensym("inPhase");

// decoder
static t_symbol* hoa_sym_ambisonic					= gensym("ambisonic");
static t_symbol* hoa_sym_irregular					= gensym("irregular");
static t_symbol* hoa_sym_binaural					= gensym("binaural");
static t_symbol* hoa_sym_offset						= gensym("offset");
static t_symbol* hoa_sym_pinna						= gensym("pinna");
static t_symbol* hoa_sym_small						= gensym("small");
static t_symbol* hoa_sym_large						= gensym("large");

// Layers
static t_symbol* hoa_sym_background_layer 			= gensym("background_layer");
static t_symbol* hoa_sym_sources_layer 				= gensym("sources_layer");
static t_symbol* hoa_sym_groups_layer 				= gensym("groups_layer");
static t_symbol* hoa_sym_rect_selection_layer 		= gensym("rect_selection_layer");
static t_symbol* hoa_sym_harmonics_layer	 		= gensym("harmonics_layer");
static t_symbol* hoa_sym_space_layer		 		= gensym("space_layer");
static t_symbol* hoa_sym_points_layer		 		= gensym("points_layer");
static t_symbol* hoa_sym_channels_layer				= gensym("channels_layer");
static t_symbol* hoa_sym_text_layer					= gensym("text_layer");
static t_symbol* hoa_sym_fisheye_layer				= gensym("fisheye_layer");
static t_symbol* hoa_sym_rectselection_layer		= gensym("rectselection_layer");
static t_symbol* hoa_sym_vectors_layer				= gensym("vectors_layer");
static t_symbol* hoa_sym_leds_layer					= gensym("leds_layer");
static t_symbol* hoa_sym_separator_layer			= gensym("separator_layer");
static t_symbol* hoa_sym_skeleton_layer				= gensym("skeleton_layer");


static t_symbol* hoa_sym_relpolar 					= gensym("relpolar");
static t_symbol* hoa_sym_relradius 					= gensym("relradius");
static t_symbol* hoa_sym_relazimuth					= gensym("relazimuth");
static t_symbol* hoa_sym_relelevation				= gensym("relelevation");

// Preset
static t_symbol* hoa_sym_store 						= gensym("store");
static t_symbol* hoa_sym_storeagain 				= gensym("storeagain");
static t_symbol* hoa_sym_storeempty 				= gensym("storeempty");
static t_symbol* hoa_sym_storeend 					= gensym("storeend");
static t_symbol* hoa_sym_storenext 					= gensym("storenext");
static t_symbol* hoa_sym_insert 					= gensym("insert");
static t_symbol* hoa_sym_delete 					= gensym("delete");
static t_symbol* hoa_sym_copy 						= gensym("copy");
static t_symbol* hoa_sym_renumber 					= gensym("renumber");
static t_symbol* hoa_sym_clear 						= gensym("clear");
static t_symbol* hoa_sym_recall 					= gensym("recall");
static t_symbol* hoa_sym_read 						= gensym("read");
static t_symbol* hoa_sym_write 						= gensym("write");
static t_symbol* hoa_sym_storesource 				= gensym("storesource");
static t_symbol* hoa_sym_storegroup 				= gensym("storegroup");
static t_symbol* hoa_sym_record 					= gensym("record");
static t_symbol* hoa_sym_limit 						= gensym("limit");
static t_symbol* hoa_sym_erase 						= gensym("erase");
static t_symbol* hoa_sym_erasepart 					= gensym("erasepart");

static t_symbol* hoa_sym_free 						= gensym("free");
static t_symbol* hoa_sym_endeditbox 				= gensym("endeditbox");
static t_symbol* hoa_sym_text 						= gensym("text");
static t_symbol* hoa_sym_getname 					= gensym("getname");

static t_symbol* hoa_sym_zoom 						= gensym("zoom");
static t_symbol* hoa_sym_number 					= gensym("number");
static t_symbol* hoa_sym_index 						= gensym("index");

#endif
