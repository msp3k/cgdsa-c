/* I/O Routines
 * by: Michael Peek
 * Date: Wed, 13 Apr 2016 21:07:15 -0400
 *
 * License:
 *
 * Copyright (c) 2014, Michael Peek <peek3000@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 		* Redistributions of source code must retain the above copyright
 * 			notice, this list of conditions and the following disclaimer.
 * 		* Redistributions in binary form must reproduce the above copyright
 * 			notice, this list of conditions and the following disclaimer in the
 * 			documentation and/or other materials provided with the distribution.
 * 		* The name of the author may not be used to endorse or promote products
 * 		  derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */

#ifndef __IO_H__
#define __IO_H__

#include <stdbool.h>

//---[ Data Structures ]-----------------------------------------------------

//---[ Supporting Functions ]------------------------------------------------

//---[ I/O Functions ]-------------------------------------------------------

/** A routine to test if a file descriptor is ready to be written to
 *
 * Example:
 *
 * @code
 * if (write_ready(fd)) {
 *  // fd is ready to receive data
 *  char * msg = "Hello World";
 *  if (write(fd, msg, strlen(msg)) != strlen(msg)) {
 *   // Incomplete write
 *  }
 *  else {
 *   // All data written successfully
 *  }
 * }
 * @endcode
 */
bool write_ready(int fd);

/** A routine to test if a file descriptor is ready to be read from
 *
 * Example:
 *
 * @code
 * if (read_ready(fd)) {
 *  // fd is ready to give data
 *  char * buf[4096] = { 0 };
 *  if (read(fd, msg, 4095) < 0) {
 *   // Incomplete read
 *  }
 *  else {
 *   // Data read successfully
 *  }
 * }
 * @endcode
 */
bool read_ready(int fd);

#endif

