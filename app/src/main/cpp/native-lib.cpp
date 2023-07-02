#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <fcntl.h>
#include <unistd.h>

#define DOT_DEVICE "/dev/fpga_dot"
#define BUZZER_DEVICE "/dev/fpga_buzzer"
#define PUSH_SW_DEVICE "/dev/fpga_push_switch"
#define MAX_BUTTON 9

int gFd = -1;
unsigned char defaultmap[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char my_map[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char sample[10] = {0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
int my_index = 0;
unsigned char my_position[7] = {0x40,0x20,0x10,0x08,0x04,0x02,0x01};
int cur_pos = 0;
unsigned char mine[14][2] = {{0,2},{1,3},{2,1},{2,5},{3,7},{4,1},{4,5},{5,7},{6,2},{6,6},{7,1},{8,4},{9,2},{9,3}};
unsigned char led_full[10] = {0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f};
void posclear(){
    int dev;
    my_index = 0;
    cur_pos = 0;
    dev=open(DOT_DEVICE,O_RDWR);
    write(dev,sample,sizeof (my_map));
    close(dev);
}

int fpga_dot(jint x){
    int i;
    int dev;
    size_t str_size;

    str_size = sizeof(my_map);

    dev = open(DOT_DEVICE,O_RDWR);
    if (dev<0){
        __android_log_print(ANDROID_LOG_INFO,"Dot Device Open Error","Driver = %d",dev);
    } else{
        __android_log_print(ANDROID_LOG_INFO,"Dot Device Open Success","Driver = %d",dev);

        switch (x) {
            case 2:
                //내 위치에서 인덱스 +1
                if(my_index >= 0 ){
                    my_index--;
                    my_map[my_index] = (my_position[cur_pos]);
                    write(dev, my_map, str_size);
                    memcpy(my_map, defaultmap, sizeof(my_map));
                    break;
                }
            case 8:
                //내 위치에서 인덱스 -1
                if(my_index <= 9) {
                    my_index++;
                    my_map[my_index] = my_position[cur_pos];
                    write(dev, my_map, str_size);
                    memcpy(my_map, defaultmap, sizeof(my_map));
                    break;
                }
            case 4:
                //내 위치에서 -1
                if(cur_pos >= 0) {
                    cur_pos--;
                    my_map[my_index] = (my_position[cur_pos]);
                    write(dev, my_map, str_size);
                    memcpy(my_map, defaultmap, sizeof(my_map));
                    break;
                }
            case 6:
                //내 위치에서 +1
                if(cur_pos <= 6) {
                    cur_pos++;
                    my_map[my_index] = (my_position[cur_pos]);
                    write(dev, my_map, str_size);
                    memcpy(my_map, defaultmap, sizeof(my_map));
                    break;
                }
        }
    }
    close(dev);
    return my_index;
}



int fpga_push_open(void){
    int dev;

    dev = open(PUSH_SW_DEVICE,O_RDWR);
    if(dev<0){
        __android_log_print(ANDROID_LOG_INFO,"Push Device open Error","Driver = %d",dev);
        return -1;
    }
    else {
        gFd = dev;
    }
    return 0;
}
int fpga_push_close(void){
    if (gFd<0){
        __android_log_print(ANDROID_LOG_INFO,"Push Device Not opened","Driver = %d",gFd);
        return 0;
    }
    else{
        close(gFd);
        __android_log_print(ANDROID_LOG_INFO,"Push Device Close","Driver = %d",gFd);
        return -1;
    }
}

int fpga_push_switch(void){
    int i,dev;
    size_t buff,buff_size;
    int retval;

    unsigned char push_sw_buff[MAX_BUTTON];

    if (gFd<0){
        __android_log_print(ANDROID_LOG_INFO,"Push Device open Error","Driver = %d",dev);
        return -1;
    }
    else{
        __android_log_print(ANDROID_LOG_INFO,"Push Device open Success","Driver = %d",dev);
        buff_size = sizeof(push_sw_buff);

        __android_log_print(ANDROID_LOG_INFO,"Debug1","Driver = %d",dev);
        read(gFd,&push_sw_buff,buff_size);

        __android_log_print(ANDROID_LOG_INFO,"Debug2","Driver = %d",dev);
        retval = 0;

        for (i=0;i<MAX_BUTTON;i++){
            if(push_sw_buff[i] != 0)
                retval = i+1 ;
        }
    }
    return retval;
}


int fpga_buzzer(int x){
    int dev;
    unsigned char data;
    unsigned char retval;

    data = (char)x;

    dev = open(BUZZER_DEVICE,O_RDWR);
    if (dev<0){
        __android_log_print(ANDROID_LOG_INFO,"OPEN ERROR","Driver = %d",x);
        return -1;
    }else{
        __android_log_print(ANDROID_LOG_INFO,"OPEN SUCCESS","Driver = %d",x);
        write(dev,&data,1);
        close(dev);
        return 0;}
}

int match_mine(){
    int i,j = 0;
    int val;

    for (i = 0 ;i < 14;i++){
        if (mine[i][0]==my_index && mine[i][1]==cur_pos)
        {
            return 0;
        }
    }
        return 1;
    }
void blinkdot(){
    int dev;
    dev = open(DOT_DEVICE,O_RDWR);
    write(dev,led_full,sizeof (led_full));
    usleep(100000);
    write(dev,defaultmap,sizeof (defaultmap));
    usleep(100000);
    write(dev,led_full,sizeof (led_full));
    usleep(100000);
    write(dev,defaultmap,sizeof (defaultmap));
    usleep(100000);
    write(dev,led_full,sizeof (led_full));
    usleep(100000);
    write(dev,defaultmap,sizeof (defaultmap));
    usleep(100000);
    write(dev,led_full,sizeof (led_full));
    usleep(100000);
    write(dev,defaultmap,sizeof (defaultmap));
    usleep(100000);
    write(dev,led_full,sizeof (led_full));
    usleep(100000);
    write(dev,defaultmap,sizeof (defaultmap));
    usleep(100000);
    write(dev,led_full,sizeof (led_full));
    usleep(100000);
    write(dev,defaultmap,sizeof (defaultmap));
    usleep(100000);
    close(dev);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_androidfinal_MainActivity_DeviceOpen(JNIEnv *env, jobject thiz) {
    // TODO: implement DeviceOpen()
    int retval;
    retval = fpga_push_open();
    return retval;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_androidfinal_MainActivity_DeviceClose(JNIEnv *env, jobject thiz) {
    // TODO: implement DeviceClose()
    int retval;
    retval = fpga_push_close();
    return retval;

}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_androidfinal_MainActivity_RecriveRpushSwitchValue(JNIEnv *env, jobject thiz) {
    // TODO: implement RecriveRpushSwitchValue()
    int retval;
    retval = fpga_push_switch();
    return retval;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_androidfinal_MainActivity_ReceiveDotValue(JNIEnv *env, jobject thiz, jint dir) {
    // TODO: implement ReceiveDotValue()
    int pos;
    char stat;
    pos = fpga_dot(dir);
    stat = match_mine();
    switch (stat) {
        case 1: // 이동
            break;
        case 0: // 펑 - 부저울림 및 초기화
            fpga_buzzer(1);
            blinkdot();
            posclear();
            break;


    }

    return stat;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidfinal_MainActivity_StartDot(JNIEnv *env, jobject thiz) {
    // TODO: implement StartDot()
    posclear();
    fpga_buzzer(0);
}