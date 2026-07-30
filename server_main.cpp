#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <zmq.hpp>
#include "scene.hpp"

Scene *gScene;

int main(int argc, char *argv[])
{
	gScene=new Scene;

	char fileName[128];
	int32_t i;
	// for(i=0; i<8; i++, samples_per_pixel*=2)
	for(i=0; i<10; i++)
	{
		// float spec=i*0.1;

		Ray::REFLECTIONS_LIMIT+=1;
		Scene::SAMPLES_PER_PIXEL*=2;

		gScene->Render();

		sprintf(fileName, "render_%02i_%luspp.png", i, Scene::SAMPLES_PER_PIXEL);
		gScene->RenderedImage.write(fileName);
	}

	return 0;
}
