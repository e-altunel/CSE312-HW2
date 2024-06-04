#include <Folder.hpp>

namespace FileSystem {

Folder::Folder(const std::string &name,
	       std::size_t size,
	       bool readPermission,
	       bool writePermission,
	       uint64_t creationTime,
	       const std::string &content)
	: FileSystemElement(name, size, readPermission, writePermission, creationTime), children() {
}

Folder::~Folder() {
}
}
