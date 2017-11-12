#include "pid.h"

PID pid; //´æ·ÅPIDËã·¨ËùÐèÒªµÄÊý¾Ý


void PID_Calc()  //pid¼ÆËã
{
 float DelEk;
	float ti,ki;
	float td;
	float kd;
	float out;
 if(pid.C10ms<(pid.T))  //¼ÆËãÖÜÆÚÎ´µ½
 {
    return ;
 }

 pid.Ek=pid.Sv-pid.Pv;   //µÃµ½µ±Ç°µÄÆ«²îÖµ
 pid.Pout=pid.Kp*pid.Ek;      //±ÈÀýÊä³ö
 
 pid.SEk+=pid.Ek;        //ÀúÊ·Æ«²î×ÜºÍ
 
 DelEk=pid.Ek-pid.Ek_1;  //×î½üÁ½´ÎÆ«²îÖ®²î
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //»ý·ÖÊä³ö

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;    //Î¢·ÖÊä³ö
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //±¾´ÎµÄ¼ÆËã½á¹û
 pid.Ek_1=pid.Ek;  //¸üÐÂÆ«²î
 
 pid.C10ms=0;
}


void PID_Calc_overshoot()  //pid¼ÆËã-¹ý³å´¦Àí¿
{
 float DelEk;
	float ti,ki;
	float td;
	float kd;
	float out;
  float SvSave;
 if(pid.C10ms<(pid.T))  //¼ÆËãÖÜÆÚÎ´µ½£¬pid.C10msÊÇÃ¿1ms×Ô¼ÓÒ»£¬ËùÒÔ¿ÉÒÔÓÃÀ´ºÍPIDµÄ¼ÆËãÊ±¼äÀ´¶Ô±È
 {
    return ;
 }

//
//²Â²â:¸Ã³ÌÐò²»ÍêÕû£¬ÀíÂÛÉÏÓ¦¸ÃÃ»½øÐÐÒ»´ÎPID¼ÆËã£¬Ôòpid.times++,¶øÇÒÔÚÎÂ¶È<65¡æµÄÊ±ºò£¬pid.timesºãµÈÓÚ0£¬¼´´ËÊ±²»½øÐÐ¹ý³å´¦Àí
//ÔÚ¶¨Ê±Æ÷3µÄÖÐ¶ÏÑ­»·ÖÐ¶Ôpid.times½øÐÐ++²Ù×÷£¬Ã¿1sÖÐpid.times×Ô¼ÓÒ»£¬¶Ô¹ý³åµÄ´¦ÀíÖ÷ÒªÊÇÍ¨¹ýÊ±¼äÀ´¿ØÖÆ
//¶ø£¬µ±ÎÂ¶ÈÉý¸ßµ½65¶ÈÒÔºó£¬Ôò¿ªÊ¼¹ý³å´¦Àí
//ÈçºÎ½øÐÐ¹ý³å´¦ÀíÄØ?¾ÍÊÇÉèÖÃÔÚ<120¡æÒ»ÏÂ·Ö½×¶Î½øÐÐPID´¦Àí£¬
//0-60s´¦Àí65-72¡æ
//60-100s´¦Àí72-96¶ÈµÄPID¹ý³Ì
//100-200s´¦Àí96-102¶ÈµÄPID¹ý³Ì
//200-300s´¦Àí102-108¶ÈµÄPID¹ý³Ì
//300-400s´¦Àí108-114¶ÈµÄPID¹ý³Ì
//¼´Ã¿6¡æµÄÎÂ¶ÈÌÝ¶È£¬ÐèÒª100sµÄÊ±¼äÀ´½øÐÐ´¦Àí£¬½ø¶ø·ÀÖ¹¹ý³å


//ÅÐ¶ÏSv
  SvSave=pid.Sv;     //Sv = 120
 
 if(pid.Pv<(SvSave*0.55))       //ÎÂ¶È´ïµ½¼ÆÊý¿ªÊ¼ 66¶È
 {
    pid.times=0;
 }
 
  if(pid.times<=60)//3               
 {
	//120*0.6 = 72
	pid.Sv=SvSave*0.6;
 }
 
 if((pid.times>60)&&(pid.times<=100))//3·ÖÖÓ 96¶È  100s
 {
  pid.Sv=SvSave*0.8;
 }
 
 if((pid.times>100)&&(pid.times<=200))//2·ÖÖÓ 102¶È 100s
 {
  pid.Sv=SvSave*0.85;
 }
 
 if((pid.times>200)&&(pid.times<=300))//2·ÖÖÓ 108¶È 100s
 {
  pid.Sv=SvSave*0.9;
 }
 
  if((pid.times>300)&&(pid.times<=400))//2·ÖÖÓ 114¶È 100s
 {
  pid.Sv=SvSave*0.95;
 }
 
 if(pid.times>400)
 {
  pid.Sv=SvSave;
 }
//////////////////////////////////
/*
                         n
OUT=(Kp*Ek) + (Kp*(T/Ti)¡ÆEk) + (Kp*(TD/T)(EK-Ek-1)) +OUT0           
                        k=0
Kp:Ëã·¨ÔöÒæµ÷½Ú
Ek:µ±Ç°±¾´ÎÆ«²î
T: ²ÉÑùÖÜÆÚ£¬¼´¿ØÖÆÖÜÆÚ£¬Ã¿¸ôTÊ±¼ä¶Î½øÐÐÒ»´ÎPID¼ÆËã
Ti:»ý·ÖÊ±¼ä
TD:Î¢·ÖÊ±¼ä³£Êý
Ek_1:ÉÏ´ÎÆ«²îÖµ
OUT0:ÊÇÒ»¸ö³£Êý£¬Ä¿µÄÊÇ±£Ö¤OUTÊä³ö²»Îª0£¬Èç¹ûOUT=0£¬ÔòPIDÊ§¿Ø

Áî:
Ki= Kp*(T/Ti);
KD=(Kp*(TD/T)
Ôò:
 
                   n
OUT=(Kp*Ek) + (Ki¡ÆEk) + (KD(EK-Ek-1)) +OUT0           
                   k=0

*/

	//¼ÆËãÉÏÊö¹«Ê½µÄµÚÒ»Ïî:(Kp*Ek)
 pid.Ek=pid.Sv-pid.Pv;   //µÃµ½µ±Ç°µÄÆ«²îÖµ
 pid.Pout=pid.Kp*pid.Ek;      //±ÈÀýÊä³ö
    //¼ÆËãÉÏÊö¹«Ê½µÄµÚ¶þÏî: 
 pid.SEk+=pid.Ek;        //ÀúÊ·Æ«²î×ÜºÍ
 
 DelEk=pid.Ek-pid.Ek_1;  //×î½üÁ½´ÎÆ«²îÖ®²î
 
 ti=pid.T/pid.Ti;
 ki=ti*pid.Kp;
 
  pid.Iout=ki*pid.SEk*pid.Kp;  //»ý·ÖÊä³ö

 td=pid.Td/pid.T;
 
 kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;    //Î¢·ÖÊä³ö
 
 out= pid.Pout+ pid.Iout+ pid.Dout;
 
 //////////////////////////////////////////////////////////
 
 if(out>pid.pwmcycle)
 {
  pid.OUT=pid.pwmcycle;
 }
 else if(out<0)
 {
  pid.OUT=pid.OUT0; 
 }
 else 
 {
  pid.OUT=out;
 }
 //pid.OUT+=; //±¾´ÎµÄ¼ÆËã½á¹û
 pid.Ek_1=pid.Ek;  //¸üÐÂÆ«²î
 
 pid.C10ms=0;
 pid.Sv=SvSave;
}












