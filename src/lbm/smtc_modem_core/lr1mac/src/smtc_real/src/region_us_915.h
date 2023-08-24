/**
 * \file      region_us_915.h
 *
 * \brief     region_us_915 abstraction layer definition
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef REGION_US915_H
#define REGION_US915_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>

#include "smtc_real_defs.h"
#include "lbm/smtc_modem_core/lr1mac/src/lr1mac_defs.h"
#include "lbm/smtc_modem_core/lr1mac/src/lr1_stack_mac_layer.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/**
 * @brief Initialize the region with default value and pointers
 *
 * @param real
 */
void region_us_915_init( smtc_real_t* real );
/**
 * \brief Configure the region
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
void region_us_915_config( smtc_real_t* real );
/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
status_lorawan_t region_us_915_get_next_channel( smtc_real_t* real, uint8_t tx_data_rate, uint32_t* out_tx_frequency,
                                                 uint32_t* out_rx1_frequency );
/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
status_lorawan_t region_us_915_get_join_next_channel( smtc_real_t* real, uint8_t* out_tx_data_rate,
                                                      uint32_t* out_tx_frequency, uint32_t* out_rx1_frequency );
/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
void region_us_915_set_channel_mask( smtc_real_t* real );
/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
void region_us_915_init_join_snapshot_channel_mask( smtc_real_t* real );
/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
void region_us_915_init_after_join_snapshot_channel_mask( smtc_real_t* real, uint8_t tx_data_rate,
                                                          uint32_t tx_frequency );
/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
status_channel_t region_us_915_build_channel_mask( smtc_real_t* real, uint8_t ChMaskCntl, uint16_t ChMask );
/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
void region_us_915_enable_all_channels_with_valid_freq( smtc_real_t* real );
/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
status_lorawan_t region_us_915_is_acceptable_tx_dr( smtc_real_t* real, uint8_t dr, bool is_ch_mask_from_link_adr );

/**
 * @brief Get the corresponding RF modulation from a Datarate
 *
 * @param datarate
 * @return modulation_type_t
 */
modulation_type_t region_us_915_get_modulation_type_from_datarate( uint8_t datarate );

/**
 * @brief Convert LoRaWAN Datarate to LoRa SF and BW
 *
 * @param in_dr
 * @param out_sf
 * @param out_bw
 */
void region_us_915_lora_dr_to_sf_bw( uint8_t in_dr, uint8_t* out_sf, lr1mac_bandwidth_t* out_bw );

/**
 * @brief Convert LoRaWAN Datarate to LR-FHSS CR and BW
 *
 * @param [in]  in_dr
 * @param [out] out_cr
 * @param [out] out_bw
 */
void region_us_915_lr_fhss_dr_to_cr_bw( uint8_t in_dr, lr_fhss_v1_cr_t* out_cr, lr_fhss_v1_bw_t* out_bw );

/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
uint32_t region_us_915_get_tx_frequency_channel( smtc_real_t* real, uint8_t index );
/**
 * \brief
 * \remark
 * \param [IN]  none
 * \param [OUT] return
 */
uint32_t region_us_915_get_rx1_frequency_channel( smtc_real_t* real, uint8_t index );

/**
 * @brief Get the beacon frequency corresponding to a gps_time
 *
 * @param real
 * @param gps_time_s
 * @return uint32_t
 */
uint32_t region_us_915_get_rx_beacon_frequency_channel( smtc_real_t* real, uint32_t gps_time_s );

/**
 * @brief Get the ping slot frequency corresponding to a gps_time and Dev Address
 *
 * @param real
 * @param gps_time_s
 * @param dev_addr
 * @return uint32_t
 */
uint32_t region_us_915_get_rx_ping_slot_frequency_channel( smtc_real_t* real, uint32_t gps_time_s, uint32_t dev_addr );
#ifdef __cplusplus
}
#endif

#endif  // REGION_US915_H

/* --- EOF ------------------------------------------------------------------ */
