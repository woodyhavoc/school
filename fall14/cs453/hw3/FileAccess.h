/**
 * Author: Aaron Woods
 */

#ifndef __FILEACCESS_H
#define __FILEACCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void FileAccess(int n);

void* startAccess(void* obj);

void endAccess(int threadID);

#endif /* __FILEACCESS_H */
