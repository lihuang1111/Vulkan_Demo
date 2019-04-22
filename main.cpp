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

    /**************set surface config*****************/
#ifdef UES_SPHAL_OPEN
    SurfaceComposerClient::Transaction{}.setLayer(surfaceControl, 0x7FFFFFFF)
		.setSize(surfaceControl, width, height)
		.setPosition(surfaceControl, 0, 0)
		.show(surfaceControl);
#else
	SurfaceComposerClient::openGlobalTransaction();
    surfaceControl->setLayer(0X7FFFFFFF);//set z-pos
	surfaceControl->setPosition(0, 0);//set xoffset and yoffset
	surfaceControl->setSize(width, height);//set actW actH
    SurfaceComposerClient::closeGlobalTransaction();

	surfaceControl->show();
#endif
	sp<ANativeWindow> win = surfaceControl->getSurface();

	InitVulkan(win.get());

	do
	{
		// render if vulkan is ready
		if (IsVulkanReady())
		{
			VulkanDrawFrame();
		}
	}
	while (true);

	DeleteVulkan();

/**********************显示yuv数据******************************************************************/

	IPCThreadState::self()->joinThreadPool();//能够保证画面一直显示，否则瞬间消失
    IPCThreadState::self()->stopProcess();
	return 0;
}
