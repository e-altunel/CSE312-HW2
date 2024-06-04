#ifndef INC_FOLDER
#define INC_FOLDER

#include <string>
#include <vector>
#include <memory>
#include <FilesystemElement.hpp>

namespace FileSystem {

class Folder : public FileSystemElement {
    public:
	Folder(const std::string &name,
	       std::size_t size,
	       bool readPermission,
	       bool writePermission,
	       uint64_t creationTime,
	       const std::string &content);
	virtual ~Folder();

    private:
	std::vector<std::shared_ptr<FileSystemElement> > children;
};

}

#endif /* INC_FOLDER */
