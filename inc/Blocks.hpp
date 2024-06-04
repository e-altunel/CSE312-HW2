#ifndef INC_BLOCKS
#define INC_BLOCKS

#include <cstddef>

namespace FileSystem {

template <std::size_t N> struct Block {
	uint8_t data[N];
};

}

#endif /* INC_BLOCKS */
