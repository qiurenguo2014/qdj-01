/*************************************************************************
#    FileName: shell.c
#      Author: Allen
#       Email: qiurenguo@gmail.com
#    HomePage: Allen
#       Brief: shell������������Ժ���
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
  * @brief  �����ַ����Ա�.
  * @param  None.
  * @retval None.
  * @verify .
  */
u8 SHL_CmpStr(u8*str1,u8 *str2)
{
	while(1){
		if(*str1!=*str2) return FALSE;//�����
		if(*str1=='\0') break;//�Ա������.
		str1++;
		str2++;
	}
	return TRUE;//�����ַ������
}
/**
  * @brief  ��str1������copy��str2.
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
  * @brief  str1����num���ַ���str2.
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
  * @brief  ����ַ�������.
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
  * @brief  ��η�.
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
  * @brief  ���num���ַ�����Ϊ�ո�.
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
  * @brief  �ַ���תΪ����.
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
		if((*p<='9'&&*p>='0')||(*p<='F'&&*p>='A')||(*p=='X'&&bnum==1))//�����Ϸ�
		{
			if(*p>='A')hexdec=16;	//�ַ����д�����ĸ,Ϊ16���Ƹ�ʽ.
			bnum++;					//λ������.
		}else if(*p=='\0')break;	//����������,�˳�.
		else return 1;				//��ȫ��ʮ���ƻ���16��������.
		p++; 
	} 
	p=str;			    //���¶�λ���ַ�����ʼ�ĵ�ַ.
	if(hexdec==16)		//16��������
	{
		if(bnum<3)return 2;			//λ��С��3��ֱ���˳�.��Ϊ0X��ռ��2��,���0X���治������,������ݷǷ�.
		if(*p=='0' && (*(p+1)=='X'))//������'0X'��ͷ.
		{
			p+=2;	//ƫ�Ƶ�������ʼ��ַ.
			bnum-=2;//��ȥƫ����	 
		}else return 3;//��ʼͷ�ĸ�ʽ����
	}else if(bnum==0)return 4;//λ��Ϊ0��ֱ���˳�.	  
	while(1)
	{
		if(bnum)bnum--;
		if(*p<='9'&&*p>='0')t=*p-'0';	//�õ����ֵ�ֵ
		else t=*p-'A'+10;				//�õ�A~F��Ӧ��ֵ	    
		*res+=t*SHL_Power(hexdec,bnum);		   
		p++;
		if(*p=='\0')break;//���ݶ�������.	
	}
	return 0;//�ɹ�ת��
}
//���Һ����Ĳ��� ���뺯���Ĳ����ַ���
//���� �����ĸ���
u8 SHL_GetParmNum(u8* str)
{
	u8 ParmNum=1;
	u8 IfStr=0;
	u8 ZeroParmTable1[]="()";
	u8 ZeroParmTable2[]="(void)";
	// 0�����ж�
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
  * @brief  ����������Ϣ.
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
	//��һ�� ��������������Buffer�е�λ��
	for(i=1;i<shl_dev->pnum+1;i++)
	{
		if(shl_dev->PInfo.Type[i]==2)
		{
			ParmPos[i]=Pos;
			Pos+=shl_dev->PInfo.Long[i]+2;//ֻ�洢�ַ���
		}
	}
  //��������
	for(i=1;i<shl_dev->pnum+1;i++)
	{
		if(shl_dev->PInfo.Type[i]==1) //����
		{
			SHL_CopyStr(str+shl_dev->PInfo.OffSet[i],Temp);
			Temp[shl_dev->PInfo.Long[i]]='\0';
			Results=SHL_Str2Num(Temp,&NumParm);
			if(Results!=0) return 1;
			shl_dev->PInfo.ParmData[i]=NumParm;
		}
		else //�ַ���
		{
			SHL_CopyStr(str+shl_dev->PInfo.OffSet[i],Temp);
			Temp[shl_dev->PInfo.Long[i]]='\0';
			SHL_CopyStr(Temp,shl_dev->PInfo.ParmBuf+ParmPos[i]);
			shl_dev->PInfo.ParmData[i]=(u32)&(shl_dev->PInfo.ParmBuf)+ParmPos[i];
		}
	}
	return 0;
}
//��ú��������ڲ����ַ����е�ƫ�ƺͳ���
u8 SHL_GetParmInfo(u8* str,struct UARTSHELL_DEV *shl_dev)
{
	u8 i=0;
	u8 j=0;
	u8 ParmNum=1;
	u8 IfStr=0;
	u32 len;
	u8* OrgStrPtr=str;
	//��ò�������
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
		else if(*str=='"')//����" ��ʼ����string
		{
			IfStr=1;
			shl_dev->PInfo.Type[ParmNum]=2; //ȷ�����ַ�������
		}
		str++;
		i++;
		
	}
	//�жϲ����Ŀ�ʼλ��
	for(i=1;i<ParmNum+1;i++)
	{
		if(shl_dev->PInfo.Type[i]==2)shl_dev->PInfo.OffSet[i]++;
	}
	//��ʼλ��Ѱ�ҽ���
	i=0;
	str=OrgStrPtr;       //��ԭ�ַ���
	SHL_GetStrLen(str,&len); //����ַ�������
	//���ÿ�������ĳ���
	for(i=1;i<ParmNum+1;i++)
	{
		str=OrgStrPtr+shl_dev->PInfo.OffSet[i];
		for(j=shl_dev->PInfo.OffSet[i];j<len;j++)
		{
			if(shl_dev->PInfo.Type[i]==1)
			{
				if(*str==',' || *str=='"' || *str==')') //Ѱ�ҽ�����־
				{
					shl_dev->PInfo.Long[i]=j-shl_dev->PInfo.OffSet[i];
					break;	
				}
			}
			else
			{
				if(*str=='"')
				{
					shl_dev->PInfo.Long[i]=j-shl_dev->PInfo.OffSet[i]; //��¼������Ϣ
					break;	
				}
			}
			str++;
		}
	}
	return 0;
	//���������Σ�����Buffer�����
}
//����ַ����Ƿ�Ϸ� ���ҷ��뺯�����ַ����Ͳ����ַ���
//str     Դ�ַ���
//FunName ���������ַ���
//FunParm ���������ַ���
/**
  * @brief  ����ַ����Ϸ��ԣ������뺯�����ַ����Ͳ����ַ���.
  * @param  str:�ַ���
  * 		FunName:���������ַ���
  * 		FunParm:���������ַ���.
  * @retval None.
  * @verify .
  */
u8 SHL_CheckStr(u8* str,u8* FunName,u8* FunParm)
{
	u32 len=0;
	u8* StrStartPtr=str; //Դ�ַ���ָ��
	u8* p;
	u8 i=0;
	u8 FoverLeft=0;   //���ŵĸ���
	u8 FoverRight=0;  //���ŵĸ���
	u8 SpaceOffset=0; //��һ���ո�ǰ��ƫ��
	//�����ж�
	SHL_GetStrLen(str,&len);
	if(len>(PARM_LEN+MAX_FNAME_LEN)) return 1;
	//�����ж�
	for(i=0;i<len;i++){
		if(*str=='(')  FoverLeft++;
		if(*str==')')  FoverRight++;
		str++;
	}
	/* ���Ŵ��󣬲��Ǻ��������������� */
	if(FoverLeft==0 || FoverRight==0 || FoverLeft!=FoverRight)  return 4;
	str=StrStartPtr; //��ԭ�ַ���ָ��
	for(i=0;i<len;i++){//��õ�һ��������λ��
		if(*str=='(') break;
		str++;
	}
	FoverLeft=i;
	//������һ���ո񣬼���һ����ĸ�Ƿ�Ϊ�Ϸ���ĸ
	str=StrStartPtr;
	for(i=0;i<len;i++){
		if(*str==' ' && (i<FoverLeft)) SpaceOffset=i+1;
		str++;
	}
	str=StrStartPtr;
	str=(str+SpaceOffset);
	p=str;
	if(!((*str>='a' && *str<='z')||(*str>='A' && *str<='Z')|| (*str=='_'))) return 5;
	//Ѱ�ҵ�һ��(
	str=StrStartPtr;
	for(i=0;i<len;i++){
		if(*str=='(') break;
		str++;
	}
	str=p;
	SHL_CopyStrSeg(str,FunName,i); //����FunName�ַ���
	SHL_CopyStr(str+i-SpaceOffset,FunParm);
	*(FunName+i-SpaceOffset)='\0';
	SHL_GetStrLen(FunName,&len);
	if(len>MAX_FNAME_LEN){//��ⳤ���Ƿ񳬳����
		printf("\r\n�������ȳ������");
		return 6; 
	}
	SHL_GetStrLen(FunParm,&len);
	if(len>PARM_LEN){
		printf("\r\n���������������");
  		return 6;
	}
	return 0;
}
//�����
u8 *SHL_CMD_TABLE[]=
{
	"?",
	"help",
	"list",
	"shutdown",
	"reset"
};

//��ִ�к���
u8 SHL_Scan(void)
{
	u8 i=0;
	u8 Result=0;
	u8 len=0;
	u8 LocalFunName[MAX_FNAME_LEN];   //���غ��������ַ���
	u8 LocalFunParm[PARM_LEN];        //���ز��������ַ���
	u8 RecFunName[MAX_FNAME_LEN];     //���յ��ĺ��������ַ���
	u8 RecFunParm[PARM_LEN];          //���յ��ĺ��������ַ���
	if((SHL_Dev.USART_STAT&0x8000)==0) return 1;
	len=SHL_Dev.USART_STAT&0x3fff;	 //�õ��˴ν��յ������ݳ���
	SHL_Dev.USART_BUF[len]='\0';	   //��ĩβ���������. 
	Result=SHL_CheckStr(SHL_Dev.USART_BUF,RecFunName,RecFunParm); //��������ַ����Ƿ�Ϊ������ʽ������ǣ��������ƺͲ����ַ���
	if(Result!=0){//����C���Ժ�����ʽ������
		for(i=0;i<MAX_COMMON;i++){//֧��3��ϵͳָ��
			if(SHL_CmpStr(SHL_Dev.USART_BUF,SHL_CMD_TABLE[i]))break;
		}
		switch(i){					   
			case 0:
			case 1://����ָ��
				printf("\r\n---------------------------SHELL V1.0--------------------------");
				printf("\r\n?:    ��ȡ������Ϣ");
				printf("\r\nhelp: ��ȡ������Ϣ");
				printf("\r\nlist: ���õĺ����б�");
				printf("\r\n�밴�ճ����д��ʽ���뺯�������������Իس�������.");    
				printf("------------------------------END------------------------------");
			case 2:
				printf("\r\n----------------------------�����嵥---------------------------");
				for(i=0;i<SHL_Dev.fnum;i++)printf("\r\n%s",SHL_Dev.funs[i].name);
				printf("\r\n------------------------------END------------------------------");
				return 0;
			case 3:
				//NVIC_SystemReset ();
				return 0;
			case 4:
				return 0;
			default: 
				printf("\r\n�޷�ʶ���ָ��");
				return 1;
		}
	}
	  //��鱾���Ƿ��д˺���
	for(i=0;i<SHL_Dev.fnum;i++){
		Result=SHL_CheckStr((u8*)SHL_Dev.funs[i].name,LocalFunName,LocalFunParm);//�õ����غ���������������
		if(Result!=0){
			printf("\r\n���غ����������� %d",Result);
			return 2;//���ؽ�������
		}
		if(SHL_CmpStr(LocalFunName,RecFunName)){//���뺯�����ͱ��غ�����ƥ��
			SHL_Dev.id=i;//��¼����ID.
			break;//����.
		}	
	}
	if(i==SHL_Dev.fnum){//���뺯�����ͱ��غ������޷�ƥ��
		printf("\r\n�޷�ƥ�亯��");
		return 5;
	}
	//�����������Ƿ���ȷ
	if(SHL_GetParmNum(RecFunParm)!=SHL_GetParmNum(LocalFunParm)){
		printf("\r\n�������벻��ȷ ����%d ����%d",SHL_GetParmNum(LocalFunParm),SHL_GetParmNum(RecFunParm));
		return 6;
	}
	SHL_Dev.pnum=SHL_GetParmNum(RecFunParm); //��ȡ�˴�Ҫִ�к����Ĳ�������
	SHL_GetParmInfo(RecFunParm,&SHL_Dev);    //��ò�����Ϣƫ�ƺʹ�С
	SHL_ProcessPara(RecFunParm,&SHL_Dev);   //��������Ϣ
	//�����ȷ ��ʼ��������
	printf("\r\n%s(",RecFunName);//�����Ҫִ�еĺ�����
	for(i=1;i<SHL_Dev.pnum+1;i++) //����Ҫִ�еĲ���
	{
		if(SHL_Dev.PInfo.Type[i]==1) //����
		{
			printf("%d",SHL_Dev.PInfo.ParmData[i]);
		}
		else  //�ַ���
		{
			printf("%c",'"');			 
			printf("%s",(u8*)SHL_Dev.PInfo.ParmData[i]);
			printf("%c",'"');
		}
		if(i!=SHL_Dev.pnum)printf(",");
	}
	printf(")\r\n");	
	//��ʼִ��
	switch(SHL_Dev.pnum){
		case 0://�޲���(void����)											  
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)();
			printf("FunRet: %d",Result);
			break;
	    case 1://��1������
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1]);
			printf("FunRet: %d",Result);
			break;
		case 2: //��2������
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2]);
			printf("FunRet: %d",Result);
			break;
		case 3: //��3������
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2],SHL_Dev.PInfo.ParmData[3]);
			printf("FunRet: %d",Result);
			break;
		case 4: //��4������
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2],SHL_Dev.PInfo.ParmData[3],SHL_Dev.PInfo.ParmData[4]);
			printf("FunRet: %d",Result);
			break;		
		case 5: //��5������
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2],SHL_Dev.PInfo.ParmData[3],SHL_Dev.PInfo.ParmData[4],\
			SHL_Dev.PInfo.ParmData[5]);
			printf("FunRet: %d",Result);
			break;		
		case 6: //��6������
			Result=(*(u32(*)())SHL_Dev.funs[SHL_Dev.id].func)(SHL_Dev.PInfo.ParmData[1],SHL_Dev.PInfo.ParmData[2],SHL_Dev.PInfo.ParmData[3],SHL_Dev.PInfo.ParmData[4],\
			SHL_Dev.PInfo.ParmData[5],SHL_Dev.PInfo.ParmData[6]);
			printf("FunRet: %d",Result);
			break;	
		default :
			printf("�������\r\n");
			break;
	}
	return 0;
}
//����1���ַ��� ������ճɹ�������scan() ����һ��
void SHL_ProcessCh(u8 ch)
{
	if((SHL_Dev.USART_STAT&0x8000)==0){
		if(SHL_Dev.USART_STAT&0x4000){//���յ���0x0D
			if(ch!=0x0A){
				SHL_Dev.USART_STAT=0; //û�л��У����մ���
			}else{
				SHL_Dev.cnum++;
				SHL_Dev.USART_STAT|=0x8000;    //�������
				SHL_Scan();                    //ִ��������
				printf("\r\n<UartShell:%u>",SHL_Dev.cnum);
				SHL_ClearStr(SHL_Dev.USART_BUF,sizeof(SHL_Dev.USART_BUF));
				SHL_Dev.USART_STAT=0;
			}
		}else{
			if(ch==0x0d) SHL_Dev.USART_STAT|=0x4000;//���յ��˻س���
			else{
				if(ch == 0x08){//ɾ����
					SHL_Dev.USART_BUF[SHL_Dev.USART_STAT&0x3FFF]=0;
					SHL_Dev.USART_STAT--;
				}else{
					SHL_Dev.USART_BUF[SHL_Dev.USART_STAT&0x3FFF]=ch;
					SHL_Dev.USART_STAT++;
					SHL_Dev.USART_BUF[SHL_Dev.USART_STAT&0x3FFF]=0;
				}
				printf("%c",ch);
				if((SHL_Dev.USART_STAT&0x3FFF)>(MAX_FNAME_LEN+PARM_LEN-1)) SHL_Dev.USART_STAT=0; //�������ݴ���,���¿�ʼ����	  
			}
		}
	}
}

/*********************************END OF FILE****************************/
