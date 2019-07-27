//
//  Tools.cpp
//  RockPaperScissors
//
//  Created by mini01 on 14-5-6.
//
//

#include "Tools.h"

const char* getCurtime()
{
    time_t tt;
    time(&tt);
    
    struct tm* curTime;
    
    curTime = localtime(&tt);
    
    printf("%d/%d/%d %d:%d:%d",curTime->tm_year + 1900,curTime->tm_mon + 1,curTime->tm_mday,curTime->tm_hour,curTime->tm_min,curTime->tm_sec );
    //将时间转换为现在的时间
    CCString* curTimeStr = CCString::createWithFormat("%d/%d/%d %d:%d:%d",curTime->tm_year + 1900,curTime->tm_mon + 1,curTime->tm_mday,curTime->tm_hour,curTime->tm_min,curTime->tm_sec);
    
    return curTimeStr->getCString();
}

tm* getInterval(const tm *beginTime, const tm *endTime)
{
    struct tm* interval;
    
    interval->tm_year = endTime->tm_year - beginTime->tm_year;
    
    interval->tm_mon = endTime->tm_mon - beginTime->tm_mon;
    
    interval->tm_mday = endTime->tm_mday - beginTime->tm_mday;
    
    interval->tm_hour = endTime->tm_hour - beginTime->tm_hour;
    
    interval->tm_min = endTime->tm_min - beginTime->tm_min;
    
    interval->tm_sec = endTime->tm_sec - beginTime->tm_sec;
    
    return interval;
}

bool isFileExist(const char *path)
{
    if (!path) return false;
	std::string filePath = path;
    
	FILE *fp = fopen(filePath.c_str(), "r");
	if (fp)
	{
		fclose(fp);
		return true;
	}
    
	return false;
}

void copyData(const char* pFileURL, const char* pFileName)
{
	std::string strPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileURL);
	unsigned long len = 0;
	unsigned char *data = NULL;
    
	data = CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "r", &len);
	CCAssert(data, "copyData is null");
	std::string destPath = CCFileUtils::sharedFileUtils()->getWritablePath();
	destPath += pFileName;
    
    std::string strContent((char *)data);
    CCFileUtils::sharedFileUtils()->saveToFile(strContent, destPath);
}

std::string getStringFromInt(int temp_int)
{
	char temp_str[200];
	std::string format = "%d";
	sprintf(temp_str, format.c_str(), temp_int);
    
	return temp_str;
}

float calcTwoPointAngle(CCPoint begin_point, CCPoint end_point)
{
    float delta_x = end_point.x - begin_point.x;
	float delta_y = end_point.y - begin_point.y;
    
    //CCLog("Delta_X = %f Delta_Y = %f", delta_x, delta_y);
    
    if (delta_x == 0)
    {
        if (delta_y > 0)
        {
            return 90;
        }
        else if (delta_y < 0)
        {
            return 270;
        }
        else
        {
            return 0;
        }
    }
	// 求出弧度
    float radian = atanf(delta_y / delta_x);
    
	// 求出角度
	float angle = CC_RADIANS_TO_DEGREES(radian);
    
    // 判断象限
    if (delta_x > 0)
    {
        if (delta_y > 0)
        {
            return angle;
        }
        else if (delta_y < 0)
        {
            return angle;
        }
        else if (delta_y == 0)
        {
            return 0;
        }
    }
    else
    {
        if (delta_y > 0)
        {
            return angle + 180;
        }
        else if (delta_y < 0)
        {
            return angle + 180;
        }
        else if (delta_y == 0)
        {
            return 180;
        }
    }
    
    /*
	// 根据coco2dx的坐标系进行修改
	if (y > 0.0f)
    {
		angle = 0.0f - angle;
	}
    
    if (angle > 0.0f)
    {
		angle -= 180.0f;
	}
    else if(angle < 0.0f)
    {
		angle += 180.0f;
	}
	*/
    return angle;
}

float calcTwoPointLength(CCPoint begin_point, CCPoint end_point)
{
    float f_temp_x = end_point.x - begin_point.x;
	float f_temp_y = end_point.y - begin_point.y;
    
	return sqrt(f_temp_x * f_temp_x + f_temp_y * f_temp_y);
}