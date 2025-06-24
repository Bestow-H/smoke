#ifndef __SERIALPACK_H__
#define __SERIALPACK_H__
class SerialToolPack{
public:
    SerialToolPack();
    unsigned int x0;
    unsigned int x1;
    unsigned int y1;
    unsigned int x2;
    unsigned int y2;
    unsigned int x3;
    unsigned int y3;
    unsigned int x4;
    unsigned int y4;
    void unpack(unsigned char*buffer,int size);
    void pack(unsigned char*buffer,int size);
};
#endif
