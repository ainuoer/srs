//
// Copyright (c) 2013-2021 The SRS Authors
//
// SPDX-License-Identifier: MIT
//

#ifndef SRS_KERNEL_FILE_HPP
#define SRS_KERNEL_FILE_HPP

#include <srs_core.hpp>

#include <srs_kernel_io.hpp>

#include <string>
#include <deque>

// for srs-librtmp, @see https://github.com/ossrs/srs/issues/213
#ifndef _WIN32
#include <sys/uio.h>
#include "srs_kernel_flv.hpp"

#endif

class SrsFileReader;

/**
 * file writer, to write to file.
 */
class SrsFileWriter : public ISrsWriteSeeker
{
private:
    std::string path;
    int fd;
public:
    SrsFileWriter();
    virtual ~SrsFileWriter();
public:
    /**
     * open file writer, in truncate mode.
     * @param p a string indicates the path of file to open.
     */
    virtual srs_error_t open(std::string p);
    /**
     * open file writer, in append mode.
     * @param p a string indicates the path of file to open.
     */
    virtual srs_error_t open_append(std::string p);
    /**
     * close current writer.
     * @remark user can reopen again.
     */
    virtual void close();
public:
    virtual bool is_open();
    virtual void seek2(int64_t offset);
    virtual int64_t tellg();
// Interface ISrsWriteSeeker
public:
    virtual srs_error_t write(void* buf, size_t count, ssize_t* pnwrite);
    virtual srs_error_t writev(const iovec* iov, int iovcnt, ssize_t* pnwrite);
    virtual srs_error_t lseek(off_t offset, int whence, off_t* seeked);
};


class ISendDatachannel
{
public:
    ISendDatachannel();
    virtual ~ISendDatachannel();
public:
    virtual srs_error_t send_data_channel(const uint16_t sid, const char* buf, const int len) = 0;
};

class IComsumeDatachannel
{
public:
    std::deque<SrsSharedPtrMessage*> datachannel_queue;
    SrsSharedPtrMessage* datachannel_metadata;
    SrsSharedPtrMessage* datachannel_sequence;
    bool hasIdr;
public:
    IComsumeDatachannel();
    virtual ~IComsumeDatachannel();
public:
    virtual srs_error_t enqueue_datachannel(SrsSharedPtrMessage* msg) = 0;
    virtual srs_error_t set_datachannel_metadata(SrsSharedPtrMessage* msg) = 0;
    virtual srs_error_t set_datachannel_sequence(SrsSharedPtrMessage* msg) = 0;
};

class SrsDatachannelWriter : public SrsFileWriter
{
private:
    ISendDatachannel* sctp;
    uint16_t sid;
public:
    SrsDatachannelWriter(ISendDatachannel* s, uint16_t id);
    virtual ~SrsDatachannelWriter();
public:
    virtual srs_error_t write(void* buf, size_t count, ssize_t* pnwrite);
};

// The file reader factory.
class ISrsFileReaderFactory
{
public:
    ISrsFileReaderFactory();
    virtual ~ISrsFileReaderFactory();
public:
    virtual SrsFileReader* create_file_reader();
};

/**
 * file reader, to read from file.
 */
class SrsFileReader : public ISrsReadSeeker
{
private:
    std::string path;
    int fd;
public:
    SrsFileReader();
    virtual ~SrsFileReader();
public:
    /**
     * open file reader.
     * @param p a string indicates the path of file to open.
     */
    virtual srs_error_t open(std::string p);
    /**
     * close current reader.
     * @remark user can reopen again.
     */
    virtual void close();
public:
    // TODO: FIXME: extract interface.
    virtual bool is_open();
    virtual int64_t tellg();
    virtual void skip(int64_t size);
    virtual int64_t seek2(int64_t offset);
    virtual int64_t filesize();
// Interface ISrsReadSeeker
public:
    virtual srs_error_t read(void* buf, size_t count, ssize_t* pnread);
    virtual srs_error_t lseek(off_t offset, int whence, off_t* seeked);
};

// For utest to mock it.
typedef int (*srs_open_t)(const char* path, int oflag, ...);
typedef ssize_t (*srs_write_t)(int fildes, const void* buf, size_t nbyte);
typedef ssize_t (*srs_read_t)(int fildes, void* buf, size_t nbyte);
typedef off_t (*srs_lseek_t)(int fildes, off_t offset, int whence);
typedef int (*srs_close_t)(int fildes);

#endif

