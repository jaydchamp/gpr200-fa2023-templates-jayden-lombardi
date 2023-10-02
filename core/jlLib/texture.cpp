#include "texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"

unsigned int loadTexture(const char* filePath, int wrapMode, int filterMode) {

	stbi_set_flip_vertically_on_load(true);

	int width, height, numComponents;
	unsigned char* data = stbi_load(filePath, &width, &height, &numComponents, 0);

	if (data == NULL) 
	{
			printf("Failed to load image %s", filePath);
			stbi_image_free(data);
			return 0;
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLenum format, internalFormat;

	if (numComponents == 1)
	{
		format = GL_RED;
		internalFormat = GL_RED;
	}
	else if (numComponents == 2)
	{
		format = GL_RG;
		internalFormat = GL_RG;
	}
	else if (numComponents == 3)
	{
		format = GL_RGB;
		internalFormat = GL_RGB;
	}
	else if (numComponents == 4)
	{
		format = GL_RGBA;
		internalFormat = GL_RGBA;
	}
	else 
	{
		printf("Unsupported number of components: %d", numComponents);
		stbi_image_free(data);
		return 0;
	}

	//set wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	//set filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return texture;
}