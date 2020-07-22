/****************************************************************************
 * apps/external/zblue/port/common/defines.c
 *
 *   Copyright (C) 2020 Xiaomi InC. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <kernel.h>
#include <net/buf.h>

/* bt_l2cap_fixed_chan START */

extern const struct bt_l2cap_fixed_chan att_fixed_chan;
extern const struct bt_l2cap_fixed_chan le_fixed_chan;
extern const struct bt_l2cap_fixed_chan smp_fixed_chan;
extern const struct bt_l2cap_fixed_chan smp_br_fixed_chan;

const struct bt_l2cap_fixed_chan *_bt_l2cap_fixed_chan_list[] =
{
#if defined(CONFIG_BT_CONN)
	&att_fixed_chan,
	&le_fixed_chan,
#endif
#if defined(CONFIG_BT_SMP)
	&smp_fixed_chan,
#endif
#if defined(CONFIG_BT_BREDR)
	&smp_br_fixed_chan,
#endif
  NULL
};

/* bt_l2cap_fixed_chan END */

/* bt_gatt_service_static START */

extern struct bt_gatt_service_static _1_gatt_svc;
extern struct bt_gatt_service_static _2_gap_svc;
extern struct bt_gatt_service_static bas;
extern struct bt_gatt_service_static dis_svc;
extern struct bt_gatt_service_static hrs_svc;

struct bt_gatt_service_static *_bt_gatt_service_static_list[] =
{
#if defined(CONFIG_BT_CONN)
	&_1_gatt_svc,
	&_2_gap_svc,
#endif
#if defined(CONFIG_BT_GATT_BAS)
	&bas,
#endif
#if defined(CONFIG_BT_GATT_DIS)
	&dis_svc,
#endif
#if defined(CONFIG_BT_GATT_HRS)
	&hrs_svc,
#endif
  NULL
};

/* bt_gatt_service_static END */

/* net_buf_pool START */

extern struct net_buf_pool acl_in_pool;
extern struct net_buf_pool acl_tx_pool;
extern struct net_buf_pool adv_buf_pool;
extern struct net_buf_pool br_sig_pool;
extern struct net_buf_pool discardable_pool;
extern struct net_buf_pool disc_pool;
extern struct net_buf_pool frag_pool;
extern struct net_buf_pool friend_buf_pool;
extern struct net_buf_pool hci_acl_pool;
extern struct net_buf_pool hci_cmd_pool;
extern struct net_buf_pool hci_rx_pool;
extern struct net_buf_pool hf_pool;
extern struct net_buf_pool num_complete_pool;
extern struct net_buf_pool prep_pool;
extern struct net_buf_pool sdp_pool;

struct net_buf_pool *net_buf_pool_list[] =
{
	&hci_cmd_pool,
	&hci_rx_pool,
#if defined(CONFIG_BT_HCI_RAW)
	&hci_acl_pool,
#endif
#if CONFIG_BT_L2CAP_TX_FRAG_COUNT > 0
	&frag_pool,
#endif
#if defined(CONFIG_BT_HCI_ACL_FLOW_CONTROL)
	&acl_in_pool,
#endif
#if defined(CONFIG_BT_CONN)
	&acl_tx_pool,
	&disc_pool,
	&num_complete_pool,
#endif
#if defined(CONFIG_BT_DISCARDABLE_BUF_COUNT)
	&discardable_pool,
#endif
#if CONFIG_BT_ATT_PREPARE_COUNT > 0
	&prep_pool,
#endif
#if defined(CONFIG_BT_MESH)
	&adv_buf_pool,
#endif
#if defined(CONFIG_BT_MESH_FRIEND)
	&friend_buf_pool,
#endif
#if defined(CONFIG_BT_HFP_HF)
	&hf_pool,
#endif
#if defined(CONFIG_BT_BREDR)
	&br_sig_pool,
	&sdp_pool,
#endif
#if defined(CONFIG_BT_RFCOMM)
	&dummy_pool,
#endif
};

int net_buf_pool_list_size = ARRAY_SIZE(net_buf_pool_list);

/* net_buf_pool END */
