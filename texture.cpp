#include "texture.h"

namespace GeoFrame {
	TextureSettings::TextureSettings(
		bool sRGBLoading, FilterType minFilter, FilterType magFilter,
		WrappingType wrapS, WrappingType wrapT, WrappingType wrapR
	) : mSRGBLoading(sRGBLoading), mMinFilter(minFilter), mMagFilter(magFilter),
		mWrapS(wrapS), mWrapT(wrapT), mWrapR(wrapR)
	{ ; }

	Texture::Texture(
		unsigned width, unsigned height, AttachmentType type = AttachmentType::COLOR,
		TextureSettings settings = TextureSettings()
	) : mWidth(width), mHeight(height) {
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_2D, mID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)settings.mWrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)settings.mWrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)settings.mMinFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)settings.mMagFilter);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	Texture::Texture(std::string path, TextureSettings settings = TextureSettings()) {
		unsigned char* bitmap = stbi_load(path.c_str(), (int*)&mWidth, (int*)&mHeight, (int*)&mChannel, 0);
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_2D, mID);

		unsigned format;
		switch (mChannel) {
		case 1:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			format = GL_RED;
			break;
		case 2:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			format = GL_RG;
			break;
		case 3:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			format = NULL;
			break;
		}

		if (format == NULL) {
			throw InvalidFormatError("Invalid texture format.");
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)settings.mWrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)settings.mWrapT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)settings.mMinFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)settings.mMagFilter);

			if (settings.mSRGBLoading) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, bitmap);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, bitmap);
			}

			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		stbi_image_free(bitmap);
	}

	Texture::Texture(const std::vector<unsigned char>& color, TextureSettings settings = TextureSettings())
		: mWidth(1), mHeight(1), mChannel(color.size())
	{
		unsigned r = 0, b = 0, g = 0, a = 0;

		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_2D, mID);

		unsigned format;
		switch (mChannel) {
		case 1:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			format = GL_RED;
			r = color[0];
			break;
		case 2:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			format = GL_RG;
			r = color[0];
			g = color[1];
			break;
		case 3:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			format = GL_RGB;
			r = color[0];
			g = color[1];
			b = color[2];
			break;
		case 4:
			format = GL_RGBA;
			r = color[0];
			g = color[1];
			b = color[2];
			a = color[3];
			break;
		default:
			format = NULL;
			break;
		}

		unsigned char bitmap[4] = {
			(unsigned char)r, (unsigned char)g,
			(unsigned char)b, (unsigned char)a
		};

		if (format == NULL) {
			throw InvalidFormatError("Invalid texture format.");
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)settings.mWrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)settings.mWrapT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)settings.mMinFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)settings.mMagFilter);

			if (settings.mSRGBLoading) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, bitmap);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, bitmap);
			}

			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	Texture::Texture(
		const std::vector<unsigned char>& bitmap,
		unsigned width, unsigned height, unsigned channel,
		TextureSettings settings = TextureSettings()
	) : mWidth(width), mHeight(height), mChannel(channel) {
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_2D, mID);

		unsigned format;
		switch (mChannel) {
		case 1:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			format = GL_RED;
			break;
		case 2:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			format = GL_RG;
			break;
		case 3:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			format = NULL;
			break;
		}

		if (format == NULL) {
			throw InvalidFormatError("Invalid texture format.");
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)settings.mWrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)settings.mWrapT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)settings.mMinFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)settings.mMagFilter);

			if (settings.mSRGBLoading) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, &bitmap[0]);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, &bitmap[0]);
			}

			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	unsigned Texture::GetWidth() const { return mWidth; }

	unsigned Texture::GetHeight() const { return mHeight; }

	unsigned Texture::GetChannel() const { return mChannel; }

	Texture::operator unsigned() const { return mID; }

	void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, mID); }

	void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }


	CubeTexture::CubeTexture(
		unsigned width, unsigned height, AttachmentType type = AttachmentType::COLOR,
		TextureSettings settings = TextureSettings()
	) {
		;
	}

	CubeTexture::CubeTexture(const std::vector<std::string>& paths, TextureSettings settings = TextureSettings()) {
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLint)settings.mWrapS);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLint)settings.mWrapT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, (GLint)settings.mWrapR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLint)settings.mMinFilter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLint)settings.mMagFilter);

		mWidths = { 0, 0, 0, 0, 0, 0 };
		mHeights = { 0, 0, 0, 0, 0, 0 };
		mChannels = { 0, 0, 0, 0, 0, 0 };

		unsigned char* bitmap;
		for (int i = 0; i < 6; i++) {
			bitmap = stbi_load(paths[i].c_str(), (int*)&mWidths[i], (int*)&mHeights[i], (int*)&mChannels[i], 0);

			unsigned format;
			switch (mChannels[i]) {
			case 1:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				format = GL_RED;
				break;
			case 2:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
				format = GL_RG;
				break;
			case 3:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				format = NULL;
				break;
			}

			if (format == NULL) {
				throw InvalidFormatError("Invalid texture format.");
			}
			else {
				if (settings.mSRGBLoading) {
					glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA,
						mWidths[i], mHeights[i], 0, format, GL_UNSIGNED_BYTE, bitmap
					);
				}
				else {
					glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
						mWidths[i], mHeights[i], 0, format, GL_UNSIGNED_BYTE, bitmap
					);
				}
			}
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			stbi_image_free(bitmap);
		}

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	CubeTexture::CubeTexture(const std::vector<std::vector<float>>& colors, TextureSettings settings = TextureSettings()) {
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLint)settings.mWrapS);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLint)settings.mWrapT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, (GLint)settings.mWrapR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLint)settings.mMinFilter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLint)settings.mMagFilter);

		mWidths = { 1, 1, 1, 1, 1, 1 };
		mHeights = { 1, 1, 1, 1, 1, 1 };
		mChannels = { 1, 1, 1, 1, 1, 1 };

		unsigned r, b, g, a;
		for (int i = 0; i < 6; i++) {
			r = g = b = a = 0;

			unsigned format;
			switch (colors[i].size()) {
			case 1:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				format = GL_RED;
				r = colors[i][0];
				break;
			case 2:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
				format = GL_RG;
				r = colors[i][0];
				g = colors[i][1];
				break;
			case 3:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
				format = GL_RGB;
				r = colors[i][0];
				g = colors[i][1];
				b = colors[i][2];
				break;
			case 4:
				format = GL_RGBA;
				r = colors[i][0];
				g = colors[i][1];
				b = colors[i][2];
				a = colors[i][3];
				break;
			default:
				format = NULL;
				break;
			}

			unsigned char bitmap[4] = {
				(unsigned char)r, (unsigned char)g,
				(unsigned char)b, (unsigned char)a
			};

			if (format == NULL) {
				throw InvalidFormatError("Invalid texture format.");
			}
			else {
				if (settings.mSRGBLoading) {
					glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA,
						1, 1, 0, format, GL_UNSIGNED_BYTE, bitmap
					);
				}
				else {
					glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
						1, 1, 0, format, GL_UNSIGNED_BYTE, bitmap
					);
				}
			}
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	CubeTexture::CubeTexture(
		const std::vector<std::vector<unsigned char>>& bitmaps,
		const std::vector<unsigned>& widths,
		const std::vector<unsigned>& heights,
		const std::vector<unsigned>& channels,
		TextureSettings settings = TextureSettings()
	) : mWidths(widths), mHeights(heights), mChannels(channels) {
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLint)settings.mWrapS);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLint)settings.mWrapT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, (GLint)settings.mWrapR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLint)settings.mMinFilter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLint)settings.mMagFilter);

		for (int i = 0; i < 6; i++) {
			unsigned format;
			switch (channels[i]) {
			case 1:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				format = GL_RED;
				break;
			case 3:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				format = NULL;
				break;
			}

			if (format == NULL) {
				throw InvalidFormatError("Invalid texture format.");
			}
			else {
				if (settings.mSRGBLoading) {
					glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA,
						mWidths[i], mHeights[i], 0, format, GL_UNSIGNED_BYTE, &bitmaps[i][0]
					);
				}
				else {
					glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
						mWidths[i], mHeights[i], 0, format, GL_UNSIGNED_BYTE, &bitmaps[i][0]
					);
				}
			}
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	std::vector<unsigned> CubeTexture::GetWidths() const { return mWidths; }
	
	std::vector<unsigned> CubeTexture::GetHeights() const { return mHeights; }
	
	std::vector<unsigned> CubeTexture::GetChannels() const { return mChannels; }

	CubeTexture::operator unsigned() const { return mID; }

	void CubeTexture::Bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, mID); }

	void CubeTexture::Unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }


	Texture3D::Texture3D(const std::vector<float>& color, TextureSettings settings = TextureSettings())
		: mWidth(1), mHeight(1), mDepth(1), mChannel(color.size())
	{
		unsigned r = 0, b = 0, g = 0, a = 0;

		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_3D, mID);

		unsigned format;
		switch (mChannel) {
		case 1:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			format = GL_RED;
			r = color[0];
			break;
		case 2:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			format = GL_RG;
			r = color[0];
			g = color[1];
			break;
		case 3:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			format = GL_RGB;
			r = color[0];
			g = color[1];
			b = color[2];
			break;
		case 4:
			format = GL_RGBA;
			r = color[0];
			g = color[1];
			b = color[2];
			a = color[3];
			break;
		default:
			format = NULL;
			break;
		}

		unsigned char bitmap[4] = {
			(unsigned char)r, (unsigned char)g,
			(unsigned char)b, (unsigned char)a
		};

		if (format == NULL) {
			throw InvalidFormatError("Invalid texture format.");
		}
		else {
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, (GLint)settings.mWrapS);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, (GLint)settings.mWrapT);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, (GLint)settings.mWrapR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, (GLint)settings.mMinFilter);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, (GLint)settings.mMagFilter);

			if (settings.mSRGBLoading) {
				glTexImage3D(GL_TEXTURE_3D, 0, GL_SRGB_ALPHA, mWidth, mHeight, mDepth, 0, format, GL_UNSIGNED_BYTE, bitmap);
			}
			else {
				glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, mWidth, mHeight, mDepth, 0, format, GL_UNSIGNED_BYTE, bitmap);
			}

			glGenerateMipmap(GL_TEXTURE_3D);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	Texture3D::Texture3D(
		const std::vector<unsigned char>& bitmap,
		unsigned width, unsigned height, unsigned depth, unsigned channel,
		TextureSettings settings = TextureSettings()
	) : mWidth(width), mHeight(height), mDepth(depth), mChannel(channel)
	{
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_3D, mID);

		unsigned format;
		switch (mChannel) {
		case 1:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			format = GL_RED;
			break;
		case 2:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			format = GL_RG;
			break;
		case 3:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			format = NULL;
			break;
		}

		if (format == NULL) {
			throw InvalidFormatError("Invalid texture format.");
		}
		else {
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, (GLint)settings.mWrapS);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, (GLint)settings.mWrapT);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, (GLint)settings.mMinFilter);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, (GLint)settings.mMagFilter);

			if (settings.mSRGBLoading) {
				glTexImage3D(GL_TEXTURE_3D, 0, GL_SRGB_ALPHA, mWidth, mHeight, mDepth, 0, format, GL_UNSIGNED_BYTE, &bitmap[0]);
			}
			else {
				glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, mWidth, mHeight, mDepth, 0, format, GL_UNSIGNED_BYTE, &bitmap[0]);
			}

			glGenerateMipmap(GL_TEXTURE_3D);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	unsigned Texture3D::GetWidth() const { return mWidth; }
	unsigned Texture3D::GetHeight() const { return mHeight; }
	unsigned Texture3D::GetDepth() const { return mDepth; }
	unsigned Texture3D::GetChannel() const { return mChannel; }

	Texture3D::operator unsigned() const { return mID; }

	void Texture3D::Bind() const { glBindTexture(GL_TEXTURE_3D, mID); }
	void Texture3D::Unbind() const { glBindTexture(GL_TEXTURE_3D, 0); }
}