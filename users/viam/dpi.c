
    #include <stdbool.h>
    #include <stdio.h>
    #include "quantum.h"
    #include "dpi.h"
    #include "ploopy_via.h"

    void dpi_set_dpi( void ){
        if(ds_state.sniper_b_enabled){
            pointing_device_set_cpi(ploopyvia_config.sniper_b_dpi);
        }
        else if(ds_state.sniper_a_enabled){
            pointing_device_set_cpi(ploopyvia_config.sniper_a_dpi);
        }
        else {
            pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
        }
    }

    bool process_record_dpi(uint16_t keycode, keyrecord_t *record) {
        switch (keycode) {
            case DPI_SNIPER_A_MOMENTARY:
                ds_state.sniper_a_enabled = record->event.pressed;
                dpi_set_dpi();
                dprintf("snp_a:%d dpi:%d\n", ds_state.sniper_a_enabled, ploopyvia_config.sniper_a_dpi);
                return false;

            case DPI_SNIPER_A_TOGGLE:
                if(record->event.pressed){
                    ds_state.sniper_a_enabled ^= 1;
                    dpi_set_dpi();
                    dprintf("snp_a:%d dpi:%d\n", ds_state.sniper_a_enabled, ploopyvia_config.sniper_a_dpi);
                }
                return false;

            case DPI_SNIPER_B_MOMENTARY:
                ds_state.sniper_b_enabled = record->event.pressed;
                dpi_set_dpi();
                dprintf("snp_b:%d dpi:%d\n", ds_state.sniper_b_enabled, ploopyvia_config.sniper_b_dpi);
                return false;

            case DPI_SNIPER_B_TOGGLE:
                if(record->event.pressed){
                    ds_state.sniper_b_enabled ^= 1;
                    dpi_set_dpi();
                    dprintf("snp_b:%d dpi:%d\n", ds_state.sniper_b_enabled, ploopyvia_config.sniper_b_dpi);
                }
                return false;
        }
        return true;
    }
