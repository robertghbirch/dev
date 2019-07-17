#include <iostream>
#include <fstream>
#include <string>
#include "p_wire_protocol.pb.h"
#include "main.h"

using namespace std;

int send_out(string s);
static size_t read_buffer(unsigned max_length, uint8_t * out, uint32_t * frame_len, uint32_t * frame_crc);

void hw(void);



//asio::io_service ioser;
//asio::serial_port portser(ioser);

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
}

const string reply_info_serial_hex(::google::protobuf::uint32 cmd_id)
{
    uint32_t frame_len = 0x12345678;
    uint32_t frame_crc = 0x12345678;
    uint8_t buffer[MAX_MSG_TO_DEVICE_SIZE];
    uint8_t bufferOut[MAX_MSG_TO_DEVICE_SIZE];
    uint8_t bufferOutCOB[MAX_MSG_TO_DEVICE_SIZE];
    uint8_t bufferIn[MAX_MSG_TO_DEVICE_SIZE];
    uint8_t bufferInCOB[MAX_MSG_TO_DEVICE_SIZE];
    int8_t *pb;

    string s;
    int i;
    string s_len;
    string s_msg;
    string s_crc;

  cerr << "size[" << s << "]\r\n";

    // TO DEVICE
    frl::device::wireformat::ToDeviceMsg toDeviceMsg;
    toDeviceMsg.set_cmd_id(cmd_id);
    toDeviceMsg.set_get_device_serial(true);
    toDeviceMsg.SerializeToString(&s);
    frame_len = (int32_t) s.size();

    toDeviceMsg.SerializeToString(&s_msg);


 send_out(s_msg) ;

/*
    frame_len = s_msg.length();

    pb = reinterpret_cast < int8_t * >(&frame_len);
    bufferOut[0] = pb[0];
    bufferOut[1] = pb[1];
    bufferOut[2] = pb[2];
    bufferOut[3] = pb[3];

    for (int c = 0; c < s_msg.length(); c++) {
        bufferOut[4 + c] = s_msg.at(c);
        cerr << ".";
    }

    pb = reinterpret_cast < int8_t * >(&frame_crc);
    bufferOut[4 + s_msg.length()] = pb[0];
    bufferOut[5 + s_msg.length()] = pb[1];
    bufferOut[6 + s_msg.length()] = pb[2];
    bufferOut[7 + s_msg.length()] = pb[3];

    size_t si = StuffData(bufferOut, 8 + s_msg.length(), bufferOutCOB);
    bufferOut[si++] = 0x00 ;

  cerr << "size[" << si << "]\r\n";

    
    for (int i = 0; i < (int)si; i++) {
        cout << bufferOutCOB[i];
    }
    cout.flush();
*/

    //for (int i = 0; i < 8 + s_msg.length(); i++) {
     //   cout << bufferOut[i];
    //}


    // TO HOST
/*
    cin.read(reinterpret_cast < char *>(&frame_len), sizeof(frame_len));
    cin.read(reinterpret_cast < char *>(buffer), frame_len);
    cin.read(reinterpret_cast < char *>(&frame_crc), sizeof(frame_crc));
*/

    frl::device::wireformat::ToHostMsg toHostMsg;
    size_t st = read_buffer(12, buffer, &frame_len, &frame_crc) ;


    toHostMsg.ParseFromArray(reinterpret_cast < const char *>(buffer), frame_len);

    switch (toHostMsg.payload_case()) {

    case toHostMsg.kTel:
        break;
    case toHostMsg.kReply:
        if (toHostMsg.has_reply()) {
            cerr << "reply.";
            frl::device::wireformat::ReplyMsg rm = toHostMsg.reply();
            if (rm.has_info()) {
                cerr << "info.";
                frl::device::wireformat::DeviceInfo di = rm.info();
                if (di.has_serial()) {
                    cerr << "serial.";
                    frl::device::wireformat::DeviceSerial ds = di.serial();
                    if (ds.has_hex()) {
                        cerr << "hex=" << "[" << ds.hex() << "]\r\n";
                        return (ds.hex());
                    }
                }
            }
        }
        break;
    case toHostMsg.PAYLOAD_NOT_SET:
        break;
    }
    return (NULL);
}


::google::protobuf::uint32 reply_info_fw_ver_major(::google::protobuf::uint32 cmd_id)
{
    uint32_t frame_len = 0x12345678;
    uint32_t frame_crc = 0x12345678;
    uint8_t buffer[MAX_MSG_TO_DEVICE_SIZE];
    string s;
    int i;

    // TO DEVICE
    frl::device::wireformat::ToDeviceMsg toDeviceMsg;
    toDeviceMsg.set_cmd_id(cmd_id);
    toDeviceMsg.set_get_fw_version(true);
    toDeviceMsg.SerializeToString(&s);
 	send_out(s) ;

    // TO HOST
    frl::device::wireformat::ToHostMsg toHostMsg;
    size_t st = read_buffer(12, buffer, &frame_len, &frame_crc) ;
    toHostMsg.ParseFromArray(reinterpret_cast < const char *>(buffer), frame_len);

    switch (toHostMsg.payload_case()) {

    case toHostMsg.kTel:
        break;
    case toHostMsg.kReply:
        if (toHostMsg.has_reply()) {
            cerr << "reply.";
            frl::device::wireformat::ReplyMsg rm = toHostMsg.reply();
            if (rm.has_info()) {
                cerr << "info.";
                frl::device::wireformat::DeviceInfo di = rm.info();
                if (di.has_fw_ver()) {
                    cerr << "fw_ver.";
                    frl::device::wireformat::FwVersion fwf = di.fw_ver();
                    if (fwf.has_major()) {
                        cerr << "major=" << "[" << fwf.major() << "]\r\n";
                    }
                    if (fwf.has_minor()) {
                        cerr << "minor=" << "[" << fwf.minor() << "]\r\n";
                    }
                    if (fwf.has_sha()) {
                        for (int i = 0; i < fwf.sha().length(); i++) {

                            cerr << "sha[" << i << "] = " << "[" << fwf.sha().at(i) << "]\r\n";
                        }
                        cerr << "sha=" << "[" << fwf.sha() << "]\r\n";
                    }
                    return (1); 
                }
            }
        }
        break;
    case toHostMsg.PAYLOAD_NOT_SET:
        break;
    }
    return (0);
}

::google::protobuf::uint32 tel_mainboard_error_code_error_message(::google::protobuf::uint32 cmd_id)
{
    uint32_t frame_len = 0x12345678;
    uint32_t frame_crc = 0x12345678;
    uint8_t buffer[MAX_MSG_TO_DEVICE_SIZE];
    string s;
    int i;

    // TO DEVICE
    frl::device::wireformat::ToDeviceMsg toDeviceMsg;
    toDeviceMsg.set_cmd_id(cmd_id);
    toDeviceMsg.set_get_board_info(true);
    toDeviceMsg.SerializeToString(&s);
 	send_out(s) ;
/*
    frame_len = (int32_t) s.size();
    cout.write(reinterpret_cast < const char *>(&frame_len), sizeof(frame_len));
    toDeviceMsg.SerializeToOstream(&cout);
    cout.write(reinterpret_cast < const char *>(&frame_crc), sizeof(frame_crc));
    cout.flush();
*/

    // TO HOST
    frl::device::wireformat::ToHostMsg toHostMsg;
/*
    cin.read(reinterpret_cast < char *>(&frame_len), sizeof(frame_len));
    cin.read(reinterpret_cast < char *>(buffer), frame_len);
    cin.read(reinterpret_cast < char *>(&frame_crc), sizeof(frame_crc));
*/
    size_t st = read_buffer(12, buffer, &frame_len, &frame_crc) ;
    toHostMsg.ParseFromArray(reinterpret_cast < const char *>(buffer), frame_len);

    switch (toHostMsg.payload_case()) {

    case toHostMsg.kTel:
        cerr << "EVENT EVENT\r\n";
        if (toHostMsg.has_tel()) {
            cerr << "tel.";
            frl::device::wireformat::EventMsg em = toHostMsg.tel();
            if (em.mbs_size() > 0) {
                cerr << "mainboard.";
                for (int i = 0; i < em.mbs_size(); i++) {
                    cerr << "error_code" << em.mbs(i).id();

                    if (em.mbs(i).has_error_code()) {
                        cerr << "error_message" << em.mbs(i).error_code() << em.mbs(i).error_msg();

                    }
                }
            }
        }
        break;
    case toHostMsg.kReply:
        break;
    case toHostMsg.PAYLOAD_NOT_SET:
        break;
    }
    return (0);
}


// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char *argv[])
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;


    reply_info_serial_hex(1);
    //reply_info_fw_ver_major(2);
    //tel_mainboard_error_code_error_message(3);
    //reply_info_serial_hex(1);

    google::protobuf::ShutdownProtobufLibrary();

    return (0);
}

int send_out(string s) {

    int32_t frame_len = 0x12345678;
    int32_t frame_crc = 0x12345678;
    uint8_t bufferOut[MAX_MSG_TO_DEVICE_SIZE];
    uint8_t bufferOutCOB[MAX_MSG_TO_DEVICE_SIZE];


    frame_len = s.length();

    int8_t *pb = reinterpret_cast < int8_t * >(&frame_len);
    bufferOut[0] = pb[0];
    bufferOut[1] = pb[1];
    bufferOut[2] = pb[2];
    bufferOut[3] = pb[3];

    for (int c = 0; c < s.length(); c++) {
        bufferOut[4 + c] = s.at(c);
        cerr << ".";
    }

    pb = reinterpret_cast < int8_t * >(&frame_crc);
    bufferOut[4 + s.length()] = pb[0];
    bufferOut[5 + s.length()] = pb[1];
    bufferOut[6 + s.length()] = pb[2];
    bufferOut[7 + s.length()] = pb[3];

    size_t si = StuffData(bufferOut, 8 + s.length(), bufferOutCOB);
    bufferOutCOB[si++] = 0x00 ;

  cerr << "size[" << si << "]\r\n";

    
    for (int i = 0; i < (int)si; i++) {
        cout << bufferOutCOB[i];
    }
    cout.flush();
}
