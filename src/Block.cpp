#include <Block.hpp>
#include <Logger.hpp>
#include <fstream>
#include <stdexcept>

using namespace LoggerNS;

namespace FileSys
{

SuperBlock::SuperBlock (const char *path, uint32_t fullBlockCount)
    : m_hasBlock (false), m_blockChanged (false), m_currentBlockIndex (-1),
      m_path (path), halfBlockCount (fullBlockCount * 2)
{
  FUNC_LOG ();
  m_block_bitmap = new byte[halfBlockCount];
  for (uint32_t i = 0; i < halfBlockCount; ++i)
    m_block_bitmap[i] = 0;

  file.open (m_path, std::ios::in);
  if (!file.fail ())
  {
    debugLogger << Spec::START << "File exists" << Spec::ENDL;
    file.close ();
    return;
  }
  debugLogger << Spec::START << "File does not exist" << Spec::ENDL;

  file.open (m_path, std::ios::out | std::ios::binary);
  if (!file.is_open ())
    throw std::runtime_error ("Failed to create file");

  Block empty = { BlockInfo{ BlockType::HalfBlock, 0 }, { 0 } };
  for (uint32_t i = 0; i < halfBlockCount / 2; ++i)
  {
    file.write (reinterpret_cast<char *> (&empty), sizeof (empty));
    if (!file)
      throw std::runtime_error ("Failed to write block");
  }
  file.close ();
  file
      = std::fstream (m_path, std::ios::in | std::ios::out | std::ios::binary);
  if (!file.is_open ())
    throw std::runtime_error ("Failed to open file");
}

SuperBlock::~SuperBlock ()
{
  FUNC_LOG ();
  if (m_hasBlock)
    saveBlock ();
  if (file.is_open ())
    file.close ();
  if (m_block_bitmap)
    delete[] m_block_bitmap;
}

void
SuperBlock::writeToBlock (uint32_t blockIndex, const byte *data, uint32_t size,
                          uint32_t offset)
{
  FUNC_LOG ();
  if (blockIndex >= halfBlockCount)
    throw std::out_of_range ("Block index out of range");
  loadActualBlock (blockIndex);
  if (size + offset > (m_currentBlock.info.type == BlockType::HalfBlock
                           ? HALF_BLOCK_SIZE
                           : FULL_BLOCK_SIZE))
    throw std::invalid_argument ("Data size is too large");

  for (uint32_t i = 0; i < size; ++i)
    m_currentBlock.data[i + offset] = data[i];
  m_blockChanged = true;
}

void
SuperBlock::readFromBlock (uint32_t blockIndex, byte *data, uint32_t size,
                           uint32_t offset)
{
  FUNC_LOG ();
  if (blockIndex >= halfBlockCount)
    throw std::out_of_range ("Block index out of range");
  loadBlock (blockIndex);
  if (size + offset > (m_currentBlock.info.type == BlockType::HalfBlock
                           ? HALF_BLOCK_SIZE
                           : FULL_BLOCK_SIZE))
    throw std::invalid_argument ("Data size is too large");

  for (uint32_t i = 0; i < size; ++i)
    data[i] = m_currentBlock.data[i + offset];
}

void
SuperBlock::initializeBlock (uint32_t blockIndex, BlockType type)
{
  FUNC_LOG ();
  if (m_hasBlock)
    saveBlock ();
  if (blockIndex >= halfBlockCount)
    throw std::out_of_range ("Block index out of range");
  if (type == BlockType::FullBlock && blockIndex % 2 == 1)
    throw std::invalid_argument ("Full block must be at even index");
  if (m_block_bitmap[blockIndex] != 0)
    throw std::invalid_argument ("Block is already initialized");

  m_block_bitmap[blockIndex] = 1;
  if (type == BlockType::FullBlock)
    m_block_bitmap[blockIndex + 1] = 2;
  m_currentBlock.info.type = type;
  m_currentBlock.info.nextBlock = 0;
  m_blockChanged = true;
  m_currentBlockIndex = blockIndex;
  m_hasBlock = true;
  clearBlock ();
}

void
SuperBlock::freeBlock (uint32_t blockIndex)
{
  FUNC_LOG ();
  if (blockIndex >= halfBlockCount)
    throw std::out_of_range ("Block index out of range");
  if (m_block_bitmap[blockIndex] == 0)
    throw std::invalid_argument ("Block is already free");

  m_block_bitmap[blockIndex] = 0;
  m_blockChanged = true;
}

void
SuperBlock::close ()
{
  FUNC_LOG ();
  if (m_hasBlock)
    saveBlock ();
  if (file.is_open ())
    file.close ();
}

void
SuperBlock::loadActualBlock (uint32_t index)
{
  FUNC_LOG ();
  if (index >= halfBlockCount)
    throw std::out_of_range ("Block index out of range");
  if (m_currentBlockIndex == index)
    return;
  if (m_block_bitmap[index] == 0)
    throw std::invalid_argument ("Block is not initialized");
  if (m_block_bitmap[index] == 2)
    throw std::invalid_argument ("Block is a full block");
  loadBlock (index);
}

void
SuperBlock::loadBlock (uint32_t index)
{
  FUNC_LOG ();
  if (m_hasBlock)
    saveBlock ();
  if (index >= halfBlockCount)
    throw std::out_of_range ("Block index out of range");
  if (m_currentBlockIndex == index)
    return;

  m_currentBlockIndex = index;
  if (!file.is_open ())
    throw std::runtime_error ("Failed to open file");

  file.seekg (index * HALF_BLOCK_SIZE, std::ios::beg);
  file.read (reinterpret_cast<char *> (&m_currentBlock),
             sizeof (m_currentBlock.info));
  if (!file)
    throw std::runtime_error ("Failed to read block");
  file.read (reinterpret_cast<char *> (m_currentBlock.data),
             m_currentBlock.info.type == BlockType::HalfBlock
                 ? HALF_BLOCK_SIZE
                 : FULL_BLOCK_SIZE);

  m_hasBlock = true;
  m_blockChanged = false;
}

void
SuperBlock::saveBlock ()
{
  FUNC_LOG ();
  if (!m_hasBlock)
    return;
  if (!m_blockChanged)
    return;

  if (!file.is_open ())
    throw std::runtime_error ("Failed to open file");

  file.seekp (m_currentBlockIndex * HALF_BLOCK_SIZE, std::ios::beg);
  file.write (reinterpret_cast<char *> (&m_currentBlock),
              m_currentBlock.info.type == BlockType::HalfBlock
                  ? HALF_BLOCK_SIZE
                  : FULL_BLOCK_SIZE);
  if (!file)
    throw std::runtime_error ("Failed to write block");

  m_hasBlock = false;
  m_blockChanged = false;
}

void
SuperBlock::clearBlock ()
{
  FUNC_LOG ();
  debugLogger << Spec::START << "Clearing block" << m_currentBlockIndex
              << (m_currentBlock.info.type == BlockType::HalfBlock ? " (Half)"
                                                                   : " (Full)")
              << Spec::ENDL;
  if (!m_hasBlock)
    return;
  for (uint32_t i = sizeof (m_currentBlock.info);
       i < (m_currentBlock.info.type == BlockType::HalfBlock
                ? HALF_BLOCK_SIZE
                : FULL_BLOCK_SIZE);
       ++i)
    m_currentBlock.data[i - sizeof (m_currentBlock.info)] = 0;
  m_blockChanged = true;
}

} // namespace FileSys