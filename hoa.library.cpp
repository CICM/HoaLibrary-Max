/*
 // Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "Max2D/Hoa2D.max.h"
#include "Max3D/Hoa3D.max.h"
#include "MaxCommon/HoaCommon.max.h"

#ifdef HOA_PACKED_LIB
int C74_EXPORT main(void)
{
    // HOA COMMON //
    c_convolve_main();
    c_freeverb_main();
    hoa_connect_main();
    hoa_dac_main();
    hoa_gain_main();
    hoa_in_main();
    hoa_in_sig_main();
    hoa_map_gui_main();
    hoa_out_main();
    hoa_out_sig_main();
    hoa_pi_main();
    hoa_pi_sig_main();
    hoa_play_main();
    hoa_process_main();
    hoa_record_main();
    hoa_thisprocess_main();
    
    // HOA 2D //
    hoa_2d_decoder_main();
    hoa_2d_encoder_main();
    hoa_2d_map_main();
    hoa_2d_meter_main();
    hoa_2d_optim_main();
    hoa_2d_projector_main();
    hoa_2d_recomposer_main();
    hoa_2d_recomposer_gui_main();
    hoa_2d_rotate_main();
    hoa_2d_scope_main();
    hoa_2d_space_main();
    hoa_2d_vector_main();
    hoa_2d_wider_main();
    
    // HOA 3D //
    hoa_3d_decoder_main();
    hoa_3d_encoder_main();
    hoa_3d_map_main();
    hoa_3d_meter_main();
    hoa_3d_optim_main();
    hoa_3d_scope_main();
    hoa_3d_vector_main();
    hoa_3d_wider_main();
    
    //return 0;
}
#endif