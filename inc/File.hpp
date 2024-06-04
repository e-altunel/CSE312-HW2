#ifndef INC_FILE
#define INC_FILE

#include <string>
#include <vector>
#include <Blocks.hpp>
#include <FilesystemElement.hpp>

namespace FileSystem {

class File : public FileSystemElement {
    public:
	File(const std::string &name,
	     std::size_t size,
	     bool readPermission,
	     bool writePermission,
	     uint64_t creationTime,
	     const std::string &content);
	virtual ~File();

    private:
	std::vector<Block<1024> *> blocks;
};

}

#endif /* INC_FILE */
