#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <zmq.hpp>
#include "scene.hpp"

Scene *NewScene;

int main(int argc, char *argv[])
{
	NewScene=new Scene;

	char fileName[128];
	int32_t i, samples_per_pixel=256;
	// for(i=0; i<8; i++, samples_per_pixel*=2)
	for(i=0; i<10; i++)
	{
		// float spec=i*0.1;

		Ray::REFLECTIONS_LIMIT+=1;

		NewScene->SetSamplesPerPixel(samples_per_pixel);
		NewScene->Render();

		sprintf(fileName, "render_%02i_%ispp.png", i, samples_per_pixel);
		NewScene->RenderedImage.write(fileName);
	}

	return 0;
}
