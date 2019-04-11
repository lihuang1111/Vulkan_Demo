#include <cutils/memory.h>

#include <unistd.h>
#include <utils/Log.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <media/stagefright/foundation/ADebug.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>
#include <ui/DisplayInfo.h>
#include <android/native_window.h>
#include <system/window.h>
#include <ui/GraphicBufferMapper.h>

#include "VulkanMain.h"
#include <android/log.h>

using namespace android;


bool getDataiFromFile(const char *path,unsigned char * pData,int size){
	FILE *fp = fopen(path,"rb");
	if(fp == NULL){
		printf("read %s fail !!!!!!!!!!!!!!!!!!!\n",path);
		return false;
	}
	fread(pData,size,1,fp);
	fclose(fp);
	return true;
}

int main(void){
	// set up the thread-pool
    sp<ProcessState> proc(ProcessState::self());
    ProcessState::self()->startThreadPool();
	
	// create a client to surfaceflinger
    sp<SurfaceComposerClient> client = new SurfaceComposerClient();
	sp<IBinder> dtoken(SurfaceComposerClient::getBuiltInDisplay(
            ISurfaceComposer::eDisplayIdMain));
	DisplayInfo dinfo;
	// get screenInfo
    status_t status = SurfaceComposerClient::getDisplayInfo(dtoken, &dinfo);
	printf("w=%d,h=%d,xdpi=%f,ydpi=%f,fps=%f,ds=%f\n", 
        dinfo.w, dinfo.h, dinfo.xdpi, dinfo.ydpi, dinfo.fps, dinfo.density);
    if (status)
        return -1;
	// create surface
    sp<SurfaceControl> surfaceControl = client->createSurface(String8("showRGBA"),
            dinfo.w, dinfo.h, HAL_PIXEL_FORMAT_RGBA_8888, 0);
			
    // get data from file			
	int width,height;
	width = 1920;
	height = 1080;
//	int size = width * height * 4;
//	unsigned char *data = new unsigned char[size];
//	const char *path = "/data/1920x1080_RGBA8888.bin";
//	getDataiFromFile(path,data,size);
	
    /**************set surface config*****************/
    SurfaceComposerClient::Transaction{}.setLayer(surfaceControl, 0x7FFFFFFF)
		.setSize(surfaceControl, width, height)
		.setPosition(surfaceControl, 0, 0)
		.show(surfaceControl);

	//SurfaceComposerClient::openGlobalTransaction();
    //surfaceControl->setLayer(0X7FFFFFFF);//set z-pos
	//surfaceControl->setPosition(0, 0);//set xoffset and yoffset
	//surfaceControl->setSize(width, height);//set actW actH
    //SurfaceComposerClient::closeGlobalTransaction();

	//surfaceControl->show();
	
	//sp<Surface> surface = surfaceControl->getSurface();
	sp<ANativeWindow> win = surfaceControl->getSurface();

	InitVulkan(win.get());

	do {
    	// render if vulkan is ready
    	if (IsVulkanReady()) {
      		VulkanDrawFrame();
    	}
	} while (true);

	DeleteVulkan();
/*
    ANativeWindow_Buffer outbuffer;

	//while(1)
	{
		surface->lock(&outbuffer, NULL);
		memcpy(outbuffer.bits, data, size);
		surface->unlockAndPost();
		usleep(100000);

	}
*/
	
	printf("[%s][%d]\n",__FILE__,__LINE__);
	
/**********************显示yuv数据******************************************************************/	

	
	IPCThreadState::self()->joinThreadPool();//能够保证画面一直显示，否则瞬间消失
    IPCThreadState::self()->stopProcess();
	return 0;
}
