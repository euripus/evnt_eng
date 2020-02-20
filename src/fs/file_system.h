#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../assets/assetmanager.h"
#include "file.h"
#include <functional>
#include <list>

namespace evnt
{
class FileSystem
{
public:
    FileSystem(std::string root_dir);
    virtual ~FileSystem() = default;

    bool   isExist(const std::string & fname) const;
    InFile getFile(const std::string & fname) const;   // ex. file name: "fonts/times.ttf"
    size_t getNumFiles() const { return m_files.size(); }

    bool writeFile(const std::string & path, BaseFile const * file);   // Memory file
    bool createZIP(std::vector<BaseFile const *> filelist,
                   const std::string &           zipname);   // all zip files saves in root directory
    bool addFileToZIP(BaseFile const * file, const std::string & zipname);

    static std::string GetTempDir();
    static std::string GetCurrentDir();
    static std::string GetTempFileName();

private:
    struct file_data
    {
        struct ZFileData
        {
            bool        compressed       = false;
            size_t      compressedSize   = 0;
            size_t      uncompressedSize = 0;
            size_t      lfhOffset        = 0;
            std::string fname;
        };

        bool        is_zip = false;
        ZFileData   zip_data;
        std::string fname;
    };

    void   addZippedDir(const std::string & fname);
    InFile loadRegularFile(const file_data & f) const;
    InFile loadZipFile(const file_data & zf) const;

    std::list<file_data> m_files;
    std::string          m_data_dir;
};
}   // namespace evnt

#endif   // FILESYSTEM_H
