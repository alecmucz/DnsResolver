#pragma once
#include <unistd.h>

class FileDescriptor {
private:
    int fd{-1};

public:
    FileDescriptor() = default;
    explicit FileDescriptor(int fd) : fd(fd) {}

    FileDescriptor(const FileDescriptor&) = delete;             // Copy Constructor (Not Allowed)
    FileDescriptor& operator=(const FileDescriptor&) = delete;  // Copy Assignment Operator (Not Allowed)

    FileDescriptor(FileDescriptor&& other) noexcept : fd(other.fd)  // Move Constructor
    {other.fd = -1;}

    FileDescriptor& operator=(FileDescriptor&& other) noexcept {    // Move Assignment Operator
        if (fd != -1)
            close(fd);

        fd = other.fd;
        other.fd = -1;
        return *this;
    }


    ~FileDescriptor() {     // Destructor (Close the file descriptor)
        if (fd != -1)
            close(fd);
    }

    int value() const {
        return fd;
    }
};
