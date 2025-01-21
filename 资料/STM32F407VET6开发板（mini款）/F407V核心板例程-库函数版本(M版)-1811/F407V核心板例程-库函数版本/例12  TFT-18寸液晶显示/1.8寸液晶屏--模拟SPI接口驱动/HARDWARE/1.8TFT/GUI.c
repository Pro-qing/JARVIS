#include "sys.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "delay.h"
#include "font.h"
#include "Picture.h"

//嵌入式开发网
//淘宝网站：http://mcudev.taobao.com



//以一个点为中心画圆函数

void Gui_Circle(u16 X,u16 Y,u16 R,u16 fc) 
{//Bresenham算法 
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
      { 
        Gui_DrawPoint(X+a,Y+b,fc);     //        7 
        Gui_DrawPoint(X-a,Y+b,fc);     //        6 
        Gui_DrawPoint(X+a,Y-b,fc);     //        2 
        Gui_DrawPoint(X-a,Y-b,fc);     //        3 
        Gui_DrawPoint(X+b,Y+a,fc);     //        8 
        Gui_DrawPoint(X-b,Y+a,fc);     //        5 
        Gui_DrawPoint(X+b,Y-a,fc);     //        1 
        Gui_DrawPoint(X-b,Y-a,fc);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
      { 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y-b,fc); 
        Gui_DrawPoint(X-a,Y-b,fc); 
        Gui_DrawPoint(X+b,Y+a,fc); 
        Gui_DrawPoint(X-b,Y+a,fc); 
        Gui_DrawPoint(X+b,Y-a,fc); 
        Gui_DrawPoint(X-b,Y-a,fc); 
     } 
	
} 
//画线函数
void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)   
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	Lcd_SetXY(x0,y0);
	dx = x1-x0;//计算x距离
	dy = y1-y0;//计算y距离

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{//且线的点数等于x距离，以x轴递增画点
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			//画点
			Gui_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //是否需要增加y坐标值
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//增加y坐标值
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x坐标值每次画点后都递增1
		} // end for
	} // end if |slope| <= 1
	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{//以y轴为递增画点
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Gui_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}



void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
{
	Gui_DrawLine(x,y,x+w,y,0xEF7D);
	Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
	Gui_DrawLine(x,y,x,y+h,0xEF7D);
  Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}


void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode)
{
	if (mode==0)	{
		Gui_DrawLine(x,y,x+w,y,0xEF7D);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
		Gui_DrawLine(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		Gui_DrawLine(x,y,x+w,y,0x2965);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
		Gui_DrawLine(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		Gui_DrawLine(x,y,x+w,y,0xffff);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
		Gui_DrawLine(x,y,x,y+h,0xffff);
	}
}


/**************************************************************************************
功能描述: 在屏幕显示一凸起的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
	Gui_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
	Gui_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
	Gui_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
	Gui_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
	Gui_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
}

/**************************************************************************************
功能描述: 在屏幕显示一凹下的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, WHITE); //H
	Gui_DrawLine(x1,  y1,  x1,y2, WHITE); //V
	
	Gui_DrawLine(x1+1,y2-1,x2,y2-1, GRAY1);  //H
	Gui_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
	Gui_DrawLine(x2-1,y1+1,x2-1,y2, GRAY1);  //V
  Gui_DrawLine(x2  ,y1  ,x2,y2, GRAY2); //V
}


//显示16x16的字符

void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;

	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    	if(asc16[k*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
				x+=8;
			}
			s++;
		}
			
		else 
		{
		

			for (k=0;k<hz16_num;k++) //限制在字库里查找对应字符的次数
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		} 
		
	}
}



//显示24x24的字符

void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k;

	while(*s) 
	{
		if( *s < 0x80 ) 
		{
			k=*s;
			if (k>32) k-=32; else k=0;

		    for(i=0;i<16;i++)
			for(j=0;j<8;j++) 
				{
			    	if(asc16[k*16+i]&(0x80>>j))	
					Gui_DrawPoint(x+j,y+i,fc);
					else 
					{
						if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
					}
				}
			s++;x+=8;
		}
		else 
		{

			for (k=0;k<hz24_num;k++) 
			{
			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<24;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3]&(0x80>>j))
								Gui_DrawPoint(x+j,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+2]&(0x80>>j))	
								Gui_DrawPoint(x+j+16,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+16,y+i,bc);
								}
							}
				    }
			  }
			}
			s+=2;x+=24;
		}
	}
}

//显示16x32的点阵数字字符--数码管字体

void Gui_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num)
{
	unsigned char i,j,k,c;

  for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c=*(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	Gui_DrawPoint(x+j*8+k,y+i,fc);
					else 
						{
						if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
						}
			}
		}
	}
}





unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};




void Redraw_Mainmenu(void)
{

	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(8,2,BLUE,GRAY0,"Mcudev电子技术");
	Gui_DrawFont_GBK16(32,20,BLUE,GRAY0,"液晶测试");

	DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,40,GREEN,GRAY0,"颜色填充测试");

	DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,70,BLUE,GRAY0,"文字显示测试");

	DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,100,RED,GRAY0,"图片显示测试");

	 #if USE_HORIZONTAL==1	//使用横屏	
	
		Gui_DrawFont_Num32(125,90,RED,GRAY0,Num[5]);
		delay_ms(1000);
		Gui_DrawFont_Num32(125,90,RED,GRAY0,Num[4]);
		delay_ms(1000);
		Gui_DrawFont_Num32(125,90,RED,GRAY0,Num[3]);
		delay_ms(1000);
		Gui_DrawFont_Num32(125,90,RED,GRAY0,Num[2]);
		delay_ms(1000);
		Gui_DrawFont_Num32(125,90,RED,GRAY0,Num[1]);
		delay_ms(1000);
		Gui_DrawFont_Num32(125,90,RED,GRAY0,Num[0]);	
	
	#else	
	
		Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[5]);
		delay_ms(1000);
		Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[4]);
		delay_ms(1000);
		Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[3]);
		delay_ms(1000);
		Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[2]);
		delay_ms(1000);
		Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[1]);
		delay_ms(1000);
		Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[0]);	
	
	#endif

}

void Num_Test(void)
{
	u8 i=0;
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	delay_ms(1000);
	Lcd_Clear(GRAY0);

	for(i=0;i<10;i++)
	{
		Gui_DrawFont_Num32((i%3)*40,32*(i/3),RED,GRAY0,Num[i]);
		delay_ms(100);
	}
	delay_ms(2000);
	
}

void Font_Test(void)
{
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"文字显示测试");

	delay_ms(1000);
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,16,BLACK,GRAY0,"嵌入式开发网");
	Gui_DrawFont_GBK16(32,36,BLACK,GRAY0,"中文测试");
	Gui_DrawFont_GBK16(16,52,BLUE,GRAY0,"专注液晶批发");
	Gui_DrawFont_GBK16(16,70,RED,GRAY0, "全程技术支持");
	Gui_DrawFont_GBK16(16,100,GREEN,GRAY0,"mcudev.taobao");

	delay_ms(1800);	
}

void Color_Test(void)
{
	u8 i=1;
	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(20,10,BLUE,GRAY0,"Color Test");
	delay_ms(200);

	while(i--)
	{
	Lcd_Clear(WHITE);
	Lcd_Clear(BLACK);
	Lcd_Clear(RED);
  Lcd_Clear(GREEN);
  Lcd_Clear(BLUE);
	}		
}
//16位 垂直扫描  右到左  高位在前
void show_pic(void)
{
	int i,j,k;
	unsigned char picH,picL;
	
	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"图片显示测试");
	
	delay_ms(1000);
	
	Lcd_Clear(GRAY0);
	
	#if USE_HORIZONTAL==1	//使用横屏	

	{
			k=0;
			for(i=0;i<128;i++)
			for(j=0;j<160;j++)
			{
				picH=gImage_1234[k++];
				picL=gImage_1234[k++];
				Lcd_WriteData(picH);
				Lcd_WriteData(picL);
				
				
			}			
	}	

		
	#else	
	{
		k=0;
		for(i=0;i<160;i++)
		for(j=0;j<128;j++)
		{
			picH=gImage_123[k++];
			picL=gImage_123[k++];
			Lcd_WriteData(picH);
			Lcd_WriteData(picL);
		}	
	}		
	#endif
} 

//画圆 划线  算法演示
void show_Circle(void)
{

	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(8,10,BLUE,GRAY0,"Circle显示测试");
	
	delay_ms(1000);
	
	Lcd_Clear(GRAY0);
	
#if USE_HORIZONTAL==1	//使用横屏	

	Gui_DrawLine(0, 0,159,0,RED);//画一条直径线
	Gui_DrawLine(0, 1,159,1,RED);//画一条直径线
	
	Gui_DrawLine(159, 0,159,127,RED);//画一条直径线
	Gui_DrawLine(158, 0,158,127,RED);//画一条直径线
	
	Gui_DrawLine(0, 127,159,127,RED);//画一条直径线
	Gui_DrawLine(0, 126,159,126,RED);//画一条直径线
	
	Gui_DrawLine(0, 0,0, 127,RED);//画一条直径线
	Gui_DrawLine(1, 0,1, 127,RED);//画一条直径线
	
 #else		//竖屏显示
		
	Gui_DrawLine(0, 0,127,0,RED);//画一条直径线
	Gui_DrawLine(0, 1,127,1,RED);//画一条直径线
	
	Gui_DrawLine(127, 0,127,159,RED);//画一条直径线
	Gui_DrawLine(126, 0,126,159,RED);//画一条直径线
	
	Gui_DrawLine(0, 159,127,159,RED);//画一条直径线
	Gui_DrawLine(0, 158,127,158,RED);//画一条直径线
	
	Gui_DrawLine(0, 0,0, 159,RED);//画一条直径线
	Gui_DrawLine(1, 0,1, 159,RED);//画一条直径线

	#endif
	
	Gui_Circle(64,64,50,RED);//画一个圆形
	Gui_Circle(64,64,40,GREEN);
	Gui_Circle(64,64,30,BLUE);
	
	
	Gui_DrawLine(10, 79,110, 79,BLACK);//画一条黑色的直径线
	Gui_DrawLine(10, 80,110, 80,BLACK);//画一条黑色的直径线
	Gui_DrawLine(10, 81,110, 81,BLACK);//画一条黑色的直径线
} 



void Test_Demo(void)	//液晶屏测试例程
{
		Redraw_Mainmenu();
		Color_Test();//简单纯色填充测试
		Num_Test();  //数字显示测试
		Font_Test();//中英文显示测试		
		show_pic();//图片显示示例
		delay_ms(1200);
		show_Circle();//绘图算法显示---画圆
		delay_ms(2000);
	
}


