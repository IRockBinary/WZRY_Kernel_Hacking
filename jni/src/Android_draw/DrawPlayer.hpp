#include "include.h"
#include "IsCharacter.h"
#define BYTE4 0x00000004
#define BYTE8 0x00000008
#define BYTE16 0x00000010
#define BYTE10 0x000000A
#define BYTE64 0x00000040
#define BYTE1024 0x00000400
uintptr_t unity;
uintptr_t unitybss;
uintptr_t GameCore;
uintptr_t GameCorebss;
uintptr_t il2cpp;
uintptr_t il2cppbss;
static uint64_t lil2cpp_base = 0;
static uint64_t libGame_base = 0;
static uint64_t libunity_base = 0;
static uint64_t libtersafe_base = 0;
uint64_t MatrixAddress = 0;//初始化矩阵地址
float isGames;//储存矩阵的值
int foeComp;//敌方阵营
int rientation = 0;//对局方向
int AroundNumbers = 0;//人头数量
pid_t get_name_pid(char* name) {
    FILE* fp;
    pid_t pid;
    char cmd[0x100] = "pidof ";

    strcat(cmd, name);
    fp = popen(cmd,"r");
    fscanf(fp,"%d", &pid);
    pclose(fp);
    return pid;
}

struct Coord {
    float X;//X轴
    float Y;//Y轴
    float W;
    float H;
};
struct DynamicData {
    struct Coord coord;//获取xy坐标
};

struct HeroTemp {
    struct Coord coord;//获取xy坐标
    int Hp;//当前血量
    int MaxHp;//最大血量
    int Id;//英雄id
    int Space;//大招cd
    int Skill;//召唤师技能cd
    int camp;//阵营id
    int HC;//回城
    int TB;//图标
    int confound;//坐标混淆
};
/**
 * 获取野怪xy和刷新时间
 */
struct Pve {
    uint64_t X;//X坐标地址
    uint64_t Y;//Y坐标地址
    uint64_t cd;//刷新时间
    uint64_t cc;
    uint64_t maxcd;
};
/**
 * 获取野怪xy和刷新时间
 */
 
 struct PveTemp {
    struct Coord coord;//获取xy坐标
    int cd;//刷新时间
    int maxcd;
};

/**
 * 获取兵线xy
 */
struct Pvc {
    uint64_t X;//X坐标地址
    uint64_t Y;//Y坐标地址
};
/**
 * 单个兵线
 */
struct CeTemp {
    struct Coord coord;//获取xy坐标
    int cd;
};

typedef struct {
    float Matrix[BYTE16];//相机矩阵
    struct HeroTemp heroTemp[BYTE16];//储存单个英雄
    struct DynamicData DynamicData[BYTE16];//储存单个英雄大地图
    struct Pve pve[BYTE1024];//储存野怪
    struct PveTemp pveTemp[BYTE1024];//储存野怪
    struct CeTemp CeTemp[BYTE1024];//储存兵线
    struct Pvc pvc[BYTE1024];//储存兵线
    int xbsl;//小兵数量
} DataTable;


DataTable dataTable;//游戏数据表


    const static ImColor Paint_purple = ImColor(255, 0, 255);  //紫色
    const static ImColor Paint_red = ImColor(255, 0, 0);       //红色
    const static ImColor Paint_white = ImColor(255, 255, 255); //白色
    const static ImColor Paint_lightblue = ImColor(0, 255, 255);   //浅蓝色
    const static ImColor Paint_yellow = ImColor(255, 255, 0);  //黄色
    const static ImColor Paint_green = ImColor(0, 255, 0);  //绿色

double Wwra = 0;    //转小地图算法




 struct Coord map_coord = {0};/*实际地图坐标*/
    struct Coord map_buff = {0};/*实际地图坐标*/



    
    


void DrawInit()
{
  //  char libGame[0x100] = "libGameCore.so";    //strlen(name) < 0xff or overflow
pid = get_name_pid((char*)"com.tencent.tmgp.sgame");
   // pid = getPID("com.tencent.tmgp.sgame");
   driver->initialize(pid);
    if(pid == 0 || pid == -1)
    {
        puts("先打开游戏");
        exit(1);
    }
    
   // chishihua();

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
  libGame_base = driver->getModuleBase("libGameCore.so");

            lil2cpp_base = driver->getModuleBase("libil2cpp.so");

            libunity_base = driver->getModuleBase("libunity.so");
            
            libtersafe_base = driver->getModuleBase("libtersafe.so");
            
            
            
    if(fenblxx==1){
            int x1,y1;
                            
                            x1=screen_x<screen_y?screen_x:screen_y;
                            y1=screen_x<screen_y?screen_y:screen_x;
                            if(yxpx<yxpy)
                            {yxpx=x1;yxpy=y1;}else{yxpy=x1;yxpx=y1;}
        
        
    }else{
    long tmp = libunity_base + 0x14D3DEC;
    
        yxpx = 2400;
    yxpy = 1080;
   // ScreenView = {(float)yxpx/2, (float)yxpy/2};
    
    }
    
       
    
    
    
    
    
    
    
    
}
 
 Coord CalMatrixMem(struct Coord coor, const float Matrix[]) {
    struct Coord points = {0};
    float XM = coor.X / 1000.0f;
    float ZM = coor.Y / 1000.0f;
    if(Matrix[11]&&Matrix[15]&&Matrix[0]&&Matrix[12]&&Matrix[9]&&Matrix[13]&&Matrix[1]&&Matrix[5] &&Matrix[9]&&Matrix[13]){
    float radio = (float)fabs(ZM * Matrix[11] + Matrix[15]);
    points.X = 0.0f, points.Y = 0.0f, points.W = 0.0f, points.H = 0.0f;
    if (radio > 0) {
        points.X = yxpx/2 + (XM * Matrix[0] + Matrix[12]) / radio * yxpx/2;
        points.Y = yxpy/2 - (ZM * Matrix[9] + Matrix[13]) / radio * yxpy/2;
        points.W = yxpy/2 - (XM * Matrix[1] + 4.0f * Matrix[5] + ZM * Matrix[9] + Matrix[13]) / radio * yxpy/2;
        points.H = ((points.Y - points.W) / 2.0f);
    }
    return points;
    }
}
    
    
/**
 * 大地图转小地图坐标
 * @param foe
 * @return
 */
    





void DrawPlayer()
{
const static float TXdx = yxpy * 0.017534f; //(头像)圈大小(22.5)半径
      const static float soldiers_dx = yxpy * (double)0.001851851851851851; //(兵)大小(2.0f)半径
    const static float CDjs_X = yxpx * 0.337250f; //绘制cd起
       const static float CDjs_Y = yxpy * 0.006481f; //绘制cd起点Y(7)
    const static float intervalcdX = yxpx * 0.025200f; //60(cd左右间隔)
    const static float intervalcdY = yxpy * 0.032407f; //35(cd上下间隔)

    
    
    const static float skills_dx = yxpy * (double)0.006481481481481481;
    const static float skills_txt_dx = skills_dx * 5.0f;    //技能CD文字大小
    Wwra=yxpy / 10.2f * (1.574074075+(jiange*0.01));   // 间隔
    
    long temp = driver->read<uint64_t>(lil2cpp_base+0x9BB8908);//8F547A8
    
    MatrixAddress = driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(temp + 0xA0) + 0x0) + 0x10) + 0xC0;
    isGames = driver->read<float>(MatrixAddress);
    //判断敌方阵营id
    /*  ?  :   三目运算符  如果大于0就返回2 否则返回1 */
    foeComp = isGames > 0 ? 2 : 1;
    //LOGE("foeComp:%d",foeComp);
    //判断阵营，获取算法，判断方向
    rientation = foeComp == 1 ? -1 : 1;
    
        for (int i = 0; i < 16; i++)
        {
        
            dataTable.Matrix[i] = driver->read<float>(MatrixAddress + i * 4);
        }
    
    
    
    //ImGui::GetBackgroundDrawList()->AddImage(createTexture1("/storage/emulated/0/105.png").textureId, ImVec2(100-25, 100-25), ImVec2(100+25, 100+25));

    long temp1 = driver->read<uint64_t>(libGame_base +  0x3876F10);
 //   printf("temp1 %f \n",temp1);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
    long bingxiang1 = temp1 + 0x138;
  
    AroundNumbers =12;

int opop=0;
    
        for (int i = 0; i < AroundNumbers; i++)
        {
            int ndh = i * 0x18;
    
    
            long bingxiang6 =driver->read<uint64_t>( driver->read<uint64_t>(driver->read<uint64_t>(bingxiang1) +0x60 )+ndh);
        int pand = 1;
        
        if (pand > 0)
            {

    int zhengxing = driver->read<int>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               (bingxiang6 + 0x34);

if(zhengxing==foeComp){



        // //////头像id↓代码

                    dataTable.heroTemp[i].Id = driver->read<uint64_t>(bingxiang6 + 0x28);
                  
dataTable.heroTemp[i].Hp =driver->read<int>(driver->read<uint64_t>(bingxiang6 + 0x160) + 0xa0);  // 当前血量

    dataTable.heroTemp[i].MaxHp = driver->read<int>(driver->read<uint64_t>(bingxiang6 + 0x160) + 0xa8);


dataTable.heroTemp[i].coord.X = (float) driver->read<int>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(bingxiang6 +0x1f0) +0x10)+0x0)+0x10)+0x0);
dataTable.heroTemp[i].coord.Y = (float) driver->read<int>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(bingxiang6 +0x1f0) +0x10)+0x0)+0x10)+0x8);
dataTable.DynamicData[i].coord.X = (dataTable.heroTemp[i].coord.X * rientation * Wwra / 50000.0f + Wwra);
dataTable.DynamicData[i].coord.Y = (dataTable.heroTemp[i].coord.Y * rientation * Wwra / 50000.0f * -1 + Wwra);
 
float pos_x = dataTable.DynamicData[i].coord.X + SmallMapX+94;
float pos_y = dataTable.DynamicData[i].coord.Y + SmallMapY+7;

map_coord = CalMatrixMem(dataTable.heroTemp[i].coord, dataTable.Matrix);
  map_coord.X = map_coord.X + SmallHPX;
map_coord.Y = map_coord.Y + SmallHPY;


        float hp1 = dataTable.heroTemp[i].Hp * 100 / dataTable.heroTemp[i].MaxHp;

    float aa = hp1*3.6;
                  //   ImColor 血量颜色;
                //     血量颜色 = ImColor(10,240,10,210);
                
                
                ImTextureID handId;
                if(dataTable.heroTemp[i].Id>100&&dataTable.heroTemp[i].Id<600){
            
                handId =reinterpret_cast<ImTextureID>(贴图1.头像[dataTable.heroTemp[i].Id].textureId);
    }else{ 
        handId=reinterpret_cast<ImTextureID>(贴图1.头像[0].textureId);
    }
                
                
                if(aa>0){
                    
    if(血量){
                    
                    //小头像血量
   ImGui::GetForegroundDrawList()->AddCircleArc({pos_x, pos_y},21+xiaodituxue, ImVec2(0, 360), Paint_white,  0, 5.5f);//白底背景
   ImGui::GetForegroundDrawList()->AddCircleArc({pos_x, pos_y},21+xiaodituxue, ImVec2(0, aa), 颜色配置.血量颜色,  0, 5.5f);//红色血圈
    }       
   
    
    if(地图){
   //小头像
                    ImGui::GetBackgroundDrawList()->AddImage( handId!= NULL ?  handId: 0 , {(int)pos_x - TXdx-touxiangdaxiao, (int)pos_y - TXdx-touxiangdaxiao}, {(int)pos_x + TXdx+touxiangdaxiao, (int)pos_y + TXdx+touxiangdaxiao});
}
                    
   
   
   if(方框){      
   //方框
   ImGui::GetForegroundDrawList()->AddRect(ImVec2((int)map_coord.X - (map_coord.H*0.5f), (int)map_coord.Y - (map_coord.H*1.32f)), ImVec2((int)map_coord.X + (map_coord.H*0.5), (int)map_coord.Y + (map_coord.H*0.2f)), 颜色配置.方框颜色, 0.0f, 0, 2.0f); //方框[ ]
   
   //方框里头像
       ImGui::GetForegroundDrawList()->AddImage(handId!= NULL ?  handId: 0, {(int)map_coord.X - (map_coord.H*0.2f), (int)map_coord.Y - (map_coord.H*0.2f)+ (-map_coord.H*1.0f)}, {(int)map_coord.X + (map_coord.H*0.2f), (int)map_coord.Y + (map_coord.H*0.2f)+ (-map_coord.H*1.0f)});
         
       
       //方框里血量
                    ImGui::GetForegroundDrawList()->AddCircleArc(ImVec2((int)map_coord.X,(int) map_coord.Y+ (-map_coord.H*1.0f)),(map_coord.H*0.22f)>0 ? (map_coord.H*0.22f) : 1, ImVec2(0, 360), Paint_white,  0, 5.5f);//白底背景
                    ImGui::GetForegroundDrawList()->AddCircleArc(ImVec2((int)map_coord.X, (int)map_coord.Y+ (-map_coord.H*1.0f)),(map_coord.H*0.22f)>0 ? (map_coord.H*0.22f) : 1, ImVec2(0, aa), 颜色配置.血量颜色,  0, 5.5f);//红色血圈
                    
    }               
    if(射线){         
                    //射线
                     
                        ImGui::GetForegroundDrawList()->AddLine(ImVec2(yxpx/2, yxpy/2), ImVec2(map_coord.X, map_coord.Y + (-map_coord.H*1.0f)), 颜色配置.射线颜色, 3.0f);
    }               
                    
                    
                    
    
    dataTable.heroTemp[i].Space = driver->read<int>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(bingxiang6+0x130)+0x108)+0xf8)+0xd0)/8192000;
        dataTable.heroTemp[i].Skill = driver->read<int>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(bingxiang6+0x130)+0x150)+0xf8)+0xd0)/8192000;          
        dataTable.heroTemp[i].HC =  driver->read<int>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(bingxiang6 + 0xf8)+0x168)+0x110)+0x20);
                    dataTable.heroTemp[i].TB = driver->read<int>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(bingxiang6+0x130) + 0x150) + 0x58) + 0x10);   
                                           
                                           
                                if (dataTable.heroTemp[i].TB != 80102 && dataTable.heroTemp[i].TB != 80103 &&
            dataTable.heroTemp[i].TB != 80104 && dataTable.heroTemp[i].TB != 80105 &&
            dataTable.heroTemp[i].TB != 80107 && dataTable.heroTemp[i].TB != 80108 &&
            dataTable.heroTemp[i].TB != 80109 && dataTable.heroTemp[i].TB != 80110 &&
            dataTable.heroTemp[i].TB != 80115 && dataTable.heroTemp[i].TB != 80121 &&
            dataTable.heroTemp[i].TB != 80116 && dataTable.heroTemp[i].TB != 801162 
            ) {
            dataTable.heroTemp[i].TB = 0;
        }                  
                                           
                                           
                                           
                //方框里技能
   if (方框技能) {
                    if (dataTable.heroTemp[i].Space > 0) {
                        int intvalue = dataTable.heroTemp[i].Space;
                        string test = std::to_string(intvalue);
                        ImGui::GetForegroundDrawList()->AddText(NULL, (float)((double)(0.316777647) * map_coord.H), ImVec2((float)map_coord.X + (map_coord.H*0.6)+ (-map_coord.H*1.06f), (float)map_coord.Y - (map_coord.H*0.07f)+ (map_coord.H*0.3f)), Paint_red,
                                      test.c_str());
                    } else {
                        ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2((float)map_coord.X + (map_coord.H*0.6)+ (-map_coord.H*1.06f), (float)map_coord.Y - (map_coord.H*0.008f)+ (map_coord.H*0.3f)), ImVec2((float)map_coord.X + (map_coord.H*0.8)+ (-map_coord.H*1.06f), (float)map_coord.Y + (map_coord.H*0.2f)+ (map_coord.H*0.3f)), Paint_red, 0); //实体方点
                    }
                    if (dataTable.heroTemp[i].Skill > 0) {
                        int intvalue = dataTable.heroTemp[i].Skill;
                        string test = std::to_string(intvalue);
                        ImGui::GetForegroundDrawList()->AddText(NULL, (float)((double)(0.316777647) * map_coord.H), ImVec2((float)map_coord.X + (map_coord.H*0.6)+ (-map_coord.H*0.3f), (float)map_coord.Y - (map_coord.H*0.07f)+ (map_coord.H*0.3f)), Paint_red,
                                      test.c_str());
                    } else {
                       // ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2((float)map_coord.X + (map_coord.H*0.6)+ (-map_coord.H*0.3f), (float)map_coord.Y - (map_coord.H*0.008f)+ (map_coord.H*0.3f)), ImVec2((float)map_coord.X + (map_coord.H*0.8)+ (-map_coord.H*0.3f), (float)map_coord.Y + (map_coord.H*0.2f)+ (map_coord.H*0.3f)), Paint_red, 0); //实体方点
                   
                        ImTextureID tubiaoid;
                    if(dataTable.heroTemp[i].TB!=0){
                    if(dataTable.heroTemp[i].TB==801162){
                        tubiaoid=reinterpret_cast<ImTextureID>(技能贴图.头像[dataTable.heroTemp[i].TB-800000].textureId);
    
                    }else{
                    tubiaoid=reinterpret_cast<ImTextureID>(技能贴图.头像[dataTable.heroTemp[i].TB-80000].textureId);
    }
    
    }else{tubiaoid =reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId);
    
                }
                    ImTextureID tubiaoid2 =reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId);
    
                
    ImGui::GetForegroundDrawList()->AddImage(tubiaoid!= NULL ?  tubiaoid: tubiaoid2, ImVec2((float)map_coord.X +(map_coord.H*0.16)+(map_coord.H*0.3), (float)map_coord.Y -(map_coord.H*0.16)+(map_coord.H*0.38)), ImVec2((float)map_coord.X -(map_coord.H*0.16)+(map_coord.H*0.3), (float)map_coord.Y +(map_coord.H*0.16)+(map_coord.H*0.38)));//顶上头像
           
                        
                        }
                }
   
                
            
                
                
                    }//血量大于0
                    
                    
                    
                    
                    
                    
                    
                       //TODO：顶上技能
            if (顶上技能) {
                float Theoffset_X = CDjs_X + SmallMapX+jinenglanz;
                float CDdrawXY[3][2] = {
                        {(float)(Theoffset_X +  intervalcdX * opop), (float)(CDjs_Y + intervalcdY * 0)},
                        {(float)(Theoffset_X +  intervalcdX * opop), (float)(CDjs_Y + (TXdx/2) + intervalcdY * 1)},
                        {(float)(Theoffset_X +  intervalcdX * opop), (float)(CDjs_Y + TXdx + intervalcdY * 2)}
                };
                const float _txt_X = Theoffset_X - (TXdx * 3.6f);
                if (opop == 0) {
                    ImGui::GetForegroundDrawList()->AddText(NULL, 28.0f, ImVec2((float)_txt_X-10.0f, (float)(CDdrawXY[0][1] + (TXdx/2))), Paint_red, "大招:");
                  //  draw->AddText(NULL, 28.0f, ImVec2((float)_txt_X, (float)(CDdrawXY[1][1] + (TXdx/2))), Paint_red, "头像:");
                    ImGui::GetForegroundDrawList()->AddText(NULL, 28.0f, ImVec2((float)_txt_X-10.0f, (float)(CDdrawXY[2][1] + (TXdx/2))), Paint_red, "召唤:");

                }
                CDdrawXY[1][0] = CDdrawXY[1][0] + (TXdx/14.0f); // x 二次改变偏移

                CDdrawXY[0][1] = CDdrawXY[0][1] + (TXdx) + (TXdx/5.0f);
                CDdrawXY[1][1] = CDdrawXY[1][1] + (TXdx) + (TXdx/5.0f);//绘制头像
                CDdrawXY[2][1] = CDdrawXY[2][1] + (TXdx) + (TXdx/5.0f);
                /*if (0==1) //显示回城
                    local_DrawHome(draw, CDdrawXY[1][0], CDdrawXY[1][1], renderScale, color);
float aa = hp*3.6;*/
                ImGui::GetForegroundDrawList()->AddImage(handId!= NULL ?  handId: 0, {CDdrawXY[1][0] - TXdx, CDdrawXY[1][1] - TXdx}, {CDdrawXY[1][0] + TXdx, CDdrawXY[1][1] + TXdx});//顶上头像
                ImGui::GetForegroundDrawList()->AddCircleArc(ImVec2(CDdrawXY[1][0], CDdrawXY[1][1]),21, ImVec2(0, aa), 颜色配置.血量颜色,  0, 5);//红色圈边
                if (dataTable.heroTemp[i].Space > 0) { //绘制大招
                    int intvalue = dataTable.heroTemp[i].Space;
                    string test = std::to_string(intvalue);
                    ImGui::GetForegroundDrawList()->AddText(NULL, skills_txt_dx, ImVec2(CDdrawXY[0][0] - (TXdx/1.4), (CDdrawXY[0][1] - (TXdx/1.2))), Paint_white,test.c_str());
                } else {
                    ImGui::GetForegroundDrawList()->AddRect(ImVec2((float)CDdrawXY[0][0] - skills_dx, (float)CDdrawXY[0][1] - skills_dx), ImVec2((float)CDdrawXY[0][0] + skills_dx, (float)CDdrawXY[0][1] + skills_dx), Paint_red, skills_dx, 0, skills_dx*2);
                }
                if (dataTable.heroTemp[i].Skill > 0) { //绘制召唤
                    int intvalue = dataTable.heroTemp[i].Skill;
                    string test = std::to_string(intvalue);
                    ImGui::GetForegroundDrawList()->AddText(NULL, skills_txt_dx, ImVec2(CDdrawXY[2][0] - (TXdx/1.4), (CDdrawXY[2][1] - (TXdx/1.2))), Paint_white,test.c_str());
                } else {
                   // ImGui::GetForegroundDrawList()->AddRect(ImVec2((float)CDdrawXY[2][0] - skills_dx, (float)CDdrawXY[2][1] - skills_dx), ImVec2((float)CDdrawXY[2][0] + skills_dx, (float)CDdrawXY[2][1] + skills_dx), Paint_red, skills_dx, 0, skills_dx*2);
                    
                    ImTextureID tubiaoid;
                    if(dataTable.heroTemp[i].TB!=0){
                    if(dataTable.heroTemp[i].TB==801162){
                        tubiaoid=reinterpret_cast<ImTextureID>(技能贴图.头像[dataTable.heroTemp[i].TB-800000].textureId);
    
                    }else{
                    tubiaoid=reinterpret_cast<ImTextureID>(技能贴图.头像[dataTable.heroTemp[i].TB-80000].textureId);
    }
    }else{tubiaoid =reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId);
    
                }
                    ImTextureID tubiaoid2 =reinterpret_cast<ImTextureID>(技能贴图.头像[0].textureId);
    
                
                    ImGui::GetForegroundDrawList()->AddImage(tubiaoid!= NULL ?  tubiaoid: tubiaoid2, {(float)CDdrawXY[2][0] - TXdx, CDdrawXY[2][1] - TXdx}, {(float)CDdrawXY[2][0] + TXdx, CDdrawXY[2][1] + TXdx});//顶上头像
           
                    
                }
                if (hp1 <= 0)
                  //  draw->AddLine(ImVec2((CDdrawXY[1][0] - TXdx), (CDdrawXY[1][1] - TXdx)), ImVec2((CDdrawXY[1][0] + TXdx), (CDdrawXY[1][1] + TXdx)), Paint_red, 10.0f);
                    ImGui::GetForegroundDrawList()->AddText(NULL, 80, ImVec2(CDdrawXY[1][0] - (TXdx/1.2)-7, CDdrawXY[1][1] - (TXdx/0.45)), Paint_red,"x");
            }
                
                    
                    opop++;
                    
                    
                    


}





        }
    
    
    
    }
    
    
    
    uint64_t BuffAddress;//buff数量地址
uint64_t BxAddress;//兵线数量地址
uint64_t bxAddress;//兵线属性首位置
uint64_t Buffsl;//buff坐标数量
    

if(野怪){
    
    //              ReadZZ(ReadZZ(ReadZZ(ReadZZ(ReadZZ(ReadZZ(libbuff) + 0xf0) + 0x108) + 0x138)+0x88)+0x120)+0x0;
        long temp2 = driver->read<uint64_t>(libGame_base + 0x3728D88);
        BuffAddress = driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>((temp2 + 0x3b0)) + 0x88)+0x120)+0x0;
        for (int i = 0; i < 21; i++) {
            dataTable.pve[i].cd = driver->read<uint64_t>((u_long)(BuffAddress + i * 0x18)) + 0x238;
            dataTable.pve[i].maxcd = driver->read<uint64_t>((u_long)(BuffAddress + i * 0x18)) + 0x1e4;
            dataTable.pve[i].cc = driver->read<uint64_t>((u_long)(BuffAddress + i * 0x18));
            
            dataTable.pve[i].X = dataTable.pve[i].cc + 0x2b0;
            dataTable.pve[i].Y = dataTable.pve[i].cc + 0x2b8;
        }
        
        for (int i = 0; i < 21; i++) {
            dataTable.pveTemp[i].cd = driver->read<int>(dataTable.pve[i].cd) / 1000;
            dataTable.pveTemp[i].maxcd = driver->read<int>(dataTable.pve[i].maxcd) / 1000;
            dataTable.pveTemp[i].coord.X = (float) driver->read<int>(dataTable.pve[i].X);
            dataTable.pveTemp[i].coord.Y = (float) driver->read<int>(dataTable.pve[i].Y);
            dataTable.pveTemp[i].coord.X = (float)(dataTable.pveTemp[i].coord.X * rientation * Wwra / 50000.0f + Wwra);
            dataTable.pveTemp[i].coord.Y = (float)(dataTable.pveTemp[i].coord.Y * rientation * Wwra / 50000.0f  * -1 + Wwra);
        }
        if(1==1){
            for (int i = 0; i <21; i++) {
                
                if(dataTable.pveTemp[i].cd ==90||dataTable.pveTemp[i].cd == 70||dataTable.pveTemp[i].maxcd >90||dataTable.pveTemp[i].maxcd ==60){
                    continue;}
                if(dataTable.pveTemp[i].cd !=90||dataTable.pveTemp[i].cd !=70) {
                    
                    int intvalue = dataTable.pveTemp[i].cd;
                    string test = std::to_string(intvalue);
                    ImGui::GetForegroundDrawList()->AddText(NULL, 20.0f, ImVec2((float) (dataTable.pveTemp[i].coord.X +SmallMapX - 6.0+94),(float) (dataTable.pveTemp[i].coord.Y - 7.9+7)),Paint_white,test.c_str());
                }
            }
            
        }
	}
	
    if(兵线){
    
    int number;//兵线数量
    
    long temp5 = driver->read<uint64_t>(libGame_base + 0x35B7BB0);
    BxAddress = driver->read<uint64_t>(driver->read<uint64_t>(temp5 + 0x378) + 0x108);
    number = 50;
    
    
    long cont = 0;
    for (int l = 0; l < number; l++){
        bxAddress = driver->read<uint64_t>(BxAddress +( l * 0x18));
        int temp = driver->read<int>(bxAddress + 0x34);
        if (temp == foeComp) {

                dataTable.CeTemp[cont].coord.X = (float) driver->read<int>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(bxAddress + 0x1f0) + 0x10) + 0x0) + 0x10) + 0x0);
                dataTable.CeTemp[cont].coord.Y = (float) driver->read<int>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(driver->read<uint64_t>(bxAddress + 0x1f0) + 0x10) + 0x0) + 0x10) + 0x8);
                if (dataTable.CeTemp[cont].coord.X == 0 || dataTable.CeTemp[cont].coord.Y == 0) {continue;}
                dataTable.CeTemp[cont].coord.X = dataTable.CeTemp[cont].coord.X * rientation * Wwra / 50000.0f + Wwra;
                dataTable.CeTemp[cont].coord.Y = dataTable.CeTemp[cont].coord.Y * rientation * Wwra / 50000.0f * -1 + Wwra;
                cont++;
            
        }
    }
    dataTable.xbsl = cont;
    
    
    
            for(int i = 0; i < dataTable.xbsl;i++){
                float x = dataTable.CeTemp[i].coord.X + SmallMapX+94;
                float y = dataTable.CeTemp[i].coord.Y + SmallMapY+7;
                ImGui::GetForegroundDrawList()->AddRect(ImVec2((float)x - soldiers_dx, (float)y - soldiers_dx), ImVec2((float)x + soldiers_dx, (float)y + soldiers_dx), Paint_red, soldiers_dx, 0, soldiers_dx*2);
            }
        }
    
    
    
}

