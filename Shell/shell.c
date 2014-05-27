/*************************************************************************
#    FileName: shell.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: shell组件，用来调试函数
#  LastChange: 2014-02-05 22:15:47
*************************************************************************/
/* Includes ------------------------------------------------------------*/
#include "shell.h"
/* Types ---------------------------------------------------------------*/
/* Constants -----------------------------------------------------------*/
/* Define --------------------------------------------------------------*/
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif
/* Variables -----------------------------------------------------------*/
/* Functions prototypes ------------------------------------------------*/

/* Functions -----------------------------------------------------------*/
/**
  * @brief  两个字符串对比.
  * @param  None.
  * @retval None.
  * @verify .
  */
u8 SHL_CmpStr(u8*str1,u8 *str2)
{
	while(1){
		if(*str1!=*str2) return FALSE;//不相等
		if(*str1=='\0') break;//对比完成了.
		str1++;
		str2++;
	}
	return TRUE;//两个字符串相等
}
/**
  * @brief  把str1的内容copy到str2.
  * @param  None.
  * @retval None.
  * @verify .
  */
void SHL_CopyStr(u8 *str1,u8 *str2)
{
	while(1){										   
		*str2 = *str1;
		if(*str1=='\0')break;
		str1++;
		str2++;
	}
}
/**
  * @brief  str1复制num个字符给str2.
  * @param  None.
  * @retval None.
  * @verify .
  */
void SHL_CopyStrSeg(u8 *str1, u8 *str2, u16 num)
{
	while(num--){										   
		*str2 = *str1;
		str1++;
		str2++;
	}
}
/**
  * @brief  获得字符串长度.
  * @param  None.
  * @retval None.
  * @verify .
  */
void SHL_GetStrLen(u8 *str, u32 *len)
{
	*len = 0;
	while(1){
		if(*str=='\0') break;
		(*len)++;
		str++;
	}
}
/**
  * @brief  求次方.
  * @param  None.
  * @retval None.
  * @verify .
  */
u32 SHL_Power(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	   
/**
  * @brief  清除num个字符，变为空格.
  * @param  None.
  * @retval None.
  * @verify .
  */
u8 SHL_ClearStr(u8* str,u32 num)
{
	while(num--){
		*str=' ';
		str++;
	}
	return 0;
}
/**
  * @brief  字符串转为数字.
  * @param  None.
  * @retval None.
  * @verify .
  */
u8 SHL_Str2Num(u8 *str, u32 *res)
{
	u32 t;
	u8 bnum=0;
	u8 *p;		  
	u8 hexdec=10;
	p=str;
	*res=0;
	while(1){
		if((*p<='9'&&*p>='0')||(*p<='F'&&*p>='A')||(*p=='X'&&bnum==1))//参数合法
		{
			if(*p>='A')hexdec=16;	//字符串中存在字母,为16进制格式.
			bnum++;					//位数增加.
		}else if(*p=='\0')break;	//碰到结束符,退出.
		else return 1;				//不全是十进制或者16进制数据.
		p++; 
	} 
	p=str;			    //重新定位到字符串开始的地址.
	if(hexdec==16)		//16进制数据
	{
		if(bnum<3)return 2;			//位数小于3，直接退出.因为0X就占了2个,如果0X后面不跟数据,则该数据非法.
		if(*p=='0' && (*(p+1)=='X'))//必须以'0X'开头.
		{
			p+=2;	//偏移到数据起始地址.
			bnum-=2;//减去偏移量	 
		}else return 3;//起始头的格式不对
	}else if(bnum==0)return 4;//位数为0，直接退出.	  
	while(1)
	{
		if(bnum)bnum--;
		if(*p<='9'&&*p>='0')t=*p-'0';	//得到数字的值
		else t=*p-'A'+10;				//得到A~F对应的值	    
		*res+=t*SHL_Power(hexdec,bnum);		   
		p++;
		if(*p=='\0')break;//数据都查完了.	
	}
	return 0;//成功转换
}
//查找函数的参数 传入函数的参数字符串
//返回 参数的个数
u8 SHL_GetParmNum(u8* str)
{
	u8 ParmNum=1;
	u8 IfStr=0;
	u8 ZeroParmTable1[]="()";
	u8 ZeroParmTable2[]="(void)";
	// 0参数判断
	if(SHL_CmpStr(str,ZeroParmTable1) || SHL_CmpStr(str,ZeroParmTable2)) return 0;
	while(1)
	{
		if((*str==')'||*str=='\0')) break;
		if(*str==',' && IfStr==0) ParmNum++;
		if(*str=='"' && IfStr==1)
		{
			IfStr=0;
		}
		else if(*str=='"') IfStr=1;
		str++;
	}
	return ParmNum;
}
/**
  * @brief  分析参数信息.
  * @param  None.
  * @retval None.
  * @verify .
  */
u8 SHL_ProcessPara(u8* str,struct UARTSHELL_DEV *shl_dev)
{
	u8 i;
	u8 ParmPos[MAX_PARM];
	u8 Pos=0;
	u8 Temp[PARM_LEN];
	u32 NumParm;
	u8 Results;
	//第一步 给出各个参数在Buffer中的位置
	for(i=1;i<shl_dev->pnum+1;i++)
	{
		if(shl_dev->PInfo.Type[i]==2)
		{
			ParmPos[i]=Pos;
			Pos+=shl_dev->PInfo.Long[i]+2;//只存储字符串
		}
	}
  //存入数字
	for(i=1;i<shl_dev->pnum+1;i++)
	{
		if(shl_dev->PInfo.Type[i]==1) //数字
		{
			SHL_CopyStr(str+shl_dev->PInfo.OffSet[i],Temp);
			Temp[shl_dev->PInfo.Long[i]]='\0';
			Results=SHL_Str2Num(Temp,&NumParm);
			if(Results!=0) return 1;
			shl_dev->PInfo.ParmData[i]=NumParm;
		}
		else //字符串
		{
			SHL_CopyStr(str+shl_dev->PInfo.OffSet[i],Temp);
			Temp[shl_dev->PInfo.Long[i]]='\0';
			SHL_CopyStr(Temp,shl_dev->PInfo.ParmBuf+ParmPos[i]);
			shl_dev->PInfo.ParmData[i]=(u32)&(shl_dev->PInfo.ParmBuf)+ParmPos[i];
		}
	}
	return 0;
}
//获得函数参数在参数字符串中的偏移和长度
u8 SHL_GetParmInfo(u8* str,struct UARTSHELL_DEV *shl_dev)
{
	u8 i=0;
	u8 j=0;
	u8 ParmNum=1;
	u8 IfStr=0;
	u32 len;
	u8* OrgStrPtr=str;
	//获得参数个数
	shl_dev->PInfo.OffSet[1]=1;
	while(1)
	{
		if((*str==')'||*str=='\0')) break;
		if(*str==',' && IfStr==0)
		{
			ParmNum++;
			shl_dev->PInfo.OffSet[ParmNum]=i+1;
		}
		if((*str>='0' && *str<='9') && IfStr==0)
		{
			shl_dev->PInfo.Type[ParmNum]=1;
		}
		if(*str=='"' && IfStr==1)
		{
				IfStr=0;
		}
		else if(*str=='"')//碰到" 开始接收string
		{
			IfStr=1;
			shl_dev->PInfo.Type[ParmNum]=2; //确认是字符串参数
		}
		str++;
		i++;
		
	}
	//判断参数的开始位置
	for(i=1;i<ParmNum+1;i++)
	{
		if(shl_dev->PInfo.Type[i]==2)shl_dev->PInfo.OffSet[i]++;
	}
	//开始位置寻找结束
	i=0;
	str=OrgStrPtr;       //还原字符串
	SHL_GetStrLen(str,&len); //获得字符串长度
	//获得每个参数的长度
	for(i=1;i<ParmNum+1;i++)
	{
		str=OrgStrPtr+shl_dev->PInfo.OffSet[i];
		for(j=shl_dev->PInfo.OffSet[i];j<len;j++)
		{
			if(shl_dev->PInfo.Type[i]==1)
			{
				if(*str==',' || *str=='"' || *str==')') //寻找结束标志
				{
					shl_dev->PInfo.Long[i]=j-shl_dev->PInfo.OffSet[i];
					break;	
				}
			}
			else
			{
				if(*str=='"')
				{
					shl_dev->PInfo.Long[i]=j-shl_dev->PInfo.OffSet[i]; //记录长度信息
					break;	
				}
			}
			str++;
		}
	}
	return 0;
	//将参数变形，存入Buffer并标记
}
//检查字符串是否合法 并且分离函数名字符串和参数字符串
//str     源字符串
//FunName 函数名称字符串
//FunParm 函数参数字符串
/**
  * @brief  检测字符串合法性，并分离函数名字符串和参数字符串.
  * @param  str:字符串
  * 		FunName:函数名称字符串
  * 		FunParm:函数参数字符串.
  * @retval None.
  * @verify .
  */
u8 SHL_CheckStr(u8* str,u8* FunName,u8* FunParm)
{
	u32 len=0;
	u8* StrStartPtr=str; //源字符串指针
	u8* p;
	u8 i=0;
	u8 FoverLeft=0;   //括号的个数
	u8 FoverRight=0;  //括号的个数
	u8 SpaceOffset=0; //第一个空格前的偏移
	//长度判断
	SHL_GetStrLen(str,&len);
	if(len>(PARM_LEN+MAX_FNAME_LEN)) return 1;
	//括号判断
	for(i=0;i<len;i++){
		if(*str=='(')  FoverLeft++;
		if(*str==')')  FoverRight++;
		str++;
	}
	/* 括号错误，不是函数，可能是命令 */
	if(FoverLeft==0 || FoverRight==0 || FoverLeft!=FoverRight)  return 4;
	str=StrStartPtr; //还原字符串指针
	for(i=0;i<len;i++){//获得第一个左括号位置
		if(*str=='(') break;
		str++;
	}
	FoverLeft=i;
	//跳过第一个空格，检测第一个字母是否为合法字母
	str=StrStartPtr;
	for(i=0;i<len;i++){
		if(*str==' ' && (i<FoverLeft)) SpaceOffset=i+1;
		str++;
	}
	str=StrStartPtr;
	str=(str+SpaceOffset);
	p=str;
	if(!((*str>='a' && *str<='z')||(*str>='A' && *str<='Z')|| (*str=='_'))) return 5;
	//寻找第一个(
	str=StrStartPtr;
	for(i=0;i<len;i++){
		if(*str=='(') break;
		str++;
	}
	str=p;
	SHL_CopyStrSeg(str,FunName,i); //拷贝FunName字符串
	SHL_CopyStr(str+i-SpaceOffset,FunParm);
	*(FunName+i-SpaceOffset)='\0';
	SHL_GetStrLen(FunName,&len);
	if(len>MAX_FNAME_LEN){//检测长度是否超出最大
		printf("\r\n函数长度超出最大");
		return 6; 
	}
	SHL_GetStrLen(FunParm,&len);
	if(len>PARM_LEN){
		printf("\r\n函数变量超出最大");
  		return 6;
	}
	return 0;
}
//命令表
u8 *SHL_CMD_TABLE[]=
{
	"?",
	"help",
	"list",
	"shutdown",
	"reset"
};

//主执行函数
u8 SHL_Scan(void)
{
	u8 i=0;
	u8 Result=0;
	u8 len=0;
	u8 LocalFunName[MAX_FNAME_LEN];   //本地函数名称字符串
	u8 LocalFunParm[PARM_LEN];        //本地参数名称字符串
	u8 RecFunName[MAX_FNAME_LEN];     //接收到的函数名称字符串
	u8 RecFunParm[PARM_LEN];          //接收到的函数参数字符串
	if((SHL_Dev.USART_STAT&0x8000)==0) return 1;
	len=SHL_Dev.USART_STAT&0x3fff;	 //得到此次接收到的数据长度
	SHL_Dev.USART_BUF[len]='\0';	   //在末尾加入结束符. 
	Result=SHL_CheckStr(SHL_Dev.USART_BUF,RecFunName,RecFunParm); //检测输入字符串是否为函数格式，如果是，分离名称和参数字符串
	if(Result!=0){//不是C语言函数格式的输入
		for(i=0;i<MAX_COMMON;i++){//支持3个系统指令
			if(SHL_CmpStr(SHL_Dev.USART_BUF,SHL_CMD_TABLE[i]))break;
		}
		switch(i){					   
			case 0:
			case 1://帮助指令
				printf("\r\n---------------------------SHELL V1.0--------------------------");
				printf("\r\n?:    获取帮助信息");
				printf("\r\nhelp: 获取帮助信息");
				printf("\r\nlist: 可用的函数列表");
				printf("\r\n请按照程序编写格式输入函数名及参数并以回车键结束.");    
				printf("------------------------------END------------------------------");
			case 2:
				printf("\r\n----------------------------函数清单---------------------------");
				for(i=0;i<SHL_Dev.fnum;i++)printf("\r\n%s",SHL_Dev.funs[i].name);
				printf("\r\n------------------------------END------------------------------");
				return 0;
			case 3:
				//NVIC_SystemReset ();
				return 0;
			case 4:
				return 0;
			default: 
				printf("\r\n无法识别的指令");
				return 1;
		}
	}
	  //检查本地是否有此函数
	for(i=0;i<SHL_Dev.fnum;i++){
		Result=SHL_CheckStr((u8*)SHL_Dev.funs[i].name,LocalFunName,LocalFunParm);//得到本地函数名及参数个数
		if(Result!=0){
			printf("\r\n本地函数解析错误 %d",Result);
			return 2;//本地解析有误
		}
		if(SHL_CmpStr(LocalFunName,RecFunName)){//输入函数名和本地函数名匹配
			SHL_Dev.id=i;//记录函数ID.
			break;//跳出.
		}	
	}
	if(i==SHL_Dev.fnum){//输入函数名和本地函数名无法匹配
		printf("\r\n无法匹配函数");
		return 5;
	}
	//检测参数输入是否正确
	if(SHL_GetParmNum(RecFunParm)!=SHL_GetParmNum(LocalFunParm)){
		printf("\r\n参数输入不正确 本地%d 输入%d",SHL_GetParmNum(LocalFunParm),SHL_GetParmNum(RecFunParm));
		return 6;
	}
	SHL_Dev.pnum=SHL_GetParmNum(RecFunParm); //获取此次要执行函数的参数个数
	SHL_GetParmInfo(RecFunParm,&SHL_Dev);    //获得参数信息偏移和大小
	SHL_ProcessPara(RecFunParm,&SHL_Dev);   //处理函数信息
	//检查正确 开始分析参数
	printf("\r\n%s(",RecFunName);//输出正要执行的函数名
	for(i=1;i<SHL_Dev.pnum+1;i++) //输入要执行的参数
	{
		if(SHL_Dev.PInfo.Type[i]==1) //数字
		{
			printf("%d",SHL_Dev.PInfo.ParmData[i]);
		}
		else  //字符串
		{
			printf("%c",'"');			 
			printf("%s",(u8*)SHL_Dev.PInfo.ParmData[i]);
			printf("%c",'"');
		}
		if(i!=SHL_Dev.pnum)printf(",");
	}
	printf(")\r\n");	
	//开始执行
	switch(SHL_Dev.pnum){
		case 0://无参数(void类型)											  
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)();
			printf("FunRet: %d",Result);
			break;
	    case 1://有1个参数
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1]);
			printf("FunRet: %d",Result);
			break;
		case 2: //有2个参数
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2]);
			printf("FunRet: %d",Result);
			break;
		case 3: //有3个参数
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2],SHL_Dev.PInfo.ParmData[3]);
			printf("FunRet: %d",Result);
			break;
		case 4: //有4个参数
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2],SHL_Dev.PInfo.ParmData[3],SHL_Dev.PInfo.ParmData[4]);
			printf("FunRet: %d",Result);
			break;		
		case 5: //有5个参数
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2],SHL_Dev.PInfo.ParmData[3],SHL_Dev.PInfo.ParmData[4],\
			SHL_Dev.PInfo.ParmData[5]);
			printf("FunRet: %d",Result);
			break;		
		case 6: //有6个参数
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2],SHL_Dev.PInfo.ParmData[3],SHL_Dev.PInfo.ParmData[4],\
			SHL_Dev.PInfo.ParmData[5],SHL_Dev.PInfo.ParmData[6]);
			printf("FunRet: %d",Result);
			break;	
		default :
			printf("输入错误\r\n");
			break;
	}
	return 0;
}
//接收1个字符串 如果接收成功，调用scan() 函数一次
void SHL_ProcessCh(u8 ch)
{
	if((SHL_Dev.USART_STAT&0x8000)==0){
		if(SHL_Dev.USART_STAT&0x4000){//接收到了0x0D
			if(ch!=0x0A){
				SHL_Dev.USART_STAT=0; //没有换行，接收错误
			}else{
				SHL_Dev.cnum++;
				SHL_Dev.USART_STAT|=0x8000;    //接收完成
				SHL_Scan();                    //执行主程序
				printf("\r\n<UartShell:%u>",SHL_Dev.cnum);
				SHL_ClearStr(SHL_Dev.USART_BUF,sizeof(SHL_Dev.USART_BUF));
				SHL_Dev.USART_STAT=0;
			}
		}else{
			if(ch==0x0d) SHL_Dev.USART_STAT|=0x4000;//接收到了回车键
			else{
				if(ch == 0x08){//删除号
					SHL_Dev.USART_BUF[SHL_Dev.USART_STAT&0x3FFF]=0;
					SHL_Dev.USART_STAT--;
				}else{
					SHL_Dev.USART_BUF[SHL_Dev.USART_STAT&0x3FFF]=ch;
					SHL_Dev.USART_STAT++;
					SHL_Dev.USART_BUF[SHL_Dev.USART_STAT&0x3FFF]=0;
				}
				printf("%c",ch);
				if((SHL_Dev.USART_STAT&0x3FFF)>(MAX_FNAME_LEN+PARM_LEN-1)) SHL_Dev.USART_STAT=0; //接收数据错误,重新开始接收	  
			}
		}
	}
}

/*********************************END OF FILE****************************/
