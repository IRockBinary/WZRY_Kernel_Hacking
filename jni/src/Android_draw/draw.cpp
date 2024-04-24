#include <draw.h>
#include "DrawPlayer.hpp"
#include "Layout.hpp"
#include "读取.h"
#include <pthread.h>         // for thread(多线程)
void *handle;// 动态库方案
EGLDisplay display = EGL_NO_DISPLAY;
EGLConfig config;
EGLSurface surface = EGL_NO_SURFACE;
ANativeWindow *native_window;
ExternFunction externFunction;
ANativeWindow *(*createNativeWindow)(const char *surface_name, uint32_t screen_width, uint32_t screen_height, bool author);
EGLContext context = EGL_NO_CONTEXT;

Screen full_screen;
int Orientation = 0;
int screen_x = 0, screen_y = 0;
int init_screen_x = 0, init_screen_y = 0;
bool g_Initialized = false;
bool temp=true;



ImVec2 Pos;
int winWidth;
int winHeith;
string exec(string command) {
    char buffer[128];
    string result = "";
    // Open pipe to file
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }
    // read till end of process:
    while (!feof(pipe)) {
        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != nullptr){
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
}


int init_egl(int _screen_x, int _screen_y, bool log) {
          if (get_android_api_level() <= /*__ANDROID_API_R__*/ 30) {
               native_window = externFunction.createNativeWindow("Ssage",_screen_x, _screen_y, false);     
          }else{
    native_window = externFunction.createNativeWindow("Ssage",_screen_x, _screen_y,2,64, false);
    }

    ANativeWindow_acquire(native_window);


    ANativeWindow_acquire(native_window);
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        printf("eglGetDisplay error=%u\n", glGetError());
        return -1;
    }
    if(log){
        printf("eglGetDisplay ok\n");
    }
    if (eglInitialize(display, 0, 0) != EGL_TRUE) {
        printf("eglInitialize error=%u\n", glGetError());
        return -1;
    }
    if(log){
        printf("eglInitialize ok\n");
    }
    EGLint num_config = 0;
    const EGLint attribList[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BLUE_SIZE, 5,   //-->delete
            EGL_GREEN_SIZE, 6,  //-->delete
            EGL_RED_SIZE, 5,    //-->delete
            EGL_BUFFER_SIZE, 32,  //-->new field
            EGL_DEPTH_SIZE, 16,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
    };
    if (eglChooseConfig(display, attribList, nullptr, 0, &num_config) != EGL_TRUE) {
        printf("eglChooseConfig  error=%u\n", glGetError());
        return -1;
    }
    if(log){
        printf("num_config=%d\n", num_config);
    }
    if (!eglChooseConfig(display, attribList, &config, 1, &num_config)) {
        printf("eglChooseConfig  error=%u\n", glGetError());
        return -1;
    }
    if(log){
        printf("eglChooseConfig ok\n");
    }
    EGLint egl_format;
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &egl_format);
    ANativeWindow_setBuffersGeometry(native_window, 0, 0, egl_format);
    const EGLint attrib_list[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, attrib_list);
    if (context == EGL_NO_CONTEXT) {
        printf("eglCreateContext  error = %u\n", glGetError());
        return -1;
    }
    if(log){
        printf("eglCreateContext ok\n");
    }
    surface = eglCreateWindowSurface(display, config, native_window, nullptr);
    if (surface == EGL_NO_SURFACE) {
        printf("eglCreateWindowSurface  error = %u\n", glGetError());
        return -1;
    }
    if(log){
        printf("eglCreateWindowSurface ok\n");
    }
    if (!eglMakeCurrent(display, surface, surface, context)) {
        printf("eglMakeCurrent  error = %u\n", glGetError());
        return -1;
    }
    if(log){
        printf("eglMakeCurrent ok\n");
    }
    return 1;
}

void screen_config(){
   	//获取分辨率
	// getWmSize(px,py);

    std::string window_size = exec("wm size");
    sscanf(window_size.c_str(),"Physical size: %dx%d",&screen_x,&screen_y);

    full_screen.ScreenX = 2400;
    full_screen.ScreenY = 1080;

    std::thread *orithread = new std::thread([&] {
        while(true){
			
				//正常机和不正常机
                    const char * tempstr = exec("dumpsys input | grep SurfaceOrientation | awk '{print $2}' | head -n 1").c_str();
                    if(strlen(tempstr)==2){
                    int tempx1 = atoi(tempstr);
                    if(tempx1 == 0){
                    Orientation = 0;
                    screen_x = full_screen.ScreenX;
                    screen_y = full_screen.ScreenY;
                    }else if(tempx1 == 2){
                    Orientation = 2;
                    screen_x = full_screen.ScreenX;
                    screen_y = full_screen.ScreenY;
                    }else if(tempx1 == 1){
                    Orientation = 1;
                    screen_x = full_screen.ScreenY;
                    screen_y = full_screen.ScreenX;
                    }else if(tempx1 == 3){
                    Orientation = 3;
                    screen_x = full_screen.ScreenY;
                    screen_y = full_screen.ScreenX;
                    }
                    }
            else{
                         std::string tempstr = exec("dumpsys input |grep orientation | tail -n 1 | awk '{print $6}'");
                        int tempx = atoi(tempstr.substr(12,1).c_str());
                        if(tempx == 0 || tempx ==1 || tempx == 2 || tempx==3 ){
                            Orientation = tempx;
                            if(Orientation == 1 || Orientation == 3){
						        screen_x = full_screen.ScreenY;
                                screen_y = full_screen.ScreenX;
					        }else{
						        screen_x = full_screen.ScreenX;
                                screen_y = full_screen.ScreenY;
                            }
                        // printf("now orientation :%d\n",tempx);
                }
            }

     
            std::this_thread::sleep_for(1s);
        }
      
    });
    orithread->detach();
}

char *读取文件(char *path)
{
	FILE *fp;
	if ((fp = fopen(path, "r")) == NULL)
	{
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp);
	char *str;
	str = (char *)malloc(filesize);
	rewind(fp);
	fread(str, 1, filesize, fp);
	str[filesize] = '\0';
	fclose(fp);
	return str;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>

#include<fstream>
#include<iostream>
#include <iostream>
#include<thread>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
void 写出文件(char *aaa,char *bbb){
	
	system("mkdir -p  /storage/emulated/0/颜色");
	std::ofstream ofs;
	
	
	ofs.open(aaa, std::ios::out);

	ofs << bbb 
		;

	ofs.close();
}





void AimBotAuto(ImDrawList *Draw)
{   
    bool isDown = false;
    // 是否按下触摸
    double leenx = 0.0f;
    // x轴速度
    double leeny = 0.0f;
    // y轴速度     
    double de = 1.5f;
    // 顶部不可触摸区域
   
    double tx = NumIo[5], ty = NumIo[6];
    // 触摸点位置

    float SpeedMin = 2.0f;
    // 临时触摸速度

    double w = 0.0f, h = 0.0f, cmp = 0.0f;
    // 宽度 高度 正切

    double ScreenX = screen_x, ScreenY = screen_y;
    // 分辨率(竖屏)PS:滑屏用的坐标是竖屏状态下的

    double ScrXH = ScreenX / 2.0f;
    // 一半屏幕X

    double ScrYH = ScreenY / 2.0f;
    // 一半屏幕X

    static float TargetX = 0;
    static float TargetY = 0;
    // 触摸目标位置
    
    Vector3A obj;
    
    float NowCoor[3];
   
    Touch_Init(&screen_x,&screen_y);    
    
    while (1)
    {
        
        if (!DrawIo[20])
        {           
            if (isDown == true)
            {
                tx = NumIo[5], ty = NumIo[6];
                // 恢复变量 
                Touch_Up(8);
                // 抬起
                isDown = false;
            }
            usleep(NumIo[9] * 1000);
            continue;
        }
        
        
        // 获取目标

    
        

            if (isDown == false)
            {
                if (NumIo[10] == 0.0f)
                    Touch_Down(8,(int)tx, (int)ty);
                else
                    Touch_Down(8,py*2-(int)tx, px*2-(int)ty);
                isDown = true;
            }
            
            
        usleep(NumIo[9] * 1000);
    }
}



void ImGui_init(){
    if (g_Initialized){
        return;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    switch (Color[0])
    {
        case 0:
           // ImGui::StyleColorsRed();
		   ImGui::StyleColorsDark;
        break;
        case 1:
            ImGui::StyleColorsBlue();
        break;
        case 2:
            ImGui::StyleColorsOrange();
        break;
	}
    
    //testtexure =  createTexture("/data/data/com.OpenRoot.Canvas/test.png");
    //这边自己填图片路径
   // 获取头像();
	获取头像2();
	获取图标();
	
	
	char *ndhdh=读取文件("/storage/emulated/0/颜色/小x");
			if(ndhdh){SmallMapX= atof(ndhdh);}
			char *ndhdh2=读取文件("/storage/emulated/0/颜色/小y");
			if(ndhdh2){SmallMapY= atof(ndhdh2);}
	char *ndhdh3=读取文件("/storage/emulated/0/颜色/大x");
			if(ndhdh3){SmallHPX= atof(ndhdh3);}
			char *ndhdh5=读取文件("/storage/emulated/0/颜色/大y");
			if(ndhdh5){SmallHPY= atof(ndhdh5);}
	
			char *ndhdh6=读取文件("/storage/emulated/0/颜色/头像大小");
			if(ndhdh6){touxiangdaxiao= atof(ndhdh6);}
			
			
			char *ndhdh7=读取文件("/storage/emulated/0/颜色/小地图血量大小");
			if(ndhdh7){xiaodituxue= atof(ndhdh7);}
			
			char *ndhdh8=读取文件("/storage/emulated/0/颜色/小地图头像间隔");
			if(ndhdh8){jiange= atof(ndhdh8);}
	
			string LoadFile = "/sdcard/颜色/颜色配置";
	
	ReadDrawSet((总颜色 *)&颜色配置, LoadFile.c_str());
	
			
			
			char *ndhdh9=读取文件("/storage/emulated/0/颜色/分辨率使用");
			if(ndhdh9){fenblxx= 1;}else{fenblxx=0;}
			
			char *ndhdh10=读取文件("/storage/emulated/0/颜色/技能栏左右");
			if(ndhdh10){jinenglanz= atof(ndhdh10);}
			
			
    ImGui_ImplAndroid_Init(native_window);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontFromMemoryTTF((void *) font_v, font_v_size, 28.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontDefault(&font_cfg);
    ImGui::GetStyle().ScaleAllSizes(3.0f);
    g_Initialized = true;
}

void 开启自瞄(){
    
    
}

void 自动惩戒(){
    
       // WriteAddress_DWORD(a2, 70012);
    
}

void 自动斩杀(){
    char mname[] = "libil2cpp.so"; // 基址入口模块
        long int fool = get_module_base(pid, mname);
        long int a1 = getPtr32(getPtr32(fool + 0xA1ECB1C) + 0x50)+0x3C;
        //long int a2 = getPtr32(fool + 0x37F734) + 0x2F8;
        WriteAddress_DWORD(a1, 1071225242);
       // WriteAddress_DWORD(a2, 70012);
    
}

void 自动抢龙(){
    char mname[] = "libil2cpp.so"; // 基址入口模块
        long int fool = get_module_base(pid, mname);
        long int a1 = getPtr32(getPtr32(fool + 0xA1ECB1C) + 0x50)+0x3C;
        //long int a2 = getPtr32(fool + 0x37F734) + 0x2F8;
        WriteAddress_DWORD(a1, 1071225242);
       // WriteAddress_DWORD(a2, 70012);
    
}

void 透视(){
    char mname[] = "libil2cpp.so"; // 基址入口模块
        long int fool = get_module_base(pid, mname);
        long int a1 = getPtr32(getPtr32(fool + 0xA1ECB1C) + 0x50)+0x3C;
        //long int a2 = getPtr32(fool + 0x37F734) + 0x2F8;
        WriteAddress_DWORD(a1, 1071225242);
       // WriteAddress_DWORD(a2, 70012);
    
}
    
int huitu=0;
void tick()
{
    
         
	
        ImGuiIO& io = ImGui::GetIO();
        if (display == EGL_NO_DISPLAY)
        return;
        static ImVec4 clear_color = ImVec4(0, 0, 0, 0);
        ImGui_ImplOpenGL3_NewFrame();    
        ImGuiStyle& Style = ImGui::GetStyle();
        ImGui_ImplAndroid_NewFrame(init_screen_x, init_screen_y);
        ImGui::NewFrame();
		
		static bool IsBall = true;
    	static float ANIM_SPEED = 0.25f;
    	static float Velua = IsBall ? 0.0f : 1.0f;
    	Velua = ImClamp(Velua + (io.DeltaTime / ANIM_SPEED) * (IsBall ? 1.0f : -1.0f), 0.0f, 1.0f);

         if ((950 * Velua) <= 100 && !IsBall)
         {BallSwitch = true;MemuSwitch = false;}
         

         
		if (BallSwitch)
    	{
    		Style.WindowRounding = 80.0f;
    		if (ImGui::Begin("Ball", &BallSwitch,  ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoScrollbar))
        	{
				auto Pos = ImGui::GetWindowPos();
    			Window = ImGui::GetCurrentWindow();
    			DrawLogo({Pos.x + 50, Pos.y + 50}, 40);
    			if (ImGui::IsItemActive()) {
    				if (!IsDown) {
    					IsDown = true;
    					ImagePos = ImGui::GetWindowPos();
    				}
    			} else if (IsDown) {
    				IsDown = false;
    				if(ImagePos.x == ImGui::GetWindowPos().x && ImagePos.y == ImGui::GetWindowPos().y) {
    					IsBall = true;
    					MemuSwitch = true;
    					BallSwitch = false;
    			    	ImGui::SetWindowPos("HY_内核", Pos, ImGuiCond_Always);
    				}
    			}
    		}
    		ImGui::End();
    	}
        
        if(MemuSwitch)
        {
			Style.WindowRounding = 10.0f;
            if (ImGui::Begin("王者荣耀 - 无敌Jni小子 - 内核版本", &MemuSwitch,  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
            {
				auto Pos = ImGui::GetWindowPos();
        		Window = ImGui::GetCurrentWindow();
    			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {25 , 25});
    			if (ImGui::BeginChild("##主菜单", ImVec2(220, -1), false, ImGuiWindowFlags_NoScrollbar))
    			{
                    //DrawLogo({Pos.x + 117, Pos.y + 100}, 80);
					ImGui::Text("无敌Jni小子");
    				ImGui::SetCursorPos({0, 90});
    				//ImGui::Separator();
    				ImGui::ItemSize(ImVec2(0, 2));
                    if (ImGui::Button("主页", ImVec2(220, 75)))
					{
                        MenuTab = 1;
                    }
    				ImGui::ItemSize(ImVec2(0, 2));
                    if (ImGui::Button("绘制", ImVec2(220, 75)))
					{
                        MenuTab = 2;
                    }
    				ImGui::ItemSize(ImVec2(0, 2));
                    if (ImGui::Button("颜色", ImVec2(220, 75)))
					{
                        MenuTab = 3;
                    }
					
					ImGui::ItemSize(ImVec2(0, 2));
                    if (ImGui::Button("其他", ImVec2(220, 75)))
					{
                        MenuTab = 4;
                    }
					
                }
                ImGui::EndChild();
    			ImGui::PopStyleVar(1);
    			ImGui::SameLine();
    			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    			ImGui::SameLine();
    			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {25 , 25});
    			ImGui::SameLine();
                if (ImGui::BeginChild("##右菜单", ImVec2(-1, -1), false))
                {
                    ImGui::ItemSize(ImVec2(0, 4));
                    ImGui::SameLine();
                    if (ImGui::GetWindowWidth() - (100 + 50 + 30) > 0) {
                        ImGui::InvisibleButton("缩小", {ImGui::GetWindowWidth() - (100 + 50 + 30), 0});
                    }
                    ImGui::SameLine();
                    if (ButtonTextColored(Style.Colors[ImGuiCol_Button],"缩小"))
                    {
                        IsBall = false;
    					ImGui::SetWindowPos("Ball", Pos, ImGuiCond_Always);
    				}
    
                   ImGui::ItemSize(ImVec2(0, 4));
                   ImGui::ItemSize(ImVec2(0, 10));
                    switch (MenuTab)
                    {
                    case 1:
                    {
                      ImGui::Text("内核驱动状态良好\n");
					  
					  
							  if(pid>0){
					  ImGui::Text("游戏分辨率:%dx%d",yxpx<yxpy?yxpx:yxpy , yxpx<yxpy?yxpy:yxpx);
					  }
					  
					  
					  
                      
					  ImGui::Text("内核分辨率:%dx%d",screen_x<screen_y?screen_x:screen_y, screen_x<screen_y?screen_y:screen_x);
					   if(pid>0){
						   if(fenblxx==0){
						   if(ImGui::Button("使用内核分辨率"))
					{ 
							int x1,y1;
							
							x1=screen_x<screen_y?screen_x:screen_y;
							y1=screen_x<screen_y?screen_y:screen_x;
							if(yxpx<yxpy)
							{yxpx=x1;yxpy=y1;}else{yxpy=x1;yxpx=y1;}
							fenblxx=1;
					  char urlls[2560];
		sprintf(urlls, "%d", fenblxx);
		
		写出文件("/storage/emulated/0/颜色/分辨率使用",urlls);
		
					  }}else{
					    if(ImGui::Button("使用游戏分辨率"))
					{ 
						
						int x1,y1;
							long tmp = libunity_base + 0x14D3DEC;
				
						
						fenblxx=0;
						
		system("rm /storage/emulated/0/颜色/分辨率使用");
		
						
						
						}
						}
						ImGui::SameLine();
						ImGui::Text("帧率 %.1f FPS",ImGui::GetIO().Framerate);
					  
					  
					  }
                      SetTheme("切换主题");
				 ImGui::Text("\n");
				/*	if(ImGui::Button("        验证内核        \n")){ 
					 if (syscall(435,963008) == 666)  //如果成功执行，返回0。如果没有调用成功，一般会返回-1.
		printf("验证成功!\n");
		else
		printf("失败!\n");
					  }*/
                      ImGui::Text("\n");
					  if(huitu==0){
					  if(ImGui::Button("\n        开启辅助        \n\n")){DrawInit();
						  
		 huitu=1;血量=1;地图=1;射线=1;方框=1;野怪=1;兵线=1;方框技能=1;顶上技能=1;上帝=0;坐标自瞄=0;惩戒=0;斩杀=0;抢龙=0;}
						  }else{
 if(ImGui::Button("\n        关闭绘制        \n\n")){huitu=0;血量=0;地图=0;射线=0;方框=0;野怪=0;兵线=0;方框技能=0;顶上技能=0;上帝=0;坐标自瞄=0;惩戒=0;斩杀=0;抢龙=0;}
						  
							  
						  }
                      ImGui::SameLine();
					  ImGui::Text("     ");
					  ImGui::SameLine();
					  if(ImGui::Button("\n          退出          \n\n")){exit(1);}
                    }
                    break;
                    case 2: 
                    {
                      ImGui::Text("偏移上帝 %p",a1);
                      ImGui::Checkbox("地图血量", &血量);
		              ImGui::SameLine();
                      ImGui::Checkbox("人物方框", &方框);
					  ImGui::SameLine();
                      ImGui::Checkbox("方框技能", &方框技能);
	                  ImGui::Checkbox("显示射线", &射线);
		              ImGui::SameLine();
		              ImGui::Checkbox("地图头像", &地图);
					  ImGui::SameLine();
		              ImGui::Checkbox("顶上技能", &顶上技能);
		              ImGui::Checkbox("显示野怪", &野怪);
					  ImGui::SameLine();
		              ImGui::Checkbox("显示兵线", &兵线);
		              ImGui::SameLine();
		              if(ImGui::Checkbox("近景上帝", &上帝)){
		             
		          透视();
		             
		          }
		           ImGui::SameLine();
		             if(ImGui::Checkbox("开启自瞄", &坐标自瞄)){
		             
		          开启自瞄();
		             
		          }
		             ImGui::SameLine();
		             if(ImGui::Checkbox("自动惩戒", &惩戒)){
		             
		          自动惩戒();
		             
		          }
		                         ImGui::SameLine();
		             if(ImGui::Checkbox("自动斩杀", &斩杀)){
		             
		          自动斩杀();
		             
		          }
		                     ImGui::SameLine();
		             if(ImGui::Checkbox("自动抢龙", &抢龙)){
		             
		          自动抢龙();
		             
		          }
					  ImGui::Text("\n");
					  
					  if(ImGui::SliderInt("头像X", &SmallMapX,-300,300,"%.0f",2)){char urlls[2560];
		sprintf(urlls, "%d", SmallMapX);
		
		写出文件("/storage/emulated/0/颜色/小x",urlls);}
		              if(ImGui::SliderInt("头像Y", &SmallMapY,-300,300,"%.0f",2)){char urlls[2560];
		sprintf(urlls, "%d", SmallMapY);
		
		写出文件("/storage/emulated/0/颜色/小y",urlls);}
		              if(ImGui::SliderInt("大地图X", &SmallHPX,-300,300,"%.0f",2)){char urlls[2560];
		sprintf(urlls, "%d", SmallHPX);
		
		写出文件("/storage/emulated/0/颜色/大x",urlls);}
		              if(ImGui::SliderInt("大地图Y", &SmallHPY,-300,300,"%.0f",2)){char urlls[2560];
		sprintf(urlls, "%d", SmallHPY);
		
		写出文件("/storage/emulated/0/颜色/大y",urlls);}
					  
					  
					  if(ImGui::SliderInt("技能栏左右", &jinenglanz,-500,500,"%.0f",2)){char urlls[2560];
		sprintf(urlls, "%d", jinenglanz);
		
		写出文件("/storage/emulated/0/颜色/技能栏左右",urlls);}
					  }
                    break;
                    case 3:
                    {
                       ImGui::ColorEdit3("血量颜色",(float*)&颜色配置.血量颜色);
					   ImGui::ColorEdit3("射线颜色",(float*)&颜色配置.射线颜色);
					   ImGui::ColorEdit3("方框颜色",(float*)&颜色配置.方框颜色);
					   ImGui::Text("\n\n\n");
					         if (ImGui::Button("保存配置",{400, 80})) {
                  
                    string SaveFile ;
                    
                    SaveFile = "/sdcard/颜色/颜色配置";
          
					SaveDrawSet((总颜色 *)&颜色配置, SaveFile.c_str());
                    }
					                if (ImGui::Button("删除颜色配置",{400, 80})) {
                    system("rm /sdcard/颜色/颜色配置");
                    }
					
					
					
                    }
                     break;
					 
					 case 4:
                    {
                       
					if(ImGui::SliderInt("小地图头像大小", &touxiangdaxiao,-300,300,"%.0f",2)){char urlls[2560];
		sprintf(urlls, "%d", touxiangdaxiao);
		
		写出文件("/storage/emulated/0/颜色/头像大小",urlls);}
					
		if(ImGui::SliderInt("小地图血量大小", &xiaodituxue,-80,80,"%.0f",2)){char urlls[2560];
		sprintf(urlls, "%d", xiaodituxue);
		
		写出文件("/storage/emulated/0/颜色/小地图血量大小",urlls);}
					
		if(ImGui::SliderInt("小地图头像间隔", &jiange,-100,100,"%.0f",2)){char urlls[2560];
		sprintf(urlls, "%d", jiange);
		
		写出文件("/storage/emulated/0/颜色/小地图头像间隔",urlls);}
		
		
		
                    }
                     break;
                    }
                }
                ImGui::EndChild();
                ImGui::PopStyleVar(1);
            }
            ImGui::End();
        }
		
		if (IsWin)
		{
			IsWin = false;
            IsLoGin = false;
            BallSwitch = false;
            ImGui::SetWindowSize("Ball", {100.0f, 100.0f});
                     if(temp){
         temp=false;
             ImGui::SetWindowSize("内核", {950.0f, 600.0f});
             }
        }  
	    
	/*****绘制帧率*****/
	char fkhs[1000];
    //sprintf(fkhs,"%.1f FPS",ImGui::GetIO().Framerate);
  //  ImGui::GetForegroundDrawList()->AddText(ImVec2(300, 1000),ImColor(225,255,255),fkhs);
   // ImGui::GetForegroundDrawList()->AddText(ImVec2(90, 1000),ImColor(225,255,255),"HY_内核 状态稳定");
    /*****结束*****/
	if(huitu==1){
	DrawPlayer(); //调用绘图  直接写在这边的话有点影响观看了
	}
    ImGui::Render();  
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    eglSwapBuffers(display, surface);
}

void shutdown()
{
    if (!g_Initialized) {
        return;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    if (display != EGL_NO_DISPLAY){
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT){
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE){
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
    ANativeWindow_release(native_window);
}
