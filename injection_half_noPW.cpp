#include "mbed.h"
#include "sonMD.h"

UnbufferedSerial raspi         (PA_0,PA_1,9600);

sonMD            inside        (PB_10,PA_8,0.00015);
sonMD            outside       (PA_9,PC_7,0.00015);
sonMD            inside_reload (PB_9,PB_6,0.00015);
sonMD            outside_reload(PB_8,PC_9,0.00015);

int main(){
    raspi.format(8,BufferedSerial::None,1);
    int res,time; 
    static char data;
    double shotpower = 0.6;
    double reloadpower = 0.7;
    bool state = 0;
    while(true){
        res = raspi.read(&data,4);
        if(res == 1){
            switch(int(data)){
                case 9://maru,inject
                    state = 1;
                    break;
                case 10://sikaku,reload go
                    inside_reload.move_p2(0.02,reloadpower);
                    outside_reload.move_p2(0.02,reloadpower);
                    break;
                case 11://sankaku,reload back
                    inside_reload.move_p1(0.02,reloadpower);
                    outside_reload.move_p1(0.02,reloadpower);
                    break;
                case 12://batu,stop
                    state = 0;
                    break;
                default:
                    inside_reload.stop();
                    outside_reload.stop();
                    break;
            }
            if(state == 1){
                inside.move_p1(0.02, shotpower);
                outside.move_p1(0.02, shotpower);
            }
            else{
                inside.stop();
                outside.stop();
            }
        }
        else{
            inside.stop();
            outside.stop();
            inside_reload.stop();
            outside_reload.stop();
        }   
    }
}