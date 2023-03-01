#include <Frame/app.h>

class AppImpl : public Frame::App
{
public:
	virtual void initScene()
	{

		 
	}

};

int main(int argc, char** argv)
{
	runMain<AppImpl>(argc,argv);
}