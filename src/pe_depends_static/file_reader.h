#ifndef PE_DEPENDS_FILE_READER_H
#define PE_DEPENDS_FILE_READER_H

#include "status.h"
#include "util/str.h"

#include <windows.h>

enum status read_u16_at(HFILE file, INT64 pos, UINT16 * result);
enum status read_u32_at(HFILE file,INT64 pos, UINT32 * result);
enum status read_string_at(HFILE file, INT64 pos, char * * result);

#endif
