#include "config.h"
#include "zipfile.h"
#include "minizip/zip.h"

#include <string>

using namespace std;

ZipFile::ZipFile()
{
    const char *filename_try = "teste.zip";
    int err = 0;

    zipFile zf;
    int errclose;
    zf = zipOpen64(filename_try, 0);

    if (zf == NULL) {
        printf("error opening %s\n", filename_try);
        err = ZIP_ERRNO;
    } else
        printf("creating %s\n", filename_try);


    FILE * fin;
    const char* filenameinzip = "META/teste.txt";
    const char *savefilenameinzip;
    zip_fileinfo zi;

    savefilenameinzip = filenameinzip;
    while (savefilenameinzip[0] == '\\' || savefilenameinzip[0] == '/') {
        savefilenameinzip++;
    }

    err = zipOpenNewFileInZip(zf, savefilenameinzip, &zi,
            NULL, 0, NULL, 0, NULL, Z_DEFLATED,
            Z_DEFAULT_COMPRESSION);

    if (err != ZIP_OK)
        printf("error in opening %s in zipfile\n", filenameinzip);
    
    if (err == ZIP_OK) {
        string test = "buffer test";
        err = zipWriteInFileInZip(zf, test.c_str(), test.length());
        if (err < 0) {
            printf("error in writing %s in the zipfile\n",
                    filenameinzip);
        }
    }
    
    if (err < 0)
        err = ZIP_ERRNO;
    else {
        err = zipCloseFileInZip(zf);
        if (err != ZIP_OK)
            printf("error in closing %s in the zipfile\n",
                filenameinzip);
    }

    errclose = zipClose(zf, NULL);
    if (errclose != ZIP_OK)
        printf("error in closing %s\n", filename_try);
}

ZipFile::~ZipFile()
{

}
