#include <Wire.h>
#include <ArduinoSTL.h>

using namespace std;

int device_address = 0x50;//デバイスアドレス．ここは必ずデータシートを見て変更すること！
unsigned int address = 0;


void eeprom_write(int dev_address, unsigned int r_address, byte data){//dataの型はintでいい説もあるがbyteの方が安全そう
    int flag = 2;
    while (flag != 0){
      Wire.beginTransmission(dev_address);
      Wire.write((int)(r_address >> 8));//MSB（https://makers-with-myson.blog.ss-blog.jp/2014-04-18 のRead/Writeプロトコルの絵を見るとMSBとかLSBの表示があるので，addressを2回に分けて送るってのはこれだと思われる．）（addressの上側のビットを送っている感じ）
      Wire.write((int)(r_address & 0xff));//LSB（addressの下側の8ビット送信（多分））（>>とか&はビット演算子っぽいhttps://www.javadrive.jp/cstart/ope/index6.html）
      Wire.write(data);//ここでdataを書き込む
      flag = Wire.endTransmission();
    }
}

void eeprom_write_str(int dev_address, const char c[]){
    int i = 0;
    int n = 0;//文字を無理に数字に変える用
    while (c[i]){
        n =  c[i++] - 0;
        eeprom_write(dev_address, address, (byte)n);
        address++;
        Serial.println(address);
    }
}

void eeprom_write_num(int dev_address, int n){
    int temp = 0;
    vector<int> num;
    if (n > 0){
        while (n != 0){
            temp = n - n/10 * 10;
            num.push_back(temp);
            n = n / 10;//nの更新
        }
        for (int i = 0; i < num.size(); i++){
            eeprom_write(dev_address, address, (byte)num[num.size()-i-1]+'0');
            address++;
        }
    }else if (n == 0){
        eeprom_write(dev_address, address, (byte)'0');
        address++;
    }else{
        n = -n;//+の時のを再利用するため
        eeprom_write(dev_address, address, (byte)'-');
        address++;
        while (n != 0){
            temp = n - n/10 * 10;
            num.push_back(temp);
            n = n / 10;//nの更新
        }
        for (int i = 0; i < num.size(); i++){
            eeprom_write(dev_address, address, (byte)num[num.size()-i-1]+'0');
            address++;
        }
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("starting");
    delay(100);
    Wire.begin();

    eeprom_write_str(device_address, "writing_hoo");//文字を1つづつ分けて無理やり数字に変換してeepromに記録
    eeprom_write_str(device_address, "\n");//改行文字(多分無理)→いけちゃった
    eeprom_write_num(device_address, -32);//123を無理やり文字の123にして1文字づつ数値に変更してeepromに記録
    eeprom_write_str(device_address, "END");//ENDで入力を終わるようにする（必ず）（EEPROMから読む時に死ぬのを防ぐ）
}

void loop(){
}
