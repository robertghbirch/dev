#include <iomanip>
#include <unistd.h>
#include <asio.hpp>
#include <pangolin/pangolin.h>
#include <pangolin/utils/timer.h>
#include <surreal/Device/Kc28/tbwireformat.h>
#include <CLI/App.hpp>
#include <CLI/Validators.hpp>

#include <termios.h>

//#include "pcie.h"
//#include "pcie.h"
//#include <stdint.h>
#include <sys/io.h>


using namespace surreal;

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PARALLEL_PORT_BASE    0x3010
#define PARALLEL_PORT_DATA    (PARALLEL_PORT_BASE + 0)
#define PARALLEL_PORT_STATUS  (PARALLEL_PORT_BASE + 1)
#define PARALLEL_PORT_CNTRL   (PARALLEL_PORT_BASE + 2)


FILE *DB1;
FILE *DB2;
FILE *DB3;
FILE *DB4;
int db1;
int db2;
int db3;
int db4;

int iMinErrors = 5;

//#define USE_TCP


asio::io_service ioser;
asio::serial_port portser(ioser);


uint8_t toHostBuf[512];
void portser_handler(const asio::error_code & error, std::size_t bytes_transferred)
{
    toHostBuf[bytes_transferred] = 0x00;
    fprintf(stderr, "toHost[%s]\r\n", toHostBuf);
}


#define T8TABS "\t\t\t\t\t\t\t\t"
#define T5TABS "\t\t\t\t\t"
// This function reads until it receives a zero
uint8_t toHostZero[1024];
uint32_t iToHostZero = 0;
void toHost()
{
    fprintf(stderr, "toHost()\r\n");

    while (true) {
        size_t st = portser.read_some(asio::buffer(toHostBuf, 64));
        for (uint32_t i = 0; i < st; i++) {
            toHostZero[iToHostZero] = toHostBuf[i];
            if (toHostZero[iToHostZero] == 0x00) {
                fprintf(stderr, "%sFROM DEVICE\t[%s] numRead[%d] [", T8TABS, toHostZero, (int)st);
                for (uint32_t i = 0; i < st; i++) {
                    fprintf(stderr, "%x", toHostZero[i]);
                    if (i != (st - 1)) {
                        fprintf(stderr, ":");
                    }
                }
            	fprintf(stderr, "]\r\n");
                iToHostZero = 0;
            } else {
                iToHostZero++;
                if (iToHostZero == 1024) {
                    fprintf(stderr, "error\r\n");
                    exit(1);
                }
            }
        }
        toHostBuf[st] = 0x00;
        fprintf(stderr, "FROM DEVICE chunk [%s]\r\n", toHostBuf);
    }
}


uint8_t fromKB[512];
static uint8_t m_io = 0x00;
void toDevice()
{
    fprintf(stderr, "toDevice()\r\n");

    while (1) {
        //fprintf(stderr, ".");

        fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
        int numRead = read(0, fromKB, 18);
	fromKB[numRead] = 0x00;
if (numRead > 0) {
        fprintf(stderr, "KYORD[%s] numRead[%d]", fromKB, numRead);
}
        if (numRead > 1) {
	    if (numRead == 19) {
		if (fromKB[numRead - 1] == '\n') {
		    fromKB[numRead - 1] = 0x00;
		}
	    } else {
		    fromKB[numRead - 1] = 0x00;
		}
            fprintf(stderr, "%sKEYBOARD\t[%s] numRead[%d] [", T8TABS, fromKB, numRead);
            for (int i = 0; i < numRead; i++) {
                fprintf(stderr, "%x", fromKB[i]);
                if (i != (numRead - 1)) {
                    fprintf(stderr, ":");
                }
            }
            fprintf(stderr, "]\r\n");
            portser.write_some(asio::buffer(fromKB, numRead));
        }
        //m_io ^= (0x01 << 0);
        //outb(m_io, PARALLEL_PORT_DATA);
        //m_io ^= (0x01 << 0);
        //outb(m_io, PARALLEL_PORT_DATA);
    }
}


int main(int argc, char **argv)
{
    //ioperm(PARALLEL_PORT_DATA, 8, 1);
	hw() ;

    printf("%sred\n", KRED);
    printf("%sgreen\n", KGRN);
    printf("%syellow\n", KYEL);
    printf("%sblue\n", KBLU);
    printf("%smagenta\n", KMAG);
    printf("%scyan\n", KCYN);
    printf("%swhite\n", KWHT);
    printf("%snormal\n", KNRM);
    //DB1 = fopen("/dev/pts/11", "w"); // use xterm& and then type tty to find the number
    DB1 = fopen("/dev/null", "w");  // use xterm& and then type tty to find the number
    //DB2 = fopen ("/dev/pts/30", "w");
    DB2 = fopen("/dev/null", "w");
    //DB3 = fopen("/dev/pts/23", "w");
    DB3 = fopen("/tmp/23.log", "w");
    //DB4 = fopen ("/dev/pts/24", "w");
    DB4 = fopen("/dev/null", "w");

    for (int i = 0; i < 40; fprintf(DB1, "\r\n"), i++);
    fprintf(DB1, "%s**************** DB1 ******************\r\n", KRED);
    for (int i = 0; i < 40; fprintf(DB2, "\r\n"), i++);
    fprintf(DB2, "%s**************** DB2 ******************\r\n", KYEL);
    for (int i = 0; i < 40; fprintf(DB3, "\r\n"), i++);
    fprintf(DB3, "%s**************** DB3 ******************\r\n", KGRN);
    db1 = 0;
    db2 = 0;
    db3 = 0;


/*
    app.add_option("--headset_ip,-i", ip, "IPV4 ip of the headset.")->required();
    app.add_option("--headset_port,-p", port, "Port on the headset.")->required();

    try {
        app.parse(argc, argv);
    }
    catch(const CLI::ParseError & e) {
        return app.exit(e);
    }
*/

    //asio::io_service aios_;

    //bool use_udp = true;

    //std::unique_ptr < SocketConnection > socket_connection = std::make_unique < SocketConnection > (use_udp, ip, port);

    // socket_connection->Connect ();


    fprintf(stderr, "ACM0 trying to open\n");
    portser.open("/dev/ttyACM0");
    portser.set_option(asio::serial_port_base::baud_rate(115200));

    printf("ACM0 open\n");

    std::thread first(toHost);
    std::thread second(toDevice);

    first.join();
    second.join();

    return (0);


    //char c;
    //uint16_t iBuff = 0;
    fprintf(stderr, "asdflkasjdf");


    return (1);

    //asio::ip::tcp::socket::receive_buffer_size option(64) ;
    // socket_.set_option(option) ;

    //asio::ip::tcp::socket::receive_buffer_size option2 ;
    //socket_.get_option(option2) ;
    //int size = option2.value();

    //printf("buffer size [%d]\r\n", (int)size) ;


    TBWireFormat::H2BMsg h2bmsg = {
    };
    TBWireFormat::B2HMsg b2hmsg = {
    };
    size_t len;


    // REQUEST INFO (gets TCP started)
    h2bmsg.type = TBWireFormat::H2BMsgType::kRequest;
    h2bmsg.data.req.cmd = TBWireFormat::RequestCmdType::kInfoR;
    //len = socket_connection->Write (h2bmsg);
/*
*/
#ifdef kfkjf
    sleep(1);

    // INVENSENSE IMU
    h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
    h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kEnableDisableImu;
    h2bmsg.data.ctrl.extra[0] = 1;
    h2bmsg.data.ctrl.extra[1] = uint8_t(TBWireFormat::ImuSource::kImuInvenSense);
    //len = socket_connection->Write (h2bmsg);
    sleep(1);

    h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
    h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kEnableDisableImu;
    h2bmsg.data.ctrl.extra[0] = 1;
    h2bmsg.data.ctrl.extra[1] = uint8_t(TBWireFormat::ImuSource::kImuBosch);
    //len = socket_connection->Write (h2bmsg);
    sleep(1);
#endif

    // BOSCH IMU
/*
  h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
  h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kEnableDisableImu;
  h2bmsg.data.ctrl.extra[0] = 1;
  h2bmsg.data.ctrl.extra[1] = uint8_t (TBWireFormat::ImuSource::kImuBosch);
  len = socket_connection->Write (h2bmsg);
      len = socket_connection->Read (b2hmsg);

      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
      len = socket_connection->Read (b2hmsg);
*/

    // TRIGGER LEFT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 1;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    sleep(1);
    //len = socket_connection->Read (b2hmsg);

#ifdef kfkfj
#endif
    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 2;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    sleep(1);
    //len = socket_connection->Read (b2hmsg);

    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 3;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    sleep(1);
    //len = socket_connection->Read (b2hmsg);

    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 4;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    //len = socket_connection->Read (b2hmsg);

    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 5;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    //len = socket_connection->Read (b2hmsg);
    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 6;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    //len = socket_connection->Read (b2hmsg);

    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 7;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    //len = socket_connection->Read (b2hmsg);

    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 8;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    //len = socket_connection->Read (b2hmsg);
    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 8;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    //len = socket_connection->Read (b2hmsg);
    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 8;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //nnnlen = socket_connection->Write (h2bmsg);
    //len = socket_connection->Read (b2hmsg);
    // TRIGGER RIGHT SLAM
    h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
    h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
    h2bmsg.data.trig.channel = 8;
    h2bmsg.data.trig.master = 1;
    h2bmsg.data.trig.periodNs = 40000000;
    h2bmsg.data.trig.offsetNs = 0;
    h2bmsg.data.trig.activeTimeNs = 1000000;
    h2bmsg.data.trig.flags[0] = 0;
    h2bmsg.data.trig.flags[1] = 0;
    //len = socket_connection->Write (h2bmsg);
    //len = socket_connection->Read (b2hmsg);

    //int cnt = 0;
    char buf2[128];
    bool ack = false;
    int p;
    while (true) {
        fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
        int numRead = read(0, buf2, 18);
        if (numRead > 0) {
            fprintf(DB3, "Received command: %s\n", buf2);
            switch (buf2[0]) {
            case '1':          // reset
                h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
                h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kBoardReset;
                //len = socket_connection->Write (h2bmsg);
                break;
            case 'a':          // toggle ACK
                h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
                h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kEnableDisableAck;
                ack = !ack;
                h2bmsg.data.ctrl.extra[0] = ack ? 0x1 : 0x0;
                fprintf(DB3, "ACK now: %s\n", ack ? "on" : "off");
                //len = socket_connection->Write (h2bmsg);
                break;
            case 'X':
                h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
                h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kEnableDisableImu;
                h2bmsg.data.ctrl.extra[0] = 0;
                h2bmsg.data.ctrl.extra[1] = uint8_t(TBWireFormat::ImuSource::kImuInvenSense);
                //len = socket_connection->Write (h2bmsg);
                h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
                h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kEnableDisableImu;
                h2bmsg.data.ctrl.extra[0] = 0;
                h2bmsg.data.ctrl.extra[1] = uint8_t(TBWireFormat::ImuSource::kImuBosch);
                //nnlen = socket_connection->Write (h2bmsg);
                exit(0);

            case 'I':          // enable invensense IMU
            case 'i':          // disable invensense IMU
                h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
                h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kEnableDisableImu;
                h2bmsg.data.ctrl.extra[0] = ('I' == buf2[0]) ? 1 : 0;
                h2bmsg.data.ctrl.extra[1] = uint8_t(TBWireFormat::ImuSource::kImuInvenSense);
                //nlen = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'B':          // enable bosch IMU
            case 'b':          // disable bosch IMU
                h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
                h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kEnableDisableImu;
                h2bmsg.data.ctrl.extra[0] = ('B' == buf2[0]) ? 1 : 0;
                h2bmsg.data.ctrl.extra[1] = uint8_t(TBWireFormat::ImuSource::kImuBosch);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                //len = socket_connection->Write (h2bmsg);
            case 'h':          // request info
                h2bmsg.type = TBWireFormat::H2BMsgType::kRequest;
                h2bmsg.data.req.cmd = TBWireFormat::RequestCmdType::kInfoR;
                //len = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'f':          // request firmware
                h2bmsg.type = TBWireFormat::H2BMsgType::kRequest;
                h2bmsg.data.req.cmd = TBWireFormat::RequestCmdType::kFwR;
                //len = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'p':          // set power
            case 'P':          // set power
                h2bmsg.type = TBWireFormat::H2BMsgType::kControl;
                h2bmsg.data.ctrl.cmd = TBWireFormat::ControlCmdType::kPower;
                if (0 == sscanf((char *) buf2, "p:%x", &p)) {
                    ('p' == buf2[0]) ? p = 0x00 : p = 0xfff;
                }
                printf("power=[%d]\r\n", p);
                h2bmsg.data.ctrl.extra[0] = (int8_t) (p & 0xFF);
                h2bmsg.data.ctrl.extra[1] = (int8_t) ((p & 0xFF00) >> 8);
                //len = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'm':          // set trigger main slam side left
            case 'M':          // set trigger main slam side left
                h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
                h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
                h2bmsg.data.trig.channel = 1;
                h2bmsg.data.trig.master = 1;
                ('m' == buf2[0]) ? h2bmsg.data.trig.periodNs = 40000000 : h2bmsg.data.trig.periodNs = 5000000;
                h2bmsg.data.trig.offsetNs = 0;
                h2bmsg.data.trig.activeTimeNs = 1000000;
                //len = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case '8':          // Resolution
                h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
                h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerResolution;
                h2bmsg.data.trig.extra[0] = 0x10;
                h2bmsg.data.trig.extra[1] = 0x27;
                //len = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case '9':          // Resolution
                h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
                h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerResolution;
                h2bmsg.data.trig.extra[0] = 0x88;
                h2bmsg.data.trig.extra[1] = 0x13;
                //nnlen = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'c':          // CenterCam
            case 'C':          // CenterCam
                h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
                h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
                h2bmsg.data.trig.channel = 4;
                h2bmsg.data.trig.master = 1;
                ('c' == buf2[0]) ? h2bmsg.data.trig.periodNs = 20000000 : h2bmsg.data.trig.periodNs = 10000000;
                h2bmsg.data.trig.offsetNs = 0;
                h2bmsg.data.trig.activeTimeNs = 500000;
                //len = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'r':          // set trigger slam side right
            case 'R':          // set trigger slam side right
                h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
                h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
                h2bmsg.data.trig.channel = 8;
                h2bmsg.data.trig.master = 1;
                ('r' == buf2[0]) ? h2bmsg.data.trig.periodNs = 40000000 : h2bmsg.data.trig.periodNs = 1000000;
                h2bmsg.data.trig.offsetNs = 0;
                h2bmsg.data.trig.activeTimeNs = 1000000;
                //len = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'y':          // eye tracking left
            case 'Y':          // eye tracking left
                h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
                h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
                h2bmsg.data.trig.channel = 2;
                h2bmsg.data.trig.master = 1;
                ('y' == buf2[0]) ? h2bmsg.data.trig.periodNs = 40000000 : h2bmsg.data.trig.periodNs = 1000000;
                h2bmsg.data.trig.offsetNs = 0;
                h2bmsg.data.trig.activeTimeNs = 1000000;
                //len = socket_connection->Write (h2bmsg);
                //   len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'z':          // eye tracking right
            case 'Z':          // eye tracking right
                h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
                h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
                h2bmsg.data.trig.channel = 9;
                h2bmsg.data.trig.master = 1;
                ('z' == buf2[0]) ? h2bmsg.data.trig.periodNs = 40000000 : h2bmsg.data.trig.periodNs = 1000000;
                h2bmsg.data.trig.offsetNs = 0;
                h2bmsg.data.trig.activeTimeNs = 1000000;
                //len = socket_connection->Write (h2bmsg);
                // len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'v':          // RGB Left
            case 'V':          // RGB Left
                h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
                h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
                h2bmsg.data.trig.channel = 5;
                h2bmsg.data.trig.master = 1;
                ('v' == buf2[0]) ? h2bmsg.data.trig.periodNs = 20000000 : h2bmsg.data.trig.periodNs = 10000000;
                h2bmsg.data.trig.offsetNs = 0;
                h2bmsg.data.trig.activeTimeNs = 500000;
                //len = socket_connection->Write (h2bmsg);
                //len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            case 'w':          // RGB Right
            case 'W':          // RGB Right
                h2bmsg.type = surreal::TBWireFormat::H2BMsgType::kTrigger;
                h2bmsg.data.trig.cmd = surreal::TBWireFormat::TriggerCmdType::kTriggerConfig;
                h2bmsg.data.trig.channel = 11;
                h2bmsg.data.trig.master = 1;
                ('w' == buf2[0]) ? h2bmsg.data.trig.periodNs = 20000000 : h2bmsg.data.trig.periodNs = 10000000;
                h2bmsg.data.trig.offsetNs = 0;
                h2bmsg.data.trig.activeTimeNs = 500000;
                //len = socket_connection->Write (h2bmsg);
                //   len = asio::write(socket_, asio::buffer(h2bmsg.bytes), asio::transfer_exactly(18), error);
                break;
            }
        }

        fprintf(DB4, "IN [ ");
        //len = socket_connection->Read (b2hmsg);
        if (len > 0) {
            if (b2hmsg.type == TBWireFormat::B2HMsgType::kImuM) {
                //      continue ;
            }
        }


        for (int i = 0; i < (int) len; i++) {
            fprintf(DB4, "%s%02X%c", KWHT, (unsigned int) ((unsigned char *) (&b2hmsg))[i], ((i == (int) (len - 1)) ? (char) ' ' : (char) ' '));
        }
        fprintf(DB4, " ] %06x %02x\r\n", db1++, (unsigned int) len);

        if (len != 18) {
            printf("															not 18 [%d]\r\n", (int) len);
            continue;
        }

        if (len > 0) {
            switch (b2hmsg.type) {
            case TBWireFormat::B2HMsgType::kInfoM:
                fprintf(DB3, "info [ ");
                for (int i = 0; i < (int) len; i++) {
                    fprintf(DB3, "%s%02X%c", KWHT, (unsigned int) ((unsigned char *) (&b2hmsg))[i], ((i == (int) (len - 1)) ? (char) ' ' : (char) ' '));
                }

                fprintf(DB3, " ] %06x %02x  protoV[%d], v[%c.%c.%c] Serial[%d]\r\n", db1++, (unsigned int) len, (int) b2hmsg.data.info.protoVer, (char) b2hmsg.data.info.ver[0], (char) b2hmsg.data.info.ver[1], (char) b2hmsg.data.info.ver[2], b2hmsg.data.info.serial.msb << 16 | b2hmsg.data.info.serial.lss);
                break;
            case TBWireFormat::B2HMsgType::kPtpM:
                fprintf(DB3, "ptp\n");
                break;
            case TBWireFormat::B2HMsgType::kFwM:
                fprintf(DB3, "fw   [ ");
                for (int i = 0; i < (int) len; i++) {
                    fprintf(DB3, "%s%02X%c", KWHT, (unsigned int) ((unsigned char *) (&b2hmsg))[i], ((i == (int) (len - 1)) ? (char) ' ' : (char) ' '));
                }
                fprintf(DB3, " ] %06x %02x  major[%d], minor[%d] sha[%c%c%c%c%c]\r\n", db1++, (unsigned int) len, (int) b2hmsg.data.fw.major, (char) b2hmsg.data.fw.minor, (char) b2hmsg.data.fw.sha[0], (char) b2hmsg.data.fw.sha[1], (char) b2hmsg.data.fw.sha[2], (char) b2hmsg.data.fw.sha[3], (char) b2hmsg.data.fw.sha[4]);
                break;
            case TBWireFormat::B2HMsgType::kImuM:
                //          break ;
                fprintf(DB3, "source [%9s] ", b2hmsg.data.imu.source.id == 1 ? "Inven" : "Bosch");
                switch (TBWireFormat::ImuSensorIdType(b2hmsg.data.imu.source.sensor)) {
                case TBWireFormat::ImuSensorIdType::kGyro:
                    fprintf(DB3, "g[%6hd,%6hd,%6hd]\n", b2hmsg.data.imu.val.g.x, b2hmsg.data.imu.val.g.y, b2hmsg.data.imu.val.g.z);
                    break;
                case TBWireFormat::ImuSensorIdType::kAcc:
                    static long errors = 0;
                    static long prev_ns = 0;
                    static long delta_ns = 0;
                    static long prev_ns2 = 0;
                    static long delta_ns2 = 0;
                    if (b2hmsg.data.imu.source.id == 1) {
                        if (b2hmsg.time.ns > prev_ns) {
                            delta_ns = abs((b2hmsg.time.ns - prev_ns) - 1000000);
                        } else {
                            delta_ns = abs((b2hmsg.time.ns + (1000000000 - prev_ns)) - 1000000);
                        }
                        prev_ns = b2hmsg.time.ns;

                        fprintf(DB3, "a[%6hd,%6hd,%6hd] %d	%d	%ld ns	%s %ld\n", b2hmsg.data.imu.val.a.x, b2hmsg.data.imu.val.a.y, b2hmsg.data.imu.val.a.z, b2hmsg.time.sls, b2hmsg.time.ns, delta_ns, delta_ns < 20000 ? "" : "ERROR", errors);
                        delta_ns > 20000 ? errors++ : errors;
                    } else {
                        if (b2hmsg.time.ns > prev_ns2) {
                            delta_ns2 = abs((b2hmsg.time.ns - prev_ns2) - 1250000);
                        } else {
                            delta_ns2 = abs((b2hmsg.time.ns + (1000000000 - prev_ns2)) - 1250000);
                        }
                        prev_ns2 = b2hmsg.time.ns;

                        fprintf(DB3, "a[%6hd,%6hd,%6hd] %d	%d	%ld ns	%s %ld\n", b2hmsg.data.imu.val.a.x, b2hmsg.data.imu.val.a.y, b2hmsg.data.imu.val.a.z, b2hmsg.time.sls, b2hmsg.time.ns, delta_ns2, delta_ns2 < 20000 ? "" : "ERROR", errors);
                        delta_ns2 > 20000 ? errors++ : errors;
                    }
                    break;
                case TBWireFormat::ImuSensorIdType::kMag:
                    fprintf(DB3, "m[%6hd,%6hd,%6hd]\n", b2hmsg.data.imu.val.m.x, b2hmsg.data.imu.val.m.y, b2hmsg.data.imu.val.m.z);
                    break;
                case TBWireFormat::ImuSensorIdType::kPress:
                    fprintf(DB3, "p[%6d]\n", b2hmsg.data.imu.val.p.integer_);
                    break;
                case TBWireFormat::ImuSensorIdType::kTemp:
                    fprintf(DB3, "t[%6d]\n", b2hmsg.data.imu.val.t.integer);
                    break;
                default:
                    fprintf(DB3, "unknown imu sensor type %d.\n", b2hmsg.data.imu.source.sensor);
                    break;
                }
                break;
            case TBWireFormat::B2HMsgType::kSyncM:
                if (b2hmsg.data.sync.channel == 5) {
                    fprintf(DB3, "											SYNC [     55555555555555555555\r\n ");
                }
                static long prev3 = 0;
                if (b2hmsg.data.sync.channel < 8) {
                    //printf(".") ;
                    //m_io ^= (0x01 << b2hmsg.data.sync.channel);
                    m_io ^= (0x01 << 0);
                    outb(m_io, PARALLEL_PORT_DATA);
                    {
                        for (int i = 0; i < 1000; i++) {
                            // Do nothing
                        }
                    }
                    //m_io ^= (0x01 << b2hmsg.data.sync.channel);
                    m_io ^= (0x01 << 0);
                    outb(m_io, PARALLEL_PORT_DATA);
                }
                fprintf(DB3, "								SYNC [ ");
                fprintf(DB3, " %d	%d	%d	%ldms\n", b2hmsg.data.sync.channel, b2hmsg.time.sls, b2hmsg.time.ns, (b2hmsg.time.ns - prev3) / 1000000);
                prev3 = b2hmsg.time.ns;
                //for (int i = 0;  i < (int)len; i++)
                //{
                //  fprintf(DB3, "%s%02X%c", KWHT, (unsigned int)((unsigned char *)(&buf))[i], ((i == (int)(len - 1)) ? (char)' ' : (char)' '));
                //}
                //fprintf(DB3, " ] %06x %02x\r\n", db1++, (unsigned int)len);
                break;
            case TBWireFormat::B2HMsgType::kTrigM:
                fprintf(DB3, "trigger\n");
                break;
            case TBWireFormat::B2HMsgType::kEpochM:
                fprintf(DB3, "epoch\n");
                break;
            case TBWireFormat::B2HMsgType::kAckM:
                fprintf(DB3, "ACK [ ");
                for (int i = 0; i < (int) len; i++) {
                    fprintf(DB3, "%s%02X%c", KWHT, (unsigned int) ((unsigned char *) (&b2hmsg))[i], ((i == (int) (len - 1)) ? (char) ' ' : (char) ' '));
                }
                fprintf(DB3, " ] %06x %02x\r\n", db1++, (unsigned int) len);
                break;
            default:
                fprintf(DB3, "something else %u\n", uint8_t(b2hmsg.type));
                fprintf(DB3, "IN [ ");
                for (int i = 0; i < (int) len; i++) {
                    fprintf(DB3, "%s%02X%c", KWHT, (unsigned int) ((unsigned char *) (&b2hmsg))[i], ((i == (int) (len - 1)) ? (char) ' ' : (char) ' '));
                }
                fprintf(DB3, " ] %06x %02x\r\n", db1++, (unsigned int) len);
                break;
            }
        }
        //usleep(100000);
    }
}
