#include <Stepper.h>
#include <SoftwareSerial.h>

SoftwareSerial bt(2,3);
// 2048:한바퀴(360도), 1024:반바퀴(180도)...
const int stepsPerRevolution = 64; //32번회전==1회전 
// 모터 드라이브에 연결된 핀 IN4, IN2, IN3, IN1
Stepper myStepper(stepsPerRevolution,11,9,10,8);           
void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(10);//1~100 
  bt.begin(9600);
}
int cnt=0;
int rot_cnt=0;
String msg="";
int speed_=10;
int rotate_count=1;
bool is_start=false;
unsigned long t1=0;
void loop() {
  if(bt.available()){ //블루투스로 수신 예) set,20,3 => 20step/s 속도로 3회전(360도 3회)
      msg=bt.readStringUntil('\n');
        if(msg.indexOf("set")>-1){//votlage 0
          Serial.println(msg);
          is_start=false;
          cnt=0;
          rot_cnt=0;
          is_start=true; //회전시작 
          int one = msg.indexOf(",");// 첫 번째 콤마 위치
          int two = msg.indexOf(",",one+1); // 두 번째 콤마 위치
          int length_ = msg.length(); // 문자열 길이
        
          String str1 = msg.substring(0, one); // 첫 번째 토큰 
          String str2 = msg.substring(one+1, two); // 두 번째 토큰 
          speed_=str2.toInt();//회전속도
          myStepper.setSpeed(speed_);//1~100 
          String str3 = msg.substring(two+1,length_); // 세 번째 토큰
          rotate_count=str3.toInt();//회전횟수
       }
       
  }
  if (is_start){
    if(millis()-t1>15){     
      cnt++;
      myStepper.step(stepsPerRevolution);//32번반복해야함
      
      if(cnt==32){
        cnt=0;
          rot_cnt++;//회전바퀴 +1
          if(rot_cnt==rotate_count){
            is_start=false; //원하는 회전수채우면 정지 
          }
      }
      t1=millis();  
    }

  }
 
}
