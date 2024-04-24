#ifndef NATIVESURFACE_MEMREAD_H
#define NATIVESURFACE_MEMREAD_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include <dlfcn.h>




// 定义圆周率
#define PI 3.141592653589793238
typedef unsigned int ADDRESS;
typedef char PACKAGENAME;
typedef unsigned short UTF16;
typedef char UTF8;




bool mem_addr_virtophy(unsigned long vaddr)
{
	int fh = 1;
	int pageSize = getpagesize();

	// 虚拟地址相对于0X0经过的页面数
	unsigned long v_pageIndex = vaddr / pageSize;
	// 在pagemap文件中的偏移量
	unsigned long pfn_item_offset = v_pageIndex * sizeof(uint64_t);

	// 页内偏移量
	unsigned long page_offset = vaddr % pageSize;
	uint64_t item = 0;
	char filename[32];

	int fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		// printf("open failed\n");
		return false;
	}

	if (lseek(fd, pfn_item_offset, SEEK_SET) < 0)
	{
		printf("lseek failed\n");
		return false;
	}

	if (read(fd, &item, sizeof(uint64_t)) != sizeof(uint64_t))
	{
		printf("read failed\n");
		return false;
	}

	if (0 == (item & (((uint64_t) 1) << 63)))
	{
		fh = 0;
		printf("page is not present");
        return false;
	}
	close(fd);
	return true;
}



// 进程读写内存
bool pvm(void *address, void *buffer, size_t size, bool iswrite)
{
	struct iovec local[1];
	struct iovec remote[1];
	local[0].iov_base = buffer;
	local[0].iov_len = size;
	remote[0].iov_base = address;
	remote[0].iov_len = size;
	if (pid < 0)
	{
		return false;
	}
	ssize_t bytes = syscall(SYS_process_vm_readv, pid, local, 1, remote, 1, 0, iswrite);
	return bytes == size;
}

// 读取内存
bool vm_readv(unsigned long address, void *buffer, size_t size)
{
	return pvm(reinterpret_cast < void *>(address), buffer, size, false);
}

// 写入内存
bool vm_writev(unsigned long address, void *buffer, size_t size)
{
	return pvm(reinterpret_cast < void *>(address), buffer, size, true);
}

// 获取F类内存
float getfloat(unsigned long addr)
{
	float var = 0;
	vm_readv(addr, &var, 4);
	return (var);
}

// 获取F类内存
float getFloat(unsigned long addr)
{
	float var = 0;
	vm_readv(addr, &var, 4);
	return (var);
}

// 获取D类内存
int getdword(unsigned long addr)
{
	int var = 0;
	vm_readv(addr, &var, 4);
	return (var);
}

// 获取D类内存
int getDword(unsigned long addr)
{
	int var = 0;
	vm_readv(addr, &var, 4);
	return (var);
}

// 获取指针(32位游戏)
unsigned int getPtr32(unsigned int addr)
{
	if (mem_addr_virtophy(addr) || addr == 0x0000000000 || addr == 0 || addr == 0x000){
        return 0;
    }
	unsigned int var = 0;
	vm_readv(addr, &var, 8);
	return (var);
}

// 获取指针(64位游戏)
unsigned long getPtr64(unsigned long addr)
{
	if (mem_addr_virtophy(addr) || addr == 0x0000000000 || addr == 0 || addr == 0x000){
        return 0;
    }
	unsigned long var = 0;
	vm_readv(addr, &var, 8);
	return (var);
}

// 获取指针(64位游戏) 无过缺页
unsigned long getPtr641(unsigned long addr)
{
    unsigned long var = 0;
    vm_readv(addr, &var, 8);
    return (var);
}


   // 写入D类内存 
void writedword(unsigned long addr, int data)
{
   vm_writev(addr, &data, 4);
} 
    
   // 写入F类内存
void writefloat(unsigned long addr, float data)
{
	vm_writev(addr, &data, 4);
}


//获取进程
int getPID(const char *packageName)
{
    int id = -1;
    DIR *dir;
    FILE *fp;
    char filename[64];
    char cmdline[64];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL)
    {
        id = atoi(entry->d_name);
        if (id != 0)
        {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp)
            {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(packageName, cmdline) == 0)
                {
                    return id;
                }
            }
        }
    }
    closedir(dir);
    return -1;
}



// 获取基址
long int get_module_base(int pid, const char *module_name)
{
    
    FILE *fp;
    long addr = 0;
    char *pch;
    char filename[32];
    char line[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, module_name))
            {
                pch = strtok(line, "-");
                addr = strtoul(pch, NULL, 16);
                if (addr == 0x8000)
                    addr = 0;
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

uintptr_t GetModuleBase(int pid, char *name, int index)
    {
        int f = 0, ii = 0, iii = 0;
        long start = 0, end = 0;
        char line[1024] = {0};
        char fname[128];
        char dname[128];
        if (strstr(name, "bss") != NULL) {
            sscanf(name, "%[^:]", dname);
            f++;
        } else {
            sprintf(dname, "%s", name);
        }
        sprintf(fname, "/proc/%d/maps", pid);
        FILE *p = fopen(fname, "r");
        if (p)
        {
            while (fgets(line, sizeof(line), p))
            {
                if (strstr(line, dname) != NULL)
                {
                    iii++;
                    if (f == 0)
                    {
                        if (iii == index)
                        {
                            if (sizeof(long) == 8) {
                                sscanf(line, "%lx-%lx", &start, &end);
                            } else {
                                sscanf(line, "%x-%x", &start, &end);
                            }
                            break;
                        }
                    } else {
                        ii++;
                    }
                }
                if (ii > 0) {
                    if (strstr(line, "[anon:.bss]") != NULL) {
                        if (sizeof(long) == 8) {
                            sscanf(line, "%lx-%lx", &start, &end);
                        } else {
                            sscanf(line, "%x-%x", &start, &end);
                        }
                        break;
                    }
                }
            }
            fclose(p);
        }
        return start;
    }


//f获取
float WriteAddress_FLOAT(int pid,long addr,float value)
{
    char lj[64];
    int handle;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    pwrite64(handle, &value, 4, addr);
    close(handle);
    return 0;
}
//d获取
int WriteAddress_DWORD(long addr,int value)
{
    char lj[64];
    int handle;
    sprintf(lj, "/proc/%d/mem", pid);
    handle = open(lj, O_RDWR);
    lseek(handle, 0, SEEK_SET);
    pwrite64(handle, &value, 4, addr);
    close(handle);
    return 0;
}

/*
int WriteAddress_DWORD(long int addr,int value) {
	pwrite64(handle, &value, 4, addr);
	return 0;
}
*/

void WriteAddress_FLOAT(unsigned long addr, float data) {
	vm_writev(addr, &data, 4);
}





// 获取进程是否运行
int isapkrunning(PACKAGENAME * bm)
{
	DIR *dir = NULL;
	struct dirent *ptr = NULL;
	FILE *fp = NULL;
	char filepath[64];
	char filetext[64];
	dir = opendir("/proc/");
	if (dir != NULL)
	{
		while ((ptr = readdir(dir)) != NULL)
		{
			if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
				continue;
			if (ptr->d_type != DT_DIR)
				continue;
			sprintf(filepath, "/proc/%s/cmdline", ptr->d_name);
			fp = fopen(filepath, "r");
			if (NULL != fp)
			{
				fgets(filetext, sizeof(filetext), fp);
				if (strcmp(filetext, bm) == 0)
				{
					closedir(dir);
					return 1;
				}
				fclose(fp);
			}
		}
	}
	closedir(dir);
	return 0;
}

// 读取字符信息
void getUTF8(UTF8 * buf, unsigned long namepy)
{
	UTF16 buf16[16] = { 0 };
	vm_readv(namepy, buf16, 28);
	UTF16 *pTempUTF16 = buf16;
	UTF8 *pTempUTF8 = buf;
	UTF8 *pUTF8End = pTempUTF8 + 32;
	while (pTempUTF16 < pTempUTF16 + 28)
	{
		if (*pTempUTF16 <= 0x007F && pTempUTF8 + 1 < pUTF8End)
		{
			*pTempUTF8++ = (UTF8) * pTempUTF16;
		}
		else if (*pTempUTF16 >= 0x0080 && *pTempUTF16 <= 0x07FF && pTempUTF8 + 2 < pUTF8End)
		{
			*pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else if (*pTempUTF16 >= 0x0800 && *pTempUTF16 <= 0xFFFF && pTempUTF8 + 3 < pUTF8End)
		{
			*pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
			*pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else
		{
			break;
		}
		pTempUTF16++;
	}
}



// 计算骨骼

struct FMatrix
{
	float M[4][4];
};

class FRotator
{
public:
    FRotator() :Pitch(0.f), Yaw(0.f), Roll(0.f) {

    }
    FRotator(float _Pitch, float _Yaw, float _Roll) : Pitch(_Pitch), Yaw(_Yaw), Roll(_Roll)
    {

    }
    ~FRotator()
    {

    }
    float Pitch;
    float Yaw;
    float Roll;
    inline FRotator Clamp()
    {

        if (Pitch > 180)
        {
            Pitch -= 360;
        }
        else
        {
            if (Pitch < -180)
            {
                Pitch += 360;
            }
        }
        if (Yaw > 180)
        {
            Yaw -= 360;
        }
        else {
            if (Yaw < -180)
            {
                Yaw += 360;
            }
        }
        if (Pitch > 89)
        {
            Pitch = 89;
        }
        if (Pitch < -89)
        {
            Pitch = -89;
        }
        while (Yaw < 180)
        {
            Yaw += 360;
        }
        while (Yaw > 180)
        {
            Yaw -= 360;
        }
        Roll = 0;
        return FRotator(Pitch, Yaw, Roll);
    }
    inline float Length()
    {
        return sqrtf(Pitch * Pitch + Yaw * Yaw + Roll * Roll);
    }
    FRotator operator+(FRotator v) {
        return FRotator(Pitch + v.Pitch, Yaw + v.Yaw, Roll + v.Roll);
    }
    FRotator operator-(FRotator v) {
        return FRotator(Pitch - v.Pitch, Yaw - v.Yaw, Roll - v.Roll);
    }
};

struct Quat
{
	float X;
	float Y;
	float Z;
	float W;
};


struct FTransform
{
	Quat Rotation;
	Vector3A Translation;
	float chunk;
	Vector3A Scale3D;
};


float get_3D_Distance(float Self_x, float Self_y, float Self_z, float Object_x, float Object_y,
					  float Object_z)
{
	float x, y, z;
	x = Self_x - Object_x;
	y = Self_y - Object_y;
	z = Self_z - Object_z;
	// 求平方根
	return (float)(sqrt(x * x + y * y + z * z));
}

// 计算旋转坐标
Vector2A rotateCoord(float angle, float objRadar_x, float objRadar_y)
{
	Vector2A radarCoordinate;
	float s = sin((angle - 90) * PI / 180);
	float c = cos((angle - 90) * PI / 180);
	radarCoordinate.X = objRadar_x * c + objRadar_y * s;
	radarCoordinate.Y = -objRadar_x * s + objRadar_y * c;
	return radarCoordinate;
}


Vector3A MarixToVector(FMatrix matrix)
{
	return Vector3A(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
}

FMatrix MatrixMulti(FMatrix m1, FMatrix m2)
{
	FMatrix matrix = FMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
			}
		}
	}
	return matrix;
}

FMatrix TransformToMatrix(FTransform transform)
{
	FMatrix matrix;
	matrix.M[3][0] = transform.Translation.X;
	matrix.M[3][1] = transform.Translation.Y;
	matrix.M[3][2] = transform.Translation.Z;
	float x2 = transform.Rotation.X + transform.Rotation.X;
	float y2 = transform.Rotation.Y + transform.Rotation.Y;
	float z2 = transform.Rotation.Z + transform.Rotation.Z;
	float xx2 = transform.Rotation.X * x2;
	float yy2 = transform.Rotation.Y * y2;
	float zz2 = transform.Rotation.Z * z2;
	matrix.M[0][0] = (1 - (yy2 + zz2)) * transform.Scale3D.X;
	matrix.M[1][1] = (1 - (xx2 + zz2)) * transform.Scale3D.Y;
	matrix.M[2][2] = (1 - (xx2 + yy2)) * transform.Scale3D.Z;
	float yz2 = transform.Rotation.Y * z2;
	float wx2 = transform.Rotation.W * x2;
	matrix.M[2][1] = (yz2 - wx2) * transform.Scale3D.Z;
	matrix.M[1][2] = (yz2 + wx2) * transform.Scale3D.Y;
	float xy2 = transform.Rotation.X * y2;
	float wz2 = transform.Rotation.W * z2;
	matrix.M[1][0] = (xy2 - wz2) * transform.Scale3D.Y;
	matrix.M[0][1] = (xy2 + wz2) * transform.Scale3D.X;
	float xz2 = transform.Rotation.X * z2;
	float wy2 = transform.Rotation.W * y2;
	matrix.M[2][0] = (xz2 + wy2) * transform.Scale3D.Z;
	matrix.M[0][2] = (xz2 - wy2) * transform.Scale3D.X;
	matrix.M[0][3] = 0;
	matrix.M[1][3] = 0;
	matrix.M[2][3] = 0;
	matrix.M[3][3] = 1;
	return matrix;
}

FTransform getBone(unsigned long addr)
{
	FTransform transform;
	vm_readv(addr, &transform, 4 * 11);
	return transform;
}



struct D3DXMATRIX
{
	float _11;
	float _12;
	float _13;
	float _14;
	float _21;
	float _22;
	float _23;
	float _24;
	float _31;
	float _32;
	float _33;
	float _34;
	float _41;
	float _42;
	float _43;
	float _44;
};

struct D3DXVECTOR4
{
	float X;
	float Y;
	float Z;
	float W;
};



#endif
