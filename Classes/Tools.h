//
//  Tools.h
//  RockPaperScissors
//
//  Created by mini01 on 14-5-6.
//
//

#ifndef __RockPaperScissors__Tools__
#define __RockPaperScissors__Tools__

#include "cocos2d.h"
#include <time.h>
USING_NS_CC;

//判断路径下文件是否存在
bool isFileExist(const char* path);

//用于创建无法修改的包内文件副本（创建用户存储文件）
void copyData(const char* pFileURL,const char* pFileName);

//获取当前时间
const char* getCurtime();

//获取两段时间的间隔
tm* getInterval(const tm* beginTime,const tm* endTime);

//通过整数获取字符串
std::string getStringFromInt(int temp_int);

//计算两个点组成的直线同X轴形成的夹角
float calcTwoPointAngle(CCPoint begin_point, CCPoint end_point);

float calcTwoPointLength(CCPoint begin_point, CCPoint end_point);

#endif /* defined(__RockPaperScissors__Tools__) */
