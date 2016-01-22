##ifndef SCREEN_H
#define SCREEN_H

class Screen {

private:
	unsigned int *buffer;

public:
	Screen(unsigned int width, unsigned int height);

	void setPixel(int x, int y, unsigned int val);

	unsigned int getPixel(int x, int y);
};

#endif
