#include <FilesystemElement.hpp>

namespace FileSystem {

FileSystemElement::FileSystemElement(const std::string &name,
				     std::size_t size,
				     bool readPermission,
				     bool writePermission,
				     uint64_t creationTime)
	: name(name), size(size), readPermission(readPermission), writePermission(writePermission),
	  creationTime(creationTime) {
}

FileSystemElement::~FileSystemElement() {
}

}