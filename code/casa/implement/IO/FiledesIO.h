//# FiledesIO.h: Class for IO on a file descriptor
//# Copyright (C) 1996,1997,1999
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id$

#if !defined(AIPS_FILEDESIO_H)
#define AIPS_FILEDESIO_H

//# Includes
#include <aips/aips.h>
#include <aips/IO/ByteIO.h>
#include <aips/Utilities/String.h>

// <summary>Class for IO on a file descriptor.</summary>

// <use visibility=export>

// <reviewed reviewer="Friso Olnon" date="1996/11/06" tests="tByteIO" demos="">
// </reviewed>

// <prerequisite> 
//    <li> <linkto class=ByteIO>ByteIO</linkto> class
//    <li> file descriptors
// </prerequisite>

// <synopsis> 
// This class is a specialization of class
// <linkto class=ByteIO>ByteIO</linkto>. It uses a file descriptor
// to read/write data.
// <p>
// The file associated with the file descriptor has to be opened
// before hand.
// The constructor will determine automatically if the file is
// readable, writable and seekable.
// Note that on destruction the file descriptor is NOT closed.
// </synopsis>

// <example>
// This example shows how FiledesIO can be used with an fd.
// It uses the fd for a regular file, which could be done in an easier
// way using class <linkto class=RegularFileIO>RegularFileIO</linkto>.
// However, when using pipes or sockets, this would be the only way.
// <srcblock>
//    // Get a file descriptor for the file.
//    int fd = open ("file.name");
//    // Use that as the source of AipsIO (which will also use CanonicalIO).
//    FiledesIO fio (fd);
//    AipsIO stream (&fio);
//    // Read the data.
//    Int vali;
//    Bool valb;
//    stream >> vali >> valb;
// </srcblock>
// </example>

// <motivation> 
// Make it possible to use the AIPS++ IO functionality on any file.
// In this way any device can be hooked to the IO framework.
// </motivation>


class FiledesIO: public ByteIO
{
public: 
    // Default constructor.
    // A stream can be attached using the attach function.
    FiledesIO();

    // Construct from the given file descriptor.
    explicit FiledesIO (int fd);

    // Attach to the given file descriptor.
    void attach (int fd);

    // The destructor does not close the file.
    ~FiledesIO();
    
    // Write the number of bytes.
    virtual void write (uInt size, const void* buf);

    // Read <src>size</src> bytes from the descriptor. Returns the number of
    // bytes actually read or a negative number if an error occured. Will throw
    // an Exception (AipsError) if the requested number of bytes could not be
    // read, or an error occured, unless throwException is set to False. Will
    // always throw an exception if the descriptor is not readable or the
    // system call returned an undocumented value.
    virtual Int read (uInt size, void* buf, Bool throwException=True);    

    // Reset the position pointer to the given value. It returns the
    // new position.
    virtual Long seek (Long offset, ByteIO::SeekOption = ByteIO::Begin);

    // Get the length of the byte stream.
    virtual Long length();
       
    // Is the IO stream readable?
    virtual Bool isReadable() const;

    // Is the IO stream writable?
    virtual Bool isWritable() const;

    // Is the IO stream seekable?
    virtual Bool isSeekable() const;

    // Get the file name of the file attached.
    virtual String fileName() const;

    // Some static convenience functions for file create/open/close.
    // <group>
    static int create (const Char* name, int mode = 0644);
    static int open   (const Char* name, Bool writable = False,
		       Bool throwExcp = True);
    static void close (int fd);
    // </group>


protected:
    // Detach the FILE. Close it when it is owned.
    void detach();

    // Determine if the file descriptor is readable and/or writable.
    void fillRWFlags (int fd);

    // Determine if the file is seekable.
    void fillSeekable();

private:
    Bool        itsOwner;
    Bool        itsSeekable;
    Bool        itsReadable;
    Bool        itsWritable;
    int         itsFile;

    // Copy constructor, should not be used.
    FiledesIO (const FiledesIO& that);

    // Assignment, should not be used.
    FiledesIO& operator= (const FiledesIO& that);
};



#endif
