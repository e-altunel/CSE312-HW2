#ifndef INC_DEFINES
#define INC_DEFINES

namespace FileSys
{
using uint64_t = long unsigned int;
using uint32_t = unsigned int;
using uint16_t = unsigned short int;
using uint8_t = unsigned char;

using ubyte = uint8_t;

using int64_t = long int;
using int32_t = int;
using int16_t = short int;
using int8_t = signed char;

using byte = uint8_t;

constexpr int64_t FULL_BLOCK_SIZE = 128;
constexpr int64_t HALF_BLOCK_SIZE = FULL_BLOCK_SIZE / 2;

static_assert (FULL_BLOCK_SIZE % 2 == 0,
               "FULL_BLOCK_SIZE is not divisible by 2");

static_assert (sizeof (uint64_t) == 8, "uint64_t is not 8 bytes");
static_assert (sizeof (uint32_t) == 4, "uint32_t is not 4 bytes");
static_assert (sizeof (uint16_t) == 2, "uint16_t is not 2 bytes");
static_assert (sizeof (uint8_t) == 1, "uint8_t is not 1 byte");
static_assert (sizeof (ubyte) == 1, "ubyte is not 1 byte");

static_assert (sizeof (int64_t) == 8, "int64_t is not 8 bytes");
static_assert (sizeof (int32_t) == 4, "int32_t is not 4 bytes");
static_assert (sizeof (int16_t) == 2, "int16_t is not 2 bytes");
static_assert (sizeof (int8_t) == 1, "int8_t is not 1 byte");
static_assert (sizeof (byte) == 1, "byte is not 1 byte");

} // namespace FileSys

#endif /* INC_DEFINES */
