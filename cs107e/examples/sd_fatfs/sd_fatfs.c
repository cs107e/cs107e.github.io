/*
 * Simple client example using fatfs to access filesystem on SD card
 */
#include "printf.h"
#include "strings.h"
#include "uart.h"
#include "ff.h"

static const char *name_of(FILINFO *f)
{
    return ((f->fattrib & AM_LFN) || (f->lfname && *f->lfname)) ? f->lfname : f->fname;
}

static bool is_dir(FILINFO *f)
{
   return f->fattrib & AM_DIR;
}

static void print_fileinfo(FILINFO *f, const char *dir)
{
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int year = f->fdate >> 10;
    int mon = (f->fdate >> 5) & 0xf;
    int day = f->fdate & 0x1f;
    int hour = f->ftime >> 11;
    int min = (f->ftime >> 5) & 0x3f;
    //int sec = f->ftime & 0x3f;

    printf("%8d %s %2d 20%02d %2d:%02d %s/%s%s\n", 
        (int)f->fsize, months[mon-1], day, year, hour, min,  dir, name_of(f), is_dir(f) ? "/" :"");
}

static void test_read_write(const char *fname)
{
    FRESULT res;

    // create a test file in the root directory
    // see http://elm-chan.org/fsw/ff/doc/open.html
    // for information about the f_open function
    printf("Creating file %s.\n", fname); 
    FIL fp;
    res = f_open(&fp, fname, FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) {
        printf("Could not create file %s. Error: %s\n",fname, ff_err_msg(res));
        return;
    }

    // write something to the file
    // see http://elm-chan.org/fsw/ff/doc/write.html
    // for information about the f_write function
    char writebuf[] = "Shall we play a game?\nLove to. How about Global Thermonuclear War?\n";
    unsigned int nwritten;
    res = f_write(&fp, writebuf, sizeof writebuf, &nwritten);
    if (res != FR_OK || nwritten != sizeof writebuf) {
        printf("Could not write to file %s. Error: %s\n", fname, ff_err_msg(res));
        f_close(&fp);
        return;
    }
    printf("Wrote %d bytes to file %s:\n---\n%s---\n", nwritten, fname, writebuf); 
    f_close(&fp);   // close file (should commit to media)

    // we are going to read the entire file, so we can use the f_stat function 
    // to get the file length, and we can also use this to determine if the file exists
    // see http://elm-chan.org/fsw/ff/doc/stat.html
    // for information about the f_stat function
    // The file information struct details can be seen at
    // http://elm-chan.org/fsw/ff/doc/sfileinfo.html
    FILINFO fi;
    res = f_stat(fname,&fi);
    if (res != FR_OK) {
        printf("Could not stat file %s. Error: %s\n", fname, ff_err_msg(res));
        return;
    } else {
        printf("ls %s (file size should be %d bytes, date/time is bunk)\n", fname, nwritten);
        print_fileinfo(&fi, "");
    }

    // open the file up again to read it
    res = f_open(&fp, fname, FA_READ);
    if (res != FR_OK) {
        printf("Could not open file %s for reading. Error: %s\n",fname, ff_err_msg(res));
        return;
    }
    // read the entire file into a buffer
    // see http://elm-chan.org/fsw/ff/doc/read.html
    // for information about the f_read function
    char readbuf[sizeof writebuf];
    unsigned int nread;
    res = f_read(&fp, readbuf, sizeof readbuf, &nread);
    if (res != FR_OK || nread != sizeof readbuf) {
        printf("Could not read from file %s. Error: %s\n",fname, ff_err_msg(res));
        return;
    }
    printf("Read %d bytes from file %s:\n---\n%s---\n", nread, fname, readbuf); 

    // remove the file
    // see http://elm-chan.org/fsw/ff/doc/unlink.html
    // for information about the f_unlink function
    res = f_unlink(fname);
    if (res != FR_OK) {
        printf("Could not remove file %s. Error: %s\n",fname, ff_err_msg(res));
        return;
    }
    printf("Removed file %s.\n",fname);
}

static int recursive_scan(char* path)
{
    FRESULT res;
    DIR dir;
    FILINFO fi;
    int count = 0;

    char lfnbuf[128];  // used to capture long file name
    fi.lfsize = sizeof lfnbuf;
    fi.lfname = lfnbuf;

    res = f_opendir(&dir, path);    
    if (res != FR_OK) {
        printf("opendir error %s\n", ff_err_msg(res));
        return -1;
    }

    while (1) {
        res = f_readdir(&dir, &fi);
        if (res != FR_OK) {
            printf("readdir error %s\n", ff_err_msg(res));
            return -1;
        }
        if (fi.fname[0] == 0) break;  // no more entries
        if (name_of(&fi)[0] == '.') continue; // skip hidden

        if (is_dir(&fi)) {
            char full_path[1024];
            snprintf(full_path, sizeof full_path, "%s/%s", path, fi.fname);
            count += recursive_scan(full_path);                
        } else {
            print_fileinfo(&fi, path);
            count++;            
        }
    }
    f_closedir(&dir);
    return count;
}


void main(void) 
{
    uart_init();
    printf("Starting libpisd.a test\n");

    FATFS fs;
    FRESULT res = f_mount(&fs, "", 1);
    if (res != FR_OK) {
        printf("Could not mount internal SD card. Error: %s\n", ff_err_msg(res));
        return;
    }

    int n = recursive_scan(""); // start at root
    printf("Scan found %d entries.\n\n", n);

    test_read_write("curiously_long_filename.txt");

    uart_putchar(EOT);
}
