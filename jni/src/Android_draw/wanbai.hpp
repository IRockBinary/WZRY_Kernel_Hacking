#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include<dirent.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

using namespace std;
int 对象数量;
float 矩阵节点[16] = {0}, camera, x, y, w, h, r_x, r_y, r_w;
float X, Y, W;

int pid;
float px,py;
char lj[64];
int isSize = 4;//默认32位

char *结束时间;
bool 已挂载;
class CDriver {
private:
    int fd;
    pid_t pid;

    typedef struct _COPY_MEMORY {
        pid_t pid;
        uintptr_t addr;
        void* buffer;
        size_t size;
    } COPY_MEMORY, *PCOPY_MEMORY;

    typedef struct _MODULE_BASE {
        pid_t pid;
        char* name;
        uintptr_t base;
    } MODULE_BASE, *PMODULE_BASE;

    enum OPERATIONS {
        OP_INIT_KEY = 0x800,
        OP_READ_MEM = 0x801,
        OP_WRITE_MEM = 0x802,
        OP_MODULE_BASE = 0x803,
    };

    char *driver_path()
    {
        // 打开/dev目录
        const char *dev_path = "/dev";
        DIR *dir = opendir(dev_path);
        if (dir == NULL)
        {
            printf("无法打开/dev目录\n");
            return NULL;
        }
        char *searchFiles[] = { "wanbai", "CheckMe", "Ckanri", "lanran" };
        struct dirent *entry;
        char *file_path = NULL;
        while ((entry = readdir(dir)) != NULL)
        {
            // 跳过当前目录和上级目录
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            size_t path_length = strlen(dev_path) + strlen(entry->d_name) + 2;
            file_path = (char *)malloc(path_length);
            snprintf(file_path, path_length, "%s/%s", dev_path, entry->d_name);

            // 获取文件stat结构
            struct stat file_info;
            if (stat(file_path, &file_info) < 0)
            {
                free(file_path);
                file_path = NULL;
                continue;
            }

            // 判断文件名是否包含搜索数组中的设备名
            bool is_desired_driver = false;
            for (int i = 0; i < 4; i++)
            {
                if (strstr(entry->d_name, searchFiles[i]) != NULL)
                {
                    is_desired_driver = true;
                    break;
                }
            }

            if (!is_desired_driver)
            {
                free(file_path);
                file_path = NULL;
                continue;
            }

            // 排除不符合条件的文件
            if ((S_ISCHR(file_info.st_mode) || S_ISBLK(file_info.st_mode)) && strchr(entry->d_name, '_') == NULL)
            {
                // 排除标准输入输出和gpio接口
                if (strcmp(entry->d_name, "stdin") == 0 || strcmp(entry->d_name, "stdout") == 0 || strcmp(entry->d_name, "stderr") == 0)
                {
                    free(file_path);
                    file_path = NULL;
                    continue;
                }

                if (strstr(entry->d_name, "gpiochip") != NULL)
                {
                    free(file_path);
                    file_path = NULL;
                    continue;
                }

                // 检查文件大小是否为0且创建年份是否为1970
                if (file_info.st_size == 0 || localtime(&file_info.st_ctime)->tm_year + 1900 == 1970)
                {
                    free(file_path);
                    file_path = NULL;
                    continue;
                }

                closedir(dir);
                return file_path;
            }

            free(file_path);
            file_path = NULL;
        }
        closedir(dir);
        return NULL;
    }
	
    char* getDriverPath() {	
    
        ifstream procMiscFile("/proc/misc");
        if (!procMiscFile.is_open()) {
            cout << "Failed to open /proc/misc file." << endl;
            return nullptr;
        }

        string line;
        getline(procMiscFile, line); // 读取第一行
        procMiscFile.close();

        istringstream iss(line);
        string deviceNumber, deviceName;
        iss >> deviceNumber >> deviceName;
/*
		if (deviceName == "usb_accessory") {
			cout << "Failed to open /proc/misc file." << endl;
            return nullptr;
		}
		*/
        string DevName = "/dev/";
        DevName += deviceName;

        return strdup(DevName.c_str());
        
    }

public:
    CDriver() : fd(-1), pid(0) {
        char* deviceName = getDriverPath();
         fd = open(deviceName, O_RDWR);
		
        if (fd == -1) {
			char* deviceName2 = driver_path();
			fd = open(deviceName, O_RDWR);
			
			if (fd == -1) {
            	cout << "[-] Failed to open driver." << endl;
				free(deviceName);
            	exit(0);
			}
			
			printf("\n内核驱动:%s\n", deviceName);
        } else {
			printf("\n内核驱动:%s\n", deviceName);
		}
		
		free(deviceName);
    }

    ~CDriver() {
        if (fd > 0)
            close(fd);
    }

    void initialize(pid_t pid) {
        this->pid = pid;
    }

    bool initKey(const char* key) {
        if (ioctl(fd, OP_INIT_KEY, key) != 0) {
            return false;
        }
        return true;
    }

    bool read(uintptr_t addr, void* buffer, size_t size) {
        COPY_MEMORY cm;
        cm.pid = this->pid;
        cm.addr = addr;
        cm.buffer =buffer;
        cm.size = size;

        if (ioctl(fd, OP_READ_MEM, &cm) != 0) {
            return false;
        }
        return true;
    }

    bool write(uintptr_t addr, void* buffer, size_t size) {
        COPY_MEMORY cm;
        cm.pid = this->pid;
        cm.addr =addr;
        cm.buffer =buffer;
        cm.size =size;

        if (ioctl(fd, OP_WRITE_MEM, &cm) != 0) {
            return false;
        }
        return true;
    }

    template <typename T>
    T read(uintptr_t addr) {
        T res;
        if (this->read(addr, &res, sizeof(T)))
            return res;
        return {};
    }

    template <typename T>
    bool write(uintptr_t addr, T value) {
        return this->write(addr, &value, sizeof(T));
    }

    uintptr_t getModuleBase(const char* name) {
        MODULE_BASE mb;
        mb.pid = this->pid;
        mb.name = strdup(name);

        if (ioctl(fd, OP_MODULE_BASE, &mb) != 0) {
            free(mb.name);
            return 0;
        }
		
        uintptr_t base = mb.base;
        free(mb.name);
        return base;
    }
};
struct Vector2A {
    float X;
    float Y;

    Vector2A() {
        this->X = 0;
        this->Y = 0;
    }

    Vector2A(float x, float y) {
        this->X = x;
        this->Y = y;
    }
};


struct Vector3A {
    float X;
    float Y;
    float Z;

    Vector3A() {
        this->X = 0;
        this->Y = 0;
        this->Z = 0;
    }

    Vector3A(float x, float y, float z) {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }

};

struct Vector3{
    //这边重定义一个xzy形式的坐标(因为u3d是xzy);
    float X;
    float Z;
    float Y;

    Vector3() {
        this->X = 0;
        this->Z = 0;
        this->Y = 0;
    }

    Vector3(float x, float z, float y) {
        this->X = x;
        this->Z = z;
        this->Y = y;
    }

};

Vector2A WorldToScreen(Vector3A obj, float matrix[16], float ViewW)
{
    float x =
        px + (matrix[0] * obj.X + matrix[4] * obj.Y + matrix[8] * obj.Z + matrix[12]) / ViewW * px;
    float y =
        py - (matrix[1] * obj.X + matrix[5] * obj.Y + matrix[9] * obj.Z + matrix[13]) / ViewW * py;

    return Vector2A(x, y);
}

static CDriver* driver = new CDriver();
