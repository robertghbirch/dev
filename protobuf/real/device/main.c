#include <stdio.h>
#include <stdlib.h>
#include "p_wire_protocol.pb-c.h"
#include "main.h"

int main(int argc, const char *argv[])
{
    uint32_t frame_len;
    uint32_t frame_crc;
    uint8_t hexy[4] = { 'N', 'I', 'C', 'E' };


    while (1) {

        // TO DEVICE
        fprintf(stderr, "\n\t\t\t\t\tDEVICE DEVICE DEVICE\n");
        Frl__Device__Wireformat__ToDeviceMsg *toDeviceMsg;
        uint8_t buf_to_device[MAX_MSG_TO_DEVICE_SIZE];  // From HAL
        frame_len = read_buffer(MAX_MSG_TO_DEVICE_SIZE, buf_to_device, &frame_len, &frame_crc);
        toDeviceMsg = frl__device__wireformat__to_device_msg__unpack(NULL, frame_len, buf_to_device);
        fprintf(stderr, "\t\t\t\t\t>> frame_len [%d] cmd [%d] frame_crc [%8X]\r\n", frame_len, (int) toDeviceMsg->cmd_id, frame_crc);
        fprintf(stderr, "\t\t\t\t\t>> frame_len [%d] cmd case [%d] frame_crc [%8X]\r\n", frame_len, (int) toDeviceMsg->cmd_case, frame_crc);

        // TO HOST
        uint8_t buf_to_host[MAX_MSG_TO_HOST_SIZE];  // From HAL
        fprintf(stderr, "\n\t\t\t\t\tMSG TO HOST\n");
        Frl__Device__Wireformat__ToHostMsg toHostMsg = FRL__DEVICE__WIREFORMAT__TO_HOST_MSG__INIT;
        Frl__Device__Wireformat__ReplyMsg replyMsg = FRL__DEVICE__WIREFORMAT__REPLY_MSG__INIT;
        Frl__Device__Wireformat__EventMsg telMsg = FRL__DEVICE__WIREFORMAT__EVENT_MSG__INIT;


        fprintf(stderr, "\t\t\t\t\ttoDeviceMsg->cmd_case = [%d] <-------------------------- \n", toDeviceMsg->cmd_case);

        Frl__Device__Wireformat__DeviceInfo deviceInfo = FRL__DEVICE__WIREFORMAT__DEVICE_INFO__INIT;
        Frl__Device__Wireformat__DeviceSerial deviceSerial = FRL__DEVICE__WIREFORMAT__DEVICE_SERIAL__INIT;
        Frl__Device__Wireformat__MainBoard mainBoard = FRL__DEVICE__WIREFORMAT__MAIN_BOARD__INIT;
        Frl__Device__Wireformat__MainBoard mainBoard2 = FRL__DEVICE__WIREFORMAT__MAIN_BOARD__INIT;
        Frl__Device__Wireformat__MainBoard *ap_mainBoard[1];
        ap_mainBoard[0] = &mainBoard;
        ap_mainBoard[1] = &mainBoard2;
        Frl__Device__Wireformat__FwVersion fwVersion = FRL__DEVICE__WIREFORMAT__FW_VERSION__INIT;

        fprintf(stderr, "\t\t\t\t\ttoDeviceMsg->cmd_case = [%d] <-------------------------- \n", toDeviceMsg->cmd_case);
        switch (toDeviceMsg->cmd_case) {
        case FRL__DEVICE__WIREFORMAT__TO_DEVICE_MSG__CMD_GET_DEVICE_SERIAL:
            fprintf(stderr, "\t\t\t\t\t>> Get device serial <---------------------------\n");
            toHostMsg.payload_case = FRL__DEVICE__WIREFORMAT__TO_HOST_MSG__PAYLOAD_REPLY;
            toHostMsg.reply = &replyMsg;
            toHostMsg.reply->info = &deviceInfo;
            deviceInfo.serial = &deviceSerial;
            deviceSerial.has_hex = 1;
            deviceSerial.hex.len = 4;
            deviceSerial.hex.data = (uint8_t *) & hexy;

            break;
        case FRL__DEVICE__WIREFORMAT__TO_DEVICE_MSG__CMD_GET_BOARD_INFO:
            toHostMsg.payload_case = FRL__DEVICE__WIREFORMAT__TO_HOST_MSG__PAYLOAD_TEL;
            fprintf(stderr, "\t\t\t\t\t>> Get board info <--------------------------- \n");
            toHostMsg.tel = &telMsg;
            toHostMsg.tel->n_mbs = 2;
            toHostMsg.tel->mbs = ap_mainBoard;
            mainBoard.id = 33;
            mainBoard2.id = 34;
            mainBoard.status = FRL__DEVICE__WIREFORMAT__TIMER__STATUS__ADJUSTING;
            mainBoard.has_error_code = 1;
            mainBoard.error_code = 44;
            mainBoard.error_msg = "EM";
            break;
        case FRL__DEVICE__WIREFORMAT__TO_DEVICE_MSG__CMD_GET_FW_VERSION:
            toHostMsg.payload_case = FRL__DEVICE__WIREFORMAT__TO_HOST_MSG__PAYLOAD_REPLY;
            toHostMsg.reply = &replyMsg;
            toHostMsg.reply->info = &deviceInfo;
            deviceInfo.fw_ver = &fwVersion;
            fwVersion.has_major = 1;
            fwVersion.major = 10;
            fwVersion.has_minor = 1;
            fwVersion.minor = 11;
            fwVersion.has_sha = 1;
            fwVersion.sha.len = 8;

            volatile uint8_t shany[8] = { 's', 'h', 'a', '1', '2', '3', '4', '5' };
            //uint8_t shany[8] = { 's', 'H', 'a', '1', '2', '8', '4', 0x00 };
            fwVersion.sha.data = (uint8_t *) & shany;
            fprintf(stderr, "\t\t\t\t\t>> Get fw version <--------------------------- \n");
            break;

        case FRL__DEVICE__WIREFORMAT__TO_DEVICE_MSG__CMD_GET_PROTOCOL_VERSION:
            break;
        }


        frame_crc = 0x87654321;
        frame_len = frl__device__wireformat__to_host_msg__pack(&toHostMsg, buf_to_host);
        //replyMsg.cmd_id = toDeviceMsg->cmd_case ;
        fprintf(stderr, "\t\t\t\t\t<< frame_len [%d] cmd [%d] frame_crc [%8X]\r\n", frame_len, (int) replyMsg.cmd_id, frame_crc);
        fprintf(stderr, "\t\t\t\t\t<< frame_len [%d] payload case [%d] frame_crc [%8X]\r\n", frame_len, (int) toHostMsg.payload_case, frame_crc);

	write_buffer(buf_to_host, &frame_len, &frame_crc) ;
/*
        fwrite(&frame_len, 4, 1, stdout);
        fwrite(buf_to_host, frame_len, 1, stdout);
        fwrite(&frame_crc, 4, 1, stdout);
        fflush(stdout);
*/

     //   break;
    }

    return 0;
}


static size_t write_buffer(uint8_t * out, uint32_t * frame_len, uint32_t * frame_crc) {
    uint8_t bufferOut[MAX_MSG_TO_DEVICE_SIZE];
    uint8_t bufferOutCOB[MAX_MSG_TO_DEVICE_SIZE];

    int8_t *pb = (int8_t *)(frame_len);
    bufferOut[0] = pb[0];
    bufferOut[1] = pb[1];
    bufferOut[2] = pb[2];
    bufferOut[3] = pb[3];

    for (int c = 0; c < *frame_len; c++) {
        bufferOut[4 + c] = out[c];
    }

    pb = (int8_t *)frame_crc;
    bufferOut[4 + *frame_len] = pb[0];
    bufferOut[5 + *frame_len] = pb[1];
    bufferOut[6 + *frame_len] = pb[2];
    bufferOut[7 + *frame_len] = pb[3];

    size_t si = StuffData(bufferOut, 8 + *frame_len, bufferOutCOB);
    bufferOutCOB[si++] = 0x00 ;
	fprintf(stderr, "\t\t\t\t\t[%d]\n", (int)si) ;

    for (int i = 0; i < (int)si; i++) {
        fwrite(&bufferOutCOB[i],1,1,stdout);
	//fprintf(stderr, "[%d]\n", bufferOutCOB[i]) ;
    }
    fflush(stdout);
}

static size_t read_buffer(unsigned max_length, uint8_t * out, uint32_t * frame_len, uint32_t * frame_crc)
{
    size_t n;
    size_t nread;
    uint8_t buffer_temp[MAX_MSG_TO_DEVICE_SIZE_TEMP];
    uint8_t buffer_temp2[MAX_MSG_TO_DEVICE_SIZE_TEMP];
    uint8_t b;

    // break on zero
    nread = 0;
    while (1) {
        n = fread(&b, 1, 1, stdin);
        //fprintf(stderr, "\t\t\t\t\t[%d]\r\n", (int) b);
        buffer_temp[nread] = b;
        nread++;
        if (0x00 == b) {
            if (nread == 1) {
                nread = 0;
                continue;
            } else {
                nread--;
                break;
            }
        }
    }

    fprintf(stderr, "\t\t\t\t\tnread[%d]\n", (int) nread);

    nread = UnStuffData(buffer_temp, (int) nread, buffer_temp2);

    ((int8_t *) frame_len)[0] = buffer_temp2[0];
    ((int8_t *) frame_len)[1] = buffer_temp2[1];
    ((int8_t *) frame_len)[2] = buffer_temp2[2];
    ((int8_t *) frame_len)[3] = buffer_temp2[3];

    for (int i = 0; i < *frame_len; i++) {
        out[i] = buffer_temp2[4 + i];
    }


    ((int8_t *) frame_crc)[0] = buffer_temp2[4 + *frame_len];
    ((int8_t *) frame_crc)[1] = buffer_temp2[5 + *frame_len];
    ((int8_t *) frame_crc)[2] = buffer_temp2[6 + *frame_len];
    ((int8_t *) frame_crc)[3] = buffer_temp2[7 + *frame_len];


    return *frame_len;

    fprintf(stderr, "little endian[%d]\n", (int) *frame_len);

    // Get frame size
    if ((nread = fread(frame_len, 4, 1, stdin)) != 1) {
        fprintf(stderr, "\t\t\t\t\tmax message length exceeded\n");
        exit(1);
    }
    fprintf(stderr, "little endian[%d]\n", (int) *frame_len);

    while ((nread = fread(out, *frame_len, 1, stdin)) != 1) {
        fprintf(stderr, "\t\t\t\t\tmax message length exceeded\n");
        exit(1);
    }
    fprintf(stderr, "\t\t\t\t\tmax message length exceeded\n");

    if ((nread = fread(frame_crc, 4, 1, stdin)) != 1) {
        fprintf(stderr, "\t\t\t\t\tmax message length exceeded\n");
    }
    fprintf(stderr, "crc [%4x]\n", (int) *frame_crc);
    return *frame_len;
}
