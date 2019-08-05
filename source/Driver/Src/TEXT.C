
#include "include.h"
extern void (*PutPixel)(U32,U32,U8);


void dis6x12(short x,short y,char *s, unsigned char transparence,unsigned char forecolor,unsigned char bkcolor)
{
   unsigned char *pTemp;
   unsigned char cc;
   short step;
   short row,col;
  
   step = 7;
   while( (cc=(*s++))!=0x0 )
   {
	   if( cc>=0x80 )
	   cc=0x0;
	   
	   pTemp=(unsigned char *)(dot0612+ 13*(cc - 32));

	   if( transparence== 1 )
	   {
		   for(row=0; row< 13; row++ )
		   {
			  cc=*pTemp++;
			  for( col=0; col< 7; col++ )
			  {
				 if( ( cc >>(7-col) )&1 )
				 	PutPixel( x+col, y+row,forecolor );
			  }
		   }
	   }
	   else
	    {
		   for(row=0; row< 13; row++ )
		   {
			  cc=*pTemp++;
			  for(col=0; col< 7; col++ )
			  {
				  if( ( cc>>(7-col) )&1 )
				  	PutPixel( x+col, y+row, forecolor );
				  else
					PutPixel( x+col, y+row, bkcolor   );
			  }
		  }//for(col=0;col<(step-1);col++)
	   }//else
	   
	   x+=step;
   }// while(cc=*s++)
   
}


void dis8x16(short x,short y,char *s, unsigned char transparence,unsigned char forecolor,unsigned char bkcolor)
{
   unsigned char *pTemp;
   unsigned char cc;
   short step;
   short row,col;
   step = 9;
   while( (cc=(*s++))!=0x0 ){
	   if( cc>=0x80 )cc=0x0;
	   pTemp=(unsigned char *)(dot8x16+16*cc);

	   if( transparence== 1 ){
		   for(row=0; row<16; row++ ){
			  cc=*pTemp++;
			  for( col=0; col<8; col++ ){
				 if( ( cc >>(7-col) )&1 )PutPixel( x+col, y+row,forecolor );
			  }
		   }
	   }
	   else {
		   for(row=0; row<16; row++ ){
			  cc=*pTemp++;
			  for(col=0; col<8; col++ ){
				  if( ( cc>>(7-col) )&1 )PutPixel( x+col, y+row, forecolor );
						    else PutPixel( x+col, y+row, bkcolor   );
			  }
		  }//for(col=0;col<(step-1);col++){
	   }//else
	   x+=step;
   }// while(cc=*s++)
}

void dis8x16chart(short x,short y,char c, unsigned char transparence,unsigned char forecolor,unsigned char bkcolor)
{
   unsigned char *pTemp;
   unsigned char cc;
   short row,col;
   if( c>=0x80 )return ;
   pTemp=(unsigned char *)(dot8x16+16*c);
   if( transparence== 1 ){
      for(row=0; row<16; row++ ){
	  cc=*pTemp++;
	  for( col=0; col<8; col++ ){
		 if( ( cc >>(7-col) )&1 )PutPixel( x+col, y+row,forecolor );
	  }
      }
   }
   else {
      for(row=0; row<16; row++ ){
	  cc=*pTemp++;
	  for(col=0; col<8; col++ ){
		  if( ( cc>>(7-col) )&1 )PutPixel( x+col, y+row, forecolor );
				    else PutPixel( x+col, y+row, bkcolor   );
	  }
      }//for(col=0;col<(step-1);col++){
   }//else
}

// Hyq modified for reverse draw
#if 1
void PutCh16x16Str(short x,short y,char *s,unsigned char transparence,unsigned char forecolor,unsigned char bkcolor)
{
     unsigned char *pTemp;	
     long se;
     unsigned char qm,wm;
     short row,col,bytes;
     while( (qm=*s++)!=0 )
     {
        if(qm>=0x80)
        {
           qm=qm-0xa0-1;
           wm=*s++;
           wm=wm-0xa0-1;
           se=(qm*94L+wm)*32L;
           pTemp=(unsigned char *)(hz16dot+se);
           if(transparence)
           {
		        for(row=0;row<16;row++)
		        {
			    	for(bytes=0;bytes<2;bytes++)
			    	{
			       		for(col=0;col<8;col++)
			       			if((*(pTemp+row*2+bytes)>>(7-col))&1)
			       				PutPixel(x+bytes*8+col,y+row,forecolor);
			    	}
				}//for(row=0;   
	   	    }//if(transp
	   		else 
	   		{
	        	for(row=0;row<16;row++)
	        	{
			    	for(bytes=0;bytes<2;bytes++)
			    	{
			        	for(col=0;col<8;col++)
			        		if((*(pTemp+row*2+bytes)>>(7-col))&1)
			        			PutPixel(x+bytes*8+col,y+row,forecolor);
			                else 
			                	PutPixel(x+bytes*8+col,y+row,bkcolor);
			    	}
				}//for(row=0;   
	  		}//else	
	   		x+=16;
		}//if(qm>=0x80){ 
		else
		{ // not hz
		   dis8x16chart( x,y,qm, transparence,forecolor, bkcolor );
		   x+=8;
	    }
    }// while(qm=*s++)
}

#else

void PutCh16x16Str(short x,short y,char *s,unsigned char transparence,unsigned char forecolor,unsigned char bkcolor)
{
     unsigned char *pTemp;	
     long se;
     unsigned char qm,wm;
     short row,col,bytes;
     while( (qm=*s++)!=0 )
     {
        if(qm>=0x80)
        {
           qm=qm-0xa0-1;
           wm=*s++;
           wm=wm-0xa0-1;
           se=(qm*94L+wm)*32L;
           pTemp=(unsigned char *)(hz16dot+se);
           if(transparence)
           {
		        for(row=0;row<16;row++)
		        {
			    	for(bytes=0;bytes<2;bytes++)
			    	{
			       		for(col=0;col<8;col++)
			       			if((*(pTemp+row*2+bytes)>>(7-col))&1)
			       				PutPixel(x+bytes*8+col,y+row,forecolor);
			    	}
				}//for(row=0;   
	   	    }//if(transp
	   		else 
	   		{
	        	for(row=0;row<16;row++)
	        	{
			    	for(bytes=0;bytes<2;bytes++)
			    	{
			        	for(col=0;col<8;col++)
			        		if((*(pTemp+row*2+bytes)>>(7-col))&1)
			        			PutPixel(x+bytes*8+col,y+row,forecolor);
			                else 
			                	PutPixel(x+bytes*8+col,y+row,bkcolor);
			    	}
				}//for(row=0;   
	  		}//else	
	   		x+=17;
		}//if(qm>=0x80){ 
		else
		{ // not hz
		   dis8x16chart( x,y+3,qm, transparence,forecolor, bkcolor );
		   x+=9;
	    }
    }// while(qm=*s++)
}
#endif

void PutNum16x16(short x,short y,float Num,unsigned char transparence,unsigned char forecolor,unsigned char bkcolor)
{
	char String_Buffer[100];
	unsigned char t;
	for(t=0;t<=99;t++)
	{
		String_Buffer[t]=0;
	}
	sprintf(String_Buffer,"%g",Num);	
	for(t=0;t<=99;t++)
	{
		if(String_Buffer[t]==0xff)
		{
			String_Buffer[t]=0x2e;	
		}
		if(String_Buffer[t]==0)
		{
			t=99;
		}		
	}
	
	PutCh16x16Str(x,y,String_Buffer,transparence,forecolor,bkcolor);
}

float GetNum16x16(char * Source_Str)
{
	float Return_Val;
	unsigned char t;
	for(t=0;t<=99;t++)
	{
		if(Source_Str[t]==0x2e)
		{
			Source_Str[t]=0xff;	
			t=99;
		}
		if(Source_Str[t]==0)
		{
			t=99;
		}
		
	}
		
	sscanf("90","%g",&Return_Val);
//	sscanf(Source_Str,"%g",&Return_Val);
	return Return_Val;
}

#if 0
void PutCh24x24Str(short x,short y,char *s,unsigned char transparence,unsigned char forecolor,unsigned char bkcolor)
{
     unsigned char *pTemp;	
     long se;
     unsigned char qm,wm;
     short row,col,bytes;
     while( (qm=*s++)!=0 )
     {
        if(qm>=0x80)
        {
           qm=qm-0xa0-16;
           wm=*s++;
           wm=wm-0xa0-1;
           se=(qm*94L+wm)*72L;
           pTemp=(unsigned char *)(hz24dot+se);
           if(transparence)
           {
		        for(row=0;row<24;row++)
		        {
			    	for(bytes=0;bytes<3;bytes++)
			    	{
			       		for(col=0;col<8;col++)
			       			if((*(pTemp+row*3+bytes)>>(7-col))&1)
			       				PutPixel((x+row), (y+bytes*8+col), forecolor);//x+bytes*8+col,y+row,forecolor);
			    	}
				}//for(row=0;   
	   	    }//if(transp
	   		else 
	   		{
	        	for(row=0;row<24;row++)
	        	{
			    	for(bytes=0;bytes<3;bytes++)
			    	{
			        	for(col=0;col<8;col++)
			        		if((*(pTemp+row*3+bytes)>>(7-col))&1)
			        			PutPixel(x+bytes*8+col,y+row,forecolor);
			                else 
			                	PutPixel(x+bytes*8+col,y+row,bkcolor);
			    	}
				}//for(row=0;   
	  		}//else	
	   		x+=24;
		}//if(qm>=0x80){ 
		else
		{ // not hz
		   dis8x16chart( x,y+3,qm, transparence,forecolor, bkcolor );
		   x+=8;
	    }
    }// while(qm=*s++)
}
#endif

#if  0
void PutCh12x12Str(short x,short y,char *s,unsigned char transparence,unsigned char forecolor,unsigned char bkcolor)
{
     //transparence=1 is transparence;
     unsigned char *pTemp;	
     long se;
     unsigned char qm,wm;
     short row,col;
     while( (qm=*s++)!=0 ){
        if(qm>=0x80){
           qm=qm-0xa0-15-1;
           wm=*s++;
           wm=wm-0xa0-1;
           se=(qm*94L+wm)*18L;
           pTemp=(unsigned char *)(hzk12+se);//pHzDot+se;
           if(transparence){
             // _setcolor(forecolor);	
	      for(row=0;row<6;row++){
	         for(col=0;col<8;col++)if( (*(pTemp+row*3)>>(7-col))&1)PutPixel(x+col,y+row*2,forecolor);
	         for(col=0;col<4;col++)if( (*(pTemp+row*3+1)>>(7-col))&1)PutPixel(x+8+col,y+row*2,forecolor);
	         for(col=0;col<8;col++)if( (*(pTemp+row*3+2)>>(7-col))&1)PutPixel(x+col,y+row*2+1,forecolor);
	         for(col=0;col<4;col++)if( (*(pTemp+row*3+1)>>(3-col))&1)PutPixel(x+8+col,y+row*2+1,forecolor);
	      }//for(row=0;
	   }
	   else {
	      for(row=0;row<6;row++){
	         for(col=0;col<8;col++)if( (*(pTemp+row*3)>>(7-col))&1)PutPixel(x+col,y+row*2,forecolor);
                                       else PutPixel(x+col,y+row*2,bkcolor);
	         for(col=0;col<4;col++)if( (*(pTemp+row*3+1)>>(7-col))&1)PutPixel(x+8+col,y+row*2,forecolor);
	                               else PutPixel(x+8+col,y+row*2,bkcolor);
	         for(col=0;col<8;col++)if( (*(pTemp+row*3+2)>>(7-col))&1)PutPixel(x+col,y+row*2+1,forecolor);
	                               else PutPixel(x+col,y+row*2+1,bkcolor);
	         for(col=0;col<4;col++)if( (*(pTemp+row*3+1)>>(3-col))&1)PutPixel(x+8+col,y+row*2+1,forecolor);
	                               else PutPixel(x+8+col,y+row*2+1,bkcolor);
	      }//for(row=0;
	   }//else	
   	   x+=13;
	 }//if(qm>=0x80){  
	 else {
	   // dis5x7chart(x,y+4,qm, transparence,forecolor,bkcolor );
	    x+=6;
	 }
   }// while(qm=*s++)
}
#endif