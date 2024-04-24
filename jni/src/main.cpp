#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <fstream>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <sys/system_properties.h>
#include <ctime>
#include <main.h>
#include <stdlib.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include<thread>

int main()
{
    printf("\033[35;1m");		// 粉红色	
         screen_config();
    init_screen_x = screen_x + screen_y;
    init_screen_y = screen_y + screen_x;
	
	if(!init_egl(init_screen_x,init_screen_y)) 
	{     
     screen_config();
    }
    ImGui_init();//imgui初始化
	new std::thread(AimBotAuto,ImGui::GetForegroundDrawList());	
    while (1)
    {     
	  tick();  
    }
    shutdown();
    return 0;


}
