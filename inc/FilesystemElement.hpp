#ifndef INC_FILESYSTEMELEMENT
#define INC_FILESYSTEMELEMENT

#include <string>

namespace FileSystem {

class FileSystemElement {
    public:
	FileSystemElement(const std::string &name,
			  std::size_t size,
			  bool readPermission,
			  bool writePermission,
			  uint64_t creationTime);
	virtual ~FileSystemElement();

    private:
	std::string name;
	std::size_t size;
	bool readPermission;
	bool writePermission;
	uint64_t creationTime;
};

}

#endif /* INC_FILESYSTEMELEMENT */
