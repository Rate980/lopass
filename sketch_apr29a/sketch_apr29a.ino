
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <HampelFilter.h> 
#include <M5StickC.h>
#include <HampelFilter.h> // include library

HampelFilter dataBufferX = HampelFilter(0.00, 20, 3.5);
HampelFilter dataBufferZ = HampelFilter(0.00, 20, 3.5);

#define MODE_A 0 // 数値表示
#define MODE_B 1 // 姿勢表示
uint8_t disp_mode = MODE_A;

#define BTN_A_PIN  37
#define BTN_ON  LOW
#define BTN_OFF HIGH
uint8_t prev_btn_a = BTN_OFF;
uint8_t btn_a      = BTN_OFF;

#define POSE_P_X 0
#define POSE_M_X 1
#define POSE_P_Y 2
#define POSE_M_Y 3
#define POSE_P_Z 4
#define POSE_M_Z 5
uint8_t pose = POSE_P_X;
uint8_t prev_pose = POSE_P_X;

// 加速度。センサで取得できる値の単位は[g]なので、通常の[m/s^2]単位で考えるなら9.8倍する
float accX_g = 0;
float accY_g = 0;
float accZ_g = 0;
float accX_mpss = 0;
float accY_mpss = 0;
float accZ_mpss = 0;

boolean near_p_g(float value){
  if(8.0 < value && value < 12.0){
    return true;
  }else{
    return false;
  }
}

boolean near_m_g(float value){
  if(-12.0 < value && value < -8.0){
    return true;
  }else{
    return false;
  }
}

boolean near_zero(float value){
  if(-2.0 < value && value < 2.0){
    return true;
  }else{
    return false;
  }
}


//--------------------------------------------------------------------------------

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

int bleRX, bleTX;
uint8_t notifySend[2] = {0, 0};

#define SERVICE_UUID "00002220-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID_TX "00002221-0000-1000-8000-00805F9B34FB"

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

void setup()
{ 
  Serial.begin(115200);             // シリアル通信の準備をする
  while (!Serial);                  // 準備が終わるのを待つ
  Serial.println("プログラム開始");    // シリアル通信でメッセージをPCに送信

  // Initialize the M5StickC object
  M5.begin();
  pinMode(BTN_A_PIN,  INPUT_PULLUP);
  // 6軸センサ初期化
  M5.MPU6886.Init();

  //------------------------------------------------------------------

  // // Create the BLE Device
  // BLEDevice::init("BleDevice");

  // // Create the BLE Server
  // BLEServer *pServer = BLEDevice::createServer();
  // pServer->setCallbacks(new MyServerCallbacks());

  // // Create the BLE Service
  // BLEService *pService = pServer->createService(SERVICE_UUID);

  // // Create a BLE Characteristic
  // pCharacteristic = pService->createCharacteristic(
  //     CHARACTERISTIC_UUID_TX,
  //     BLECharacteristic::PROPERTY_NOTIFY);

  // pCharacteristic->addDescriptor(new BLE2902());

  // // Start the service
  // pService->start();

  // // Start advertising
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();
  // pAdvertising->addServiceUUID(SERVICE_UUID);
  // pServer->getAdvertising()->start();
  // Serial.println("Waiting a client connection to notify");
}

void loop()
{
  // notify changed value
  // if (deviceConnected)
  // {

    // 加速度取得
    M5.MPU6886.getAccelData(&accX_g,&accY_g,&accZ_g);
    accX_mpss = accX_g * 9.8;
    accY_mpss = accY_g * 9.8;
    accZ_mpss = accZ_g * 9.8;

    // 現在の姿勢を検出する
    if(near_p_g(accX_mpss) && near_zero(accY_mpss) && near_zero(accZ_mpss)){
      pose = POSE_P_X;
    }else if(near_zero(accX_mpss) && near_zero(accY_mpss) && near_p_g(accZ_mpss)){
      pose = POSE_P_Z;
    }else if(near_m_g(accX_mpss) && near_zero(accY_mpss) && near_zero(accZ_mpss)){
      pose = POSE_M_X;
    }else if(near_zero(accX_mpss) && near_zero(accY_mpss) && near_m_g(accZ_mpss)){
      pose = POSE_M_Z;
    }

    switch(pose){
      case POSE_P_X:
        accZ_mpss =  -1 * (accZ_mpss - 0.43);
        accX_mpss = -1 * (accX_mpss - 9.64);
        break;
      case POSE_M_X:
        accZ_mpss = accZ_mpss - 1.55;
        accX_mpss = accX_mpss + 9.85;
        break;
      case POSE_P_Z:
        accX_mpss = accX_mpss + 0.09;
        accZ_mpss = accZ_mpss - 10.55;
        break;
      case POSE_M_Z:
        accX_mpss = accX_mpss + 0.05;
        accZ_mpss = accZ_mpss + 8.95;
        break;
      default:
        ;
    }


    dataBufferX.write(accX_mpss);
    dataBufferZ.write(accZ_mpss);
    if(dataBufferX.checkIfOutlier(accX_mpss)==false and dataBufferZ.checkIfOutlier(accZ_mpss)==false){
      int resolt;

      Serial.print(accX_mpss);
      Serial.print(",");
      Serial.println(accZ_mpss);
      if(accX_mpss > 8 or -8 > accX_mpss){
        
        if(accX_mpss > 0){
          resolt = 0b00010101;
          Serial.println("left");
          delay(5000);
        } else {
          resolt = 0b00000000;
          Serial.println("right");
          delay(5000);
        }

        while(true){
          M5.MPU6886.getAccelData(&accX_g,&accY_g,&accZ_g);
          accX_mpss = accX_g * 9.8;
          accZ_mpss = accZ_g * 9.8;
          dataBufferX.write(accX_mpss);
          dataBufferZ.write(accZ_mpss);
          if((accX_mpss < 5 and -6 < accX_mpss) or (accZ_mpss < 5 and -6 < accZ_mpss)){
            break;
          }
          delay(1);
        }
        delay(1000);
      } else if(accZ_mpss > 7 or -8 > accZ_mpss){
      
        if(accZ_mpss > 0){
          resolt = 0b00010101;
          Serial.println("left");
          delay(5000);
        } else {
          resolt = 0b00000000;
          Serial.println("right");
          delay(5000);
        }

        while(true){
          M5.MPU6886.getAccelData(&accX_g,&accY_g,&accZ_g);
          accX_mpss = accX_g * 9.8;
          accZ_mpss = accZ_g * 9.8;
          dataBufferX.write(accX_mpss);
          dataBufferZ.write(accZ_mpss);
          if((accX_mpss < 5 and -6 < accX_mpss) or (accZ_mpss < 5 and -6 < accZ_mpss)){
            break;
          }
          delay(1);
        }
        delay(1000);
      }
    }

    
    
    
    
    delay(40);
    // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  // }
  // // disconnecting
  // if (!deviceConnected && oldDeviceConnected)
  // {
  //   delay(500);                  // give the bluetooth stack the chance to get things ready
  //   pServer->startAdvertising(); // restart advertising
  //   oldDeviceConnected = deviceConnected;
  // }
  // // connecting
  // if (deviceConnected && !oldDeviceConnected)
  // {
  //   // do stuff here on connecting
  //   oldDeviceConnected = deviceConnected;
  // }
}