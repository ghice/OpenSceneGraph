/**********************************************************************
 *
 *    FILE:            Image.cpp
 *
 *    DESCRIPTION:    Read/Write osg::Image in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerated
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 20.3.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "Exception.h"
#include "Image.h"
#include "Object.h"

using namespace ive;

void Image::write(DataOutputStream* out)
{
    // Write Image's identification.
    out->writeInt(IVEIMAGE);
    // If the osg class is inherited by any other class we should also write this to file.
    osg::Object*  obj = dynamic_cast<osg::Object*>(this);
    if(obj){
        ((ive::Object*)(obj))->write(out);
    }
    else
        throw Exception("Image::write(): Could not cast this osg::Image to an osg::Object.");
    // Write Image's properties.

    // Write name
    out->writeString(getFileName());
 
    // Write width, height, depth of image.
    out->writeInt(s());
    out->writeInt(t());
    out->writeInt(r());
    
    
    std::cout << getFileName()<<"\t"<<s()<<"\t"<<t()<<std::endl;

    // Write formats, type and packing
    out->writeInt(getInternalTextureFormat());
    out->writeInt(getPixelFormat());
    out->writeInt(getDataType());
    out->writeInt(getPacking());

    // Write modified tag.
    out->writeInt(getModifiedTag()); 

    // Write mipmapdata vector
    int size = _mipmapData.size();
    out->writeInt(size);
    for(int i=0;i<size;i++)
        out->writeInt(_mipmapData[i]);

    // Write image data if any
    out->writeLong((long)data());
    if(data()){
        // Compute the size of image data and write this.
        unsigned int size = getTotalSizeInBytesIncludingMipmaps();
        out->writeInt(size);
        // Write the data
        out->writeCharArray((char*)data(), size);
    }

}

void Image::read(DataInputStream* in)
{
    // Peek Image's identification.
    int id = in->peekInt();
    if(id == IVEIMAGE){
        // Read Image's identification.
        id = in->readInt();
        // If the osg class is inherited by any other class we should also read this from file.
        osg::Object*  obj = dynamic_cast<osg::Object*>(this);
        if(obj){
            ((ive::Object*)(obj))->read(in);
        }
        else
            throw Exception("Image::read(): Could not cast this osg::Image to an osg::Object.");
        // Read Image's properties.

        // Read name
        setFileName(in->readString());

        // Read width, height, depth of image.
        int is=in->readInt();
        int it=in->readInt();
        int ir=in->readInt();

        // Read formats, type and packing
        GLint internalTextureFormat = (GLint) in->readInt();
        GLenum pixelFormat = (GLenum) in->readInt();
        GLenum dataType = (GLenum) in->readInt();;
        unsigned int packing = (unsigned int)in->readInt();

        // Read modified tag.
        setModifiedTag((unsigned int)in->readInt());

        // Read mipmapdata vector
        int size = in->readInt();
        _mipmapData.resize(size);

        //if (size) std::cout<<"is mip mapped ";

        for(int i=0;i<size;i++)
        {
            _mipmapData[i]=(unsigned int)in->readInt();
            //std::cout<<_mipmapData[i]<<"\t";
        }
        //std::cout<<std::endl;
            

        // Read image data if any
        if(in->readInt()){
            unsigned int dataSize = (unsigned int)in->readInt();
            
            //static int totalSize = 0;
            
            //totalSize += dataSize;
            //std::cout<<getFileName()<<" "<<is<<" "<<it<<std::endl;
            //std::cout<<" dataSize = "<<dataSize<<"\tTotalSize = "<<totalSize<<std::endl;
            
            
            //char* data = (char*)malloc (dataSize);
            char* data =  new char[dataSize];
            if(!data)
                throw Exception("Image::read(): Unable to allocate memory for image data.");
            in->readCharArray(data,dataSize);
            setImage(is, it, ir, internalTextureFormat, pixelFormat,
                dataType, (unsigned char* ) data, osg::Image::USE_NEW_DELETE, packing);
        }
    }
    else{
        throw Exception("Image::read(): Expected Image identification.");
    }
}
