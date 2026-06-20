#ifndef DIRECTORYVALIDATOR_H
#define DIRECTORYVALIDATOR_H

#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <Models/global_qcardio.h>
class DirectoryValidator {
public:


    static bool validateDirectory(const QString& path, int flags = Exists | Readable | IsDirectory);

    // Create directory if it doesn't exist
    static bool ensureDirectoryExists(const QString& path, bool createParents = true);

    // Validate and create if needed
    static bool validateOrCreateDirectory(const QString& path, int flags = Exists | Readable | Writable | IsDirectory);
};

#endif // DIRECTORYVALIDATOR_H
