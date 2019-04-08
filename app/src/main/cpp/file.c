
#include "com_jni_ndk_FileNative.h"

//获取文件大小
long get_file_size(const char *path) {
    FILE *fp = fopen(path, "rb");
    //定位到文件末尾
    fseek(fp, 0, SEEK_END);
    return ftell(fp);
}
//取出数据的最高位的值
long get_num_hight(long num){
    int tmp = 1;
    long total = num;
    while((num /= 10) != 0){
        tmp *= 10;
        total = num;
    }
    return tmp*(total+1);
}

JNIEXPORT void JNICALL
Java_com_jni_ndk_FileNative_diff(JNIEnv *env, jclass type, jstring diff_path_, jstring pattern_path_,
                                 jint count) {
    const char *diff_path = (*env)->GetStringUTFChars(env, diff_path_, NULL);
    const char *pattern_path = (*env)->GetStringUTFChars(env, pattern_path_, NULL);

    //得到分割之后的子文件路径列表
    char **patches = malloc(sizeof(char *) * count);
    int i = 0;
    for (; i < count; i++) {
        patches[i] = malloc(sizeof(char) * 100);
        //元素赋值
        //需要分割的文件; ..... /def.mp4
        //子文件 ;        ...../def_%d.mp4
        sprintf(patches[i], pattern_path, (i + 1));
        LOG_I("patch path:%s", patches[i]);
    }

    const long file_size = get_file_size(diff_path);
    if(file_size < count){
        LOG_E("警告:文件大小 小于%d",count);
        return;
    }
    FILE *fpr = fopen(diff_path, "rb");
    //判断是否整除
    if (file_size % count == 0) {
        //单个文件大小
        long part_size = file_size / count;
        i = 0;
        //逐一写入不同的分割子文件中
        for (; i < count; i++) {
            FILE *fpw = fopen(patches[i],"wb");
            int j = 0;
            for (;j<part_size;i++){
                //边读边写,一个一个字符写
                fputc(fgetc(fpr),fpw);
            }
            fclose(fpw);
        }
    } else {
        //不整除
        long part_size = file_size/count;
        //将得到大小 取整
        //将大小最高位+1
        part_size = get_num_hight(part_size);
        LOG_I("part_size : %ld",part_size);
        //求出最后一部分的文件大小
        long last_part_size = file_size-part_size*(count-1);
        LOG_I("last_part_size : %ld",last_part_size);
        i = 0;
        for (; i < count; i++) {
            FILE *fpw = fopen(patches[i],"wb");
            int j = 0;
            if(i != (count-1)){
                for (;j<part_size;j++){
                    //边读边写
                    fputc(fgetc(fpr),fpw);
                }
            } else{
                for (;j<last_part_size;j++){
                    //边读边写
                    fputc(fgetc(fpr),fpw);
                }
            }
            fclose(fpw);
        }
    }
    //关闭被分割的文件
    fclose(fpr);
    //释放
    i = 0;
    for(; i < count; i++){
        free(patches[i]);
    }
    free(patches);
    (*env)->ReleaseStringUTFChars(env, diff_path_, diff_path);
    (*env)->ReleaseStringUTFChars(env, pattern_path_, pattern_path);
}

JNIEXPORT void JNICALL
Java_com_jni_ndk_FileNative_merge(JNIEnv *env, jclass type, jstring pattern_path_, jint count,
                                  jstring merge_path_) {
    const char *pattern_path = (*env)->GetStringUTFChars(env, pattern_path_, 0);
    const char *merge_path = (*env)->GetStringUTFChars(env, merge_path_, 0);
//得到分割之后的子文件的路径列表
    char **patches = malloc(sizeof(char*) * count);
    int i = 0;
    for (; i < count; i++) {
        patches[i] = malloc(sizeof(char) * 100);
        //元素赋值
        //需要分割的文件：C://jason/liuyan.png
        //子文件：C://jason/liuyan_%d.png
        sprintf(patches[i], pattern_path, (i+1));
        LOG_I("patch path:%s",patches[i]);
    }

    FILE *fpw = fopen(merge_path,"wb");
    //把所有的分割文件读取一遍，写入一个总的文件中
    i = 0;
    for(; i < count; i++){
        //每个子文件的大小
        int filesize = get_file_size(patches[i]);
        FILE *fpr = fopen(patches[i], "rb");
        int j = 0;
        for (; j < filesize; j++) {
            fputc(fgetc(fpr),fpw);
        }
        fclose(fpr);
    }
    fclose(fpw);

    //释放
    i = 0;
    for(; i < count; i++){
        free(patches[i]);
    }
    free(patches);

    (*env)->ReleaseStringUTFChars(env, pattern_path_, pattern_path);
    (*env)->ReleaseStringUTFChars(env, merge_path_, merge_path);
}