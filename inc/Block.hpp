#ifndef INC_BLOCK
#define INC_BLOCK

#include <Defines.hpp>
#include <fstream>

namespace FileSys
{

class SuperBlock
{
public:
  enum BlockType
  {
    HalfBlock,
    FullBlock
  };

  struct BlockInfo
  {
    BlockType type : 1;
    uint32_t nextBlock : 31;
  };

  struct Block
  {
    BlockInfo info;
    byte data[FULL_BLOCK_SIZE - sizeof (BlockInfo)];
  };

private:
  Block m_currentBlock;
  bool m_hasBlock;
  bool m_blockChanged;
  uint32_t m_currentBlockIndex;
  const char *m_path;
  byte *m_block_bitmap;
  uint32_t halfBlockCount;
  std::fstream file;

public:
  SuperBlock (const char *path, uint32_t fullBlockCount);
  SuperBlock (const SuperBlock &) = delete;
  SuperBlock &operator= (const SuperBlock &) = delete;
  ~SuperBlock ();

  void writeToBlock (uint32_t blockIndex, const byte *data, uint32_t size,
                     uint32_t offset = 0);
  void readFromBlock (uint32_t blockIndex, byte *data, uint32_t size,
                      uint32_t offset = 0);
  void initializeBlock (uint32_t blockIndex, BlockType type);
  void freeBlock (uint32_t blockIndex);
  void close ();

private:
  void loadActualBlock (uint32_t index);
  void loadBlock (uint32_t index);
  void saveBlock ();
  void clearBlock ();
};

}

#endif /* INC_BLOCK */
