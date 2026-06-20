#include "directoryvalidator.h"


bool DirectoryValidator::validateDirectory(const QString &path, int flags) {
    QFileInfo fileInfo(path);

    // Check if path exists
    if (flags & Exists && !fileInfo.exists()) {
        qDebug() << "Directory does not exist:" << path;
        return false;
    }

    // Check if it's a directory
    if (flags & IsDirectory && !fileInfo.isDir()) {
        qDebug() << "Path is not a directory:" << path;
        return false;
    }

    // Check if readable
    if (flags & Readable && !fileInfo.isReadable()) {
        qDebug() << "Directory is not readable:" << path;
        return false;
    }

    // Check if writable
    if (flags & Writable && !fileInfo.isWritable()) {
        qDebug() << "Directory is not writable:" << path;
        return false;
    }

    // Check if executable
    if (flags & Executable && !fileInfo.isExecutable()) {
        qDebug() << "Directory is not executable:" << path;
        return false;
    }

    // Check if absolute path
    if (flags & IsAbsolute && !fileInfo.isAbsolute()) {
        qDebug() << "Directory path is not absolute:" << path;
        return false;
    }

    // Check if directory is empty
    if (flags & NotEmpty) {
        QDir dir(path);
        if (dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries).isEmpty()) {
            qDebug() << "Directory is empty:" << path;
            return false;
        }
    }

    return true;
}

bool DirectoryValidator::ensureDirectoryExists(const QString &path, bool createParents) {
    QDir dir(path);
    if (dir.exists()) {
        return true;
    }

    if (createParents) {
        return dir.mkpath(".");
    } else {
        return dir.mkdir(".");
    }
}

bool DirectoryValidator::validateOrCreateDirectory(const QString &path, int flags) {
    if (validateDirectory(path, flags)) {
        return true;
    }

    // Try to create the directory
    if (ensureDirectoryExists(path)) {
        return validateDirectory(path, flags);
    }

    return false;
}
