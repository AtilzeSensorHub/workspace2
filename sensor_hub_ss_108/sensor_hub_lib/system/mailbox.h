/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 mailbox.h
 Mailbox library file.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : mailbox.h

 History:
   1.00 HW 18-Aug-17 Initial
 */

#ifndef SENSOR_HUB_LIB_SYSTEM_MAILBOX_H_
#define SENSOR_HUB_LIB_SYSTEM_MAILBOX_H_

int send_quark_data(qm_mbox_ch_t mbox_rx, qm_mbox_ch_t mbox_tx);

void mailbox_init(qm_mbox_ch_t *mbox_rx, qm_mbox_ch_t *mbox_tx);

#endif /* SENSOR_HUB_LIB_SYSTEM_MAILBOX_H_ */
