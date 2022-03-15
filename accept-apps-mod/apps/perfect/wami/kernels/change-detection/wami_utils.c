/* -*-Mode: C;-*- */

/**BeginCopyright************************************************************
 *
 * $HeadURL: https://pastec.gtri.gatech.edu/svn/svn-dpc/INNC/projects/PERFECT-TAV-ES/suite/wami/kernels/lib/wami_utils.c $
 * $Id: wami_utils.c 8546 2014-04-02 21:36:22Z tallent $
 *
 *---------------------------------------------------------------------------
 * Part of PERFECT Benchmark Suite (hpc.pnnl.gov/projects/PERFECT/)
 *---------------------------------------------------------------------------
 *
 * Copyright ((c)) 2014, Battelle Memorial Institute
 * Copyright ((c)) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * 1. Battelle Memorial Institute (hereinafter Battelle) and Georgia Tech
 *    Research Corporation (GTRC) hereby grant permission to any person
 *    or entity lawfully obtaining a copy of this software and associated
 *    documentation files (hereinafter "the Software") to redistribute
 *    and use the Software in source and binary forms, with or without
 *    modification.  Such person or entity may use, copy, modify, merge,
 *    publish, distribute, sublicense, and/or sell copies of the
 *    Software, and may permit others to do so, subject to the following
 *    conditions:
 * 
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimers.
 * 
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 * 
 *    * Other than as used herein, neither the name Battelle Memorial
 *      Institute nor Battelle may be used in any form whatsoever without
 *      the express written consent of Battelle.
 * 
 *      Other than as used herein, neither the name Georgia Tech Research
 *      Corporation nor GTRC may not be used in any form whatsoever
 *      without the express written consent of GTRC.
 * 
 *    * Redistributions of the software in any form, and publications
 *      based on work performed using the software should include the
 *      following citation as a reference:
 * 
 *      Kevin Barker, Thomas Benson, Dan Campbell, David Ediger, Roberto
 *      Gioiosa, Adolfy Hoisie, Darren Kerbyson, Joseph Manzano, Andres
 *      Marquez, Leon Song, Nathan R. Tallent, and Antonino Tumeo.
 *      PERFECT (Power Efficiency Revolution For Embedded Computing
 *      Technologies) Benchmark Suite Manual. Pacific Northwest National
 *      Laboratory and Georgia Tech Research Institute, December 2013.
 *      http://hpc.pnnl.gov/projects/PERFECT/
 *
 * 2. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 *    BATTELLE, GTRC, OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *    OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **EndCopyright*************************************************************/

#include "wami_utils.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>

void *xmalloc(size_t size, const char *file, int line)
{
    void *x = malloc(size);
    if (x == NULL)
    {
        fprintf(stderr, "Error: memory allocation of size %lu at %s:%d.\n",
            size, file, line);
        exit(EXIT_FAILURE);
    }
    return x;
}

void concat_dir_and_filename(
    char dir_and_filename[MAX_DIR_AND_FILENAME_LEN],
    const char *directory,
    const char *filename)
{
    assert(dir_and_filename != NULL);
    assert(directory != NULL);
    assert(filename != NULL);

    /* C89 lacks snprintf */
    if (strlen(directory) + strlen(filename) + 2 > MAX_DIR_AND_FILENAME_LEN)
    {
        fprintf(stderr, "Error: input directory (%s) too long.\n",
            directory);
        exit(EXIT_FAILURE);
    }
    dir_and_filename[0] = '\0';
    strncpy(dir_and_filename, directory, MAX_DIR_AND_FILENAME_LEN-1);
    dir_and_filename[MAX_DIR_AND_FILENAME_LEN-1] = '\0';
    strncat(dir_and_filename, "/",
        MAX_DIR_AND_FILENAME_LEN - strlen(dir_and_filename) - 1);
    strncat(dir_and_filename, filename,
        MAX_DIR_AND_FILENAME_LEN - strlen(dir_and_filename) - 1);
}

void read_data_file(
    char *data,
    const char *filename,
    const char *directory,
    size_t num_bytes)
{
    size_t nread = 0;
    FILE *fp = NULL;
    char dir_and_filename[MAX_DIR_AND_FILENAME_LEN];

    assert(data != NULL);
    assert(filename != NULL);
    assert(directory != NULL);

    concat_dir_and_filename(
        dir_and_filename,
        directory,
        filename);

    fp = fopen(dir_and_filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Unable to open input file %s for reading.\n",
            dir_and_filename);
        exit(EXIT_FAILURE);
    }

    nread = fread(data, sizeof(char), num_bytes, fp);
    if (nread != num_bytes)
    {
        fprintf(stderr, "Error: read failure on %s. "
            "Expected %lu bytes, but only read %lu.\n",
            dir_and_filename, num_bytes, nread);
        fclose(fp);
        exit(EXIT_FAILURE);
    }        

    fclose(fp);
}

void read_image_file(
    char *data,
    const char *filename,
    const char *directory,
    size_t num_bytes)
{
    int success = 1;
    size_t nread = 0, num_bytes_reported_by_header;
    FILE *fp = NULL;
    char dir_and_filename[MAX_DIR_AND_FILENAME_LEN];
    u16 width, height, channels, depth;

    assert(data != NULL);
    assert(filename != NULL);
    assert(directory != NULL);

    concat_dir_and_filename(
        dir_and_filename,
        directory,
        filename);

    fp = fopen(dir_and_filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Unable to open input file %s for reading.\n",
            dir_and_filename);
        exit(EXIT_FAILURE);
    }

    success &= (fread(&width, sizeof(u16), 1, fp) == 1);
    success &= (fread(&height, sizeof(u16), 1, fp) == 1);
    success &= (fread(&channels, sizeof(u16), 1, fp) == 1);
    success &= (fread(&depth, sizeof(u16), 1, fp) == 1);
    if (! success)
    {
        fprintf(stderr, "Error: header read failure on %s. ", dir_and_filename);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    num_bytes_reported_by_header = (size_t) width * height * channels * depth;
    if (num_bytes_reported_by_header != num_bytes)
    {
        fprintf(stderr, "Error: header dimensions in %s inconsistent with requested "
            "number of bytes to read (width=%u,height=%u,channels=%u,depth=%u)\n",
            dir_and_filename, width, height, channels, depth);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    nread = fread(data, sizeof(char), num_bytes, fp);
    if (nread != num_bytes)
    {
        fprintf(stderr, "Error: read failure on %s. "
            "Expected %lu bytes, but only read %lu.\n",
            dir_and_filename, num_bytes, nread);
        fclose(fp);
        exit(EXIT_FAILURE);
    }        

    fclose(fp);
}

void write_image_file(
    char *data,
    const char *filename,
    const char *directory,
    u16 width,
    u16 height,
    u16 channels)
{
    int success = 1;
    size_t nwritten = 0, num_bytes = 0;
    FILE *fp = NULL;
    char dir_and_filename[MAX_DIR_AND_FILENAME_LEN];
    const u16 depth = 2;

    assert(data != NULL);
    assert(filename != NULL);
    assert(directory != NULL);

    concat_dir_and_filename(
        dir_and_filename,
        directory,
        filename);

    fp = fopen(dir_and_filename, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Unable to open input file %s for reading.\n",
            dir_and_filename);
        exit(EXIT_FAILURE);
    }

    success &= (fwrite(&width, sizeof(u16), 1, fp) == 1);
    success &= (fwrite(&height, sizeof(u16), 1, fp) == 1);
    success &= (fwrite(&channels, sizeof(u16), 1, fp) == 1);
    success &= (fwrite(&depth, sizeof(u16), 1, fp) == 1);
    if (! success)
    {
        fprintf(stderr, "Error: header write failure on %s. ", dir_and_filename);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    num_bytes = (size_t) width * height * channels * depth;

    nwritten = fwrite(data, sizeof(char), num_bytes, fp);
    if (nwritten != num_bytes)
    {
        fprintf(stderr, "Error: write failure on %s. "
            "Expected to write %lu bytes, but only wrote %lu.\n",
            dir_and_filename, num_bytes, nwritten);
        fclose(fp);
        exit(EXIT_FAILURE);
    }        

    fclose(fp);
}
