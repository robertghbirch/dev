#define MAX_MSG_TO_DEVICE_SIZE 255
#define MAX_MSG_TO_HOST_SIZE 255
#define MAX_MSG_TO_DEVICE_SIZE_TEMP 2048

size_t StuffData(const uint8_t *ptr, size_t length, uint8_t *dst);
size_t UnStuffData(const uint8_t *ptr, size_t length, uint8_t *dst);

static size_t read_buffer(unsigned max_length, uint8_t * out, uint32_t * frame_len, uint32_t * frame_crc);
static size_t write_buffer(uint8_t * out, uint32_t * frame_len, uint32_t * frame_crc);
