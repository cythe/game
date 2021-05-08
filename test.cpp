#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <string.h>
#include "basetype.h"

void JpegToBmp(char *JpegName, char *NewBmpName)//将读取jpeg中的数据并保存为BMP格式的文件,生成的都是24位真彩图，不需要调色板
{
    FILE *fNewBmp;//存储生成的Bmp文件的句柄
    FILE *fJpeg;//目标jpeg文件的句柄
    unsigned char *data;   //存放解压后的数据
    unsigned char *jpgbuf;      //存放解压后一行图像数据
    BITMAPFILEHEADER header;//存储文件头
    memset(&header, 0, sizeof(header));//文件头赋初始值
    BITMAPINFOHEADER infoheader;//位图信息头
    memset(&infoheader, 0, sizeof(infoheader));//赋初始值

    printf("header = %d\t infoheader = %d\n", sizeof(header), sizeof(infoheader));

    int row_stride;        //定义每行的字节数
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);//声明并初始化解压缩对象

    fJpeg=fopen(JpegName,"rb");//二进制模式读取jpeg文件
    if(fJpeg==NULL) //二进制模式读取
    {
	printf("error: cannot open  the file [%s]\n", JpegName);
	return ;
    }//打开jpeg图片

    jpeg_stdio_src(&cinfo, fJpeg);//指定解压对象的源文件
    jpeg_read_header(&cinfo, TRUE);//读取文件信息，将图像的缺省的信息填充到cinfo结构中比便程序使用
    jpeg_start_decompress(&cinfo);//开始接压缩

    data=(unsigned char *)malloc(cinfo.output_height* cinfo.output_components*cinfo.output_width);//动态分配数据存储内存
    memset(data,0,cinfo.output_height*cinfo.output_width*cinfo.output_components);//设置图像数据初值为0
    jpgbuf = (unsigned char *) malloc(cinfo.output_width *cinfo.output_components);//动态分配缓存内存
    memset(jpgbuf,0,cinfo.output_width*cinfo.output_components);//为缓存内存设置初值

    row_stride = cinfo.output_width * cinfo.output_components; //计算每行所需的空间，字节为单位
    while (cinfo.output_scanline < cinfo.output_height)
    {
	int line=cinfo.output_scanline;//当前行数

	(void) jpeg_read_scanlines(&cinfo, &jpgbuf, 1);//执行该操作读取第line行数据，cinfo.output_scanline将加一，指向下一个要扫描的行

	for(int i=0;i< cinfo.output_width;i++)//循环将存储在jpgbuf缓存区的数据放入data中
	{      
	    data[line*row_stride+i*cinfo.output_components+0]=jpgbuf[i*3];
	    data[line*row_stride+i*cinfo.output_components+1]=jpgbuf[i*3+1];
	    data[line*row_stride+i*cinfo.output_components+2]=jpgbuf[i*3+2];
#ifdef SHOWDATA__
	    printf("(%d,%d,%d),(%d,%d)",jpgbuf[i*3],jpgbuf[i*3+1],jpgbuf[i*3+2],line,i);//打印图像数据
#endif  
	}

    }

    free(jpgbuf);
    //填充文件头信息
    header.bfType= 0x4D42;//设置为“BM”
    header.bfSize=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+cinfo.output_width* cinfo.output_components*cinfo.output_width ;
    header.bfOffBits=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    printf("header.bfType IS %c\n", (char)header.bfType);
    printf("header.bfSize IS %d\n", header.bfSize);
    printf("header.bfOffBits IS %d\n", header.bfOffBits);
    //printf("sizeof（data） IS %d.\n",cinfo.output_width* cinfo.output_components*cinfo.output_width );

    //填充位图信息头
    infoheader.biSize=sizeof(infoheader);//位图信息头结构的长度，为40
    infoheader.biBitCount=24;//设置为真彩图
    infoheader.biHeight=cinfo.image_height;
    infoheader.biWidth=cinfo.image_width;
    infoheader.biCompression=0; //BI_RGB 一般默认为BI_RGB格式表示不压缩
    infoheader.biPlanes=1;//表示不用考虑
    infoheader.biXPelsPerMeter=0;
    infoheader.biYPelsPerMeter=0;
    infoheader.biClrUsed=0;//在MSDN中队RGB位图定义可知默认值为0

    //将文件 头和位图信息头写入文件中
    fNewBmp=fopen(NewBmpName,"wb");//二进制写的形式打开文件
    if(!fNewBmp)
    {
	printf("cannot open the NewBmpFile [%s]\n", NewBmpName);
	return;
    }
    fwrite(&header,sizeof(header),1,fNewBmp);//写入文件头
    fwrite(&infoheader,sizeof(infoheader),1,fNewBmp);//写入位图信息头

    //由于BMP扫描方式为从上到下，从左到右，window扫描要满足4字节的倍数。读data数据进行修改，存储为BGR顺序
    int tmpRow=((infoheader.biWidth*3+3)>>2)<<2;//调整没行的字节数，使其是4的倍数

    printf("((infoheader.biWidth*3+3)>>2)<<2 IS %d\n", (((infoheader.biWidth*3+3)>>2)<<2));
    printf("infoheader.biWidth*3 IS %d\n", infoheader.biWidth*3);

    jpgbuf = (unsigned char *) malloc(tmpRow);//动态分配缓存内存
    memset(jpgbuf,0,tmpRow);//为缓存内存设置初值

    for(int i=0;i<infoheader.biHeight;i++)
    {
	for(int j=0;j<infoheader.biWidth;j++)//将从下到上从左到右读取数据，并将RGB顺寻装换为BGR顺序
	{

	    jpgbuf[3*j+0]=data[(infoheader.biHeight-1-i)*row_stride+3*j+2];
	    jpgbuf[3*j+1]=data[(infoheader.biHeight-1-i)*row_stride+3*j+1];
	    jpgbuf[3*j+2]=data[(infoheader.biHeight-1-i)*row_stride+3*j+0];
#if 0
	    //printf("data(%d,%d,%d),buf(%d,%d,%d)",data[(infoheader.biHeight-1-i)*row_stride+3*j+0],data[(infoheader.biHeight-1-i)*row_stride+3*j+1],data[(infoheader.biHeight-1-i)*row_stride+3*j+2], jpgbuf[3*j+0], jpgbuf[3*j+1], jpgbuf[3*j+2]);
	    printf("%d,%d,%d ",data[(infoheader.biHeight-1-i)*row_stride+3*j+0],data[(infoheader.biHeight-1-i)*row_stride+3*j+1],data[(infoheader.biHeight-1-i)*row_stride+3*j+2]);
#endif

	}
///	printf("\n");
	fwrite(jpgbuf,sizeof(unsigned char),tmpRow,fNewBmp);//每行的数据写入文件中
    }
    jpeg_finish_decompress(&cinfo);//完成解压过程
    jpeg_destroy_decompress(&cinfo);//释放cinfo
    fclose(fJpeg);
    free(data);
    free(jpgbuf);
    fclose(fNewBmp);
    return;
}

int main(int argc, char* argv)
{
    JpegToBmp("t.jpg", "d.bmp");
}
