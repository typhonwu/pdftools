#include "config.h"
#include "zipfile.h"
#include "minizip/zip.h"

#include <string>
#include <iostream>

using namespace std;

ZipFile::ZipFile()
{
    const char *filename_try = "teste.zip";
    int err = 0;

    zipFile zf;
    int errclose;
#ifdef USEWIN32IOAPI
    zlib_filefunc_def ffunc;
    fill_win32_filefunc(&ffunc);
    zf = zipOpen2(filename_try, 0, NULL, &ffunc);
#else
    zf = zipOpen(filename_try, 0);
#endif

    if (zf == NULL) {
        cout << "error opening " << filename_try << endl;
        err = ZIP_ERRNO;
    } else
        cout << "creating " << filename_try << endl;

    const char* filenameinzip = "teste/teste.txt";
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
        cout << "error in opening " << filenameinzip << " in zipfile" << endl;

    if (err == ZIP_OK) {
        string test = "buffer test";
        err = zipWriteInFileInZip(zf, test.c_str(), test.length());
        if (err < 0) {
            cout << "error in writing " << filenameinzip << " in the zipfile" << endl;
        }
    }

    if (err < 0)
        err = ZIP_ERRNO;
    else {
        err = zipCloseFileInZip(zf);
        if (err != ZIP_OK)
            cout << "error in closing " << filenameinzip << " in the zipfile" << endl;
    }

    errclose = zipClose(zf, NULL);
    if (errclose != ZIP_OK)
        cout << "error in closing " << filename_try << endl;
}

ZipFile::~ZipFile()
{

}
