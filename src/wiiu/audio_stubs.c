//
// Audio library stubs for missing libraries
// This file provides empty implementations for missing audio library functions
// to allow SDL2_mixer to link without the actual libraries
//

#ifdef __WIIU__

#include <stddef.h>
#include <sys/types.h>

// ModPlug stubs
void ModPlug_Seek(void* file, int millisecond) {}
int ModPlug_Read(void* file, void* buffer, int size) { return 0; }
void* ModPlug_Load(const void* data, int size) { return NULL; }
void ModPlug_Unload(void* file) {}
int ModPlug_SeekOrder(void* file, int order) { return 0; }
int ModPlug_GetLength(void* file) { return 0; }
void ModPlug_GetSettings(void* file, void* settings) {}
void ModPlug_SetSettings(void* file, void* settings) {}
void ModPlug_SetMasterVolume(void* file, int volume) {}
const char* ModPlug_GetName(void* file) { return NULL; }

// mpg123 stubs
int mpg123_exit(void) { return 0; }
int mpg123_close(void* mh) { return 0; }
void mpg123_delete(void* mh) {}
int mpg123_format(void* mh, long rate, int channels, int encodings) { return 0; }
int mpg123_format_none(void* mh) { return 0; }
int mpg123_getformat(void* mh, long* rate, int* channels, int* encoding) { return 0; }
int mpg123_init(void) { return 0; }
void* mpg123_new(const char* decoder, int* error) { return NULL; }
const long* mpg123_rates(void) { return NULL; }
int mpg123_open_handle(void* mh, void* iohandle) { return 0; }
const char* mpg123_plain_strerror(int errcode) { return "mpg123 not available"; }
int mpg123_read(void* mh, unsigned char* outmemory, size_t outmemsize, size_t* done) { return 0; }
int mpg123_replace_reader_handle(void* mh, ssize_t (*r_read)(void*, void*, size_t), off_t (*r_lseek)(void*, off_t, int), void (*cleanup)(void*)) { return 0; }
off_t mpg123_seek(void* mh, off_t sampleoff, int whence) { return 0; }
off_t mpg123_tell(void* mh) { return 0; }
off_t mpg123_length(void* mh) { return 0; }
const char* mpg123_strerror(void* mh) { return "mpg123 not available"; }

// vorbisidec stubs (if needed)
// These would be added if vorbisidec functions are referenced

#endif // __WIIU__
