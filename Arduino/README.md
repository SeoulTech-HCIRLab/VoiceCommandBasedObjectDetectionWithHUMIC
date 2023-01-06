# Prepare for Arduino

#### 1. install Kangaroo Library
- install at: https://www.dimensionengineering.com/info/arduino

#### 2. move "/yj" directory in "/libraries" to under " ~/Arduino/libraries/ros_lib/ " directory
- reference at: http://wiki.ros.org/rosserial_arduino/Tutorials/Adding%20Custom%20Messages
  
#### 3. upload Arduino
- upload each sketch to each Arduino  
  - reference at: HCIR NAS > HCIR > 5_Project > !RobotSystem > 로봇제작용역 > 납품결과 > 2019-01-30 최종 전달 파일 > 5. 설명서 > 2019-01-29 Mobile Robot 설명서.pptx  
  
  - Rot/Rot.ino -> Arduino board of humic for Mobile  
    - Mobile은 RS232로 통신하기 때문에, 업로드용과 통신용 케이블이 따로 존재함. 업로드용 케이블에 연결하고서 업로드 진행  
  
  - USensor/USensor.ino -> Arduino board of humic for column&ir sensor  

