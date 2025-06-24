#include"SerialPack.h"
SerialToolPack::SerialToolPack(){
    x1=0;
    x2=0;
    y1=0;
    y2=0;
    x3=0;
    x4=0;
    y3=0;
    y4=0;
}

void SerialToolPack::unpack(unsigned char*buffer,int size){
    if(size!=18){
      return;
    }
    x1=(buffer[1]<<8);
    x1=x1 | buffer[2];
    
    y1=(buffer[3]<<8);
    y1=y1 | buffer[4];
    
    x2=(buffer[5]<<8);
    x2=x2 | buffer[6];
    
    y2=(buffer[7]<<8);
    y2=y2 | buffer[8];

    x3=(buffer[9]<<8);
    x3=x3 | buffer[10];

    y3=(buffer[11]<<8);
    y3=y3 | buffer[12];

    x4=(buffer[13]<<8);
    x4=x4 | buffer[14];

    y4=(buffer[15]<<8);
    y4=y4 | buffer[16];
}

void SerialToolPack::pack(unsigned char*buffer,int size){
    if(size!=18){
        return;
    }
    buffer[0]=0xCD;

    buffer[1]=(x1>>8)&0xFF;
    buffer[2]=x1 & 0xFF;
    
    buffer[3]=(y1>>8)&0xFF;
    buffer[4]=y1 & 0xFF;
    
    buffer[5]=(x2>>8)&0xFF;
    buffer[6]=x2 & 0xFF;
    
    buffer[7]=(y2>>8)&0xFF;
    buffer[8]=y2 & 0xFF;
    
    buffer[9]=(x3>>8)&0xFF;
    buffer[10]=x3 & 0xFF;

    buffer[11]=(y3>>8)&0xFF;
    buffer[12]=y3 & 0xFF;

    buffer[13]=(x4>>8)&0xFF;
    buffer[14]=x4 & 0xFF;

    buffer[15]=(y4>>8)&0xFF;
    buffer[16]=y4 & 0xFF;

    buffer[17]=0xDC;
}
