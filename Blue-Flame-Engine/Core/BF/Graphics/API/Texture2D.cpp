#include "Texture2D.h"
#include "BF/Graphics/API/Context.h"


namespace BF
{
	namespace Graphics
	{
		namespace API
		{
			using namespace std;

			Texture2D::Texture2D(const Shader& shader) :
				shader(shader), glTexture2D(shader.glShader)
			{
			}

			Texture2D::~Texture2D()
			{
			}

			void Texture2D::Load(const string& filename)
			{
				Texture::Load(filename);

#ifdef BF_PLATFORM_WINDOWS
				if (Context::GetRenderAPI() == RenderAPI::DirectX)
					dxTexture2D.Create(*textureData, Format::R8G8B8A8, Wrap::Repeat, Filter::Point);
#endif
#if defined (BF_PLATFORM_WINDOWS) || defined (BF_PLATFORM_LINUX) || defined (BF_PLATFORM_WEB) || defined (BF_PLATFORM_ANDROID)
				if (Context::GetRenderAPI() == RenderAPI::OpenGL)
					glTexture2D.Create(*textureData, Format::R8G8B8A8, Wrap::Repeat, Filter::Point);
#endif
			}

			void Texture2D::Load(const string& filename, Wrap wrap, Filter filter)
			{
				Texture::Load(filename);

#ifdef BF_PLATFORM_WINDOWS
				if (Context::GetRenderAPI() == RenderAPI::DirectX)
					dxTexture2D.Create(*textureData, Format::R8G8B8A8, wrap, filter);
#endif
#if defined (BF_PLATFORM_WINDOWS) || defined (BF_PLATFORM_LINUX) || defined (BF_PLATFORM_WEB) || defined (BF_PLATFORM_ANDROID)
				if (Context::GetRenderAPI() == RenderAPI::OpenGL)
					glTexture2D.Create(*textureData, Format::R8G8B8A8, wrap, filter);
#endif
			}

			void Texture2D::Create(TextureData& textureData, Format format)
			{
				this->textureData = &textureData;

#ifdef BF_PLATFORM_WINDOWS
				if (Context::GetRenderAPI() == RenderAPI::DirectX)
					dxTexture2D.Create(textureData, format, Wrap::Repeat, Filter::Point);
#endif
#if defined (BF_PLATFORM_WINDOWS) || defined (BF_PLATFORM_LINUX) || defined (BF_PLATFORM_WEB) || defined (BF_PLATFORM_ANDROID)
				if (Context::GetRenderAPI() == RenderAPI::OpenGL)
					glTexture2D.Create(textureData, format, Wrap::Repeat, Filter::Point);
#endif
			}

			void Texture2D::Create(TextureData& textureData, Format format, Wrap wrap, Filter filter)
			{
				this->textureData = &textureData;

#ifdef BF_PLATFORM_WINDOWS
				if (Context::GetRenderAPI() == RenderAPI::DirectX)
					dxTexture2D.Create(textureData, format, wrap, filter);
#endif
#if defined (BF_PLATFORM_WINDOWS) || defined (BF_PLATFORM_LINUX) || defined (BF_PLATFORM_WEB) || defined (BF_PLATFORM_ANDROID)
				if (Context::GetRenderAPI() == RenderAPI::OpenGL)
					glTexture2D.Create(textureData, format, wrap, filter);
#endif
			}

			void Texture2D::Bind() const
			{
#ifdef BF_PLATFORM_WINDOWS
				if (Context::GetRenderAPI() == RenderAPI::DirectX)
					dxTexture2D.Bind(0);
#endif
#if defined (BF_PLATFORM_WINDOWS) || defined (BF_PLATFORM_LINUX) || defined (BF_PLATFORM_WEB) || defined (BF_PLATFORM_ANDROID)
				if (Context::GetRenderAPI() == RenderAPI::OpenGL)
					glTexture2D.Bind();
#endif
			}

			void Texture2D::Bind(const string& samplerName, unsigned int index) const
			{
#ifdef BF_PLATFORM_WINDOWS
				if (Context::GetRenderAPI() == RenderAPI::DirectX)
					dxTexture2D.Bind(index);
#endif
#if defined (BF_PLATFORM_WINDOWS) || defined (BF_PLATFORM_LINUX) || defined (BF_PLATFORM_WEB) || defined (BF_PLATFORM_ANDROID)
				if (Context::GetRenderAPI() == RenderAPI::OpenGL)
					glTexture2D.Bind(samplerName, index);
#endif
			}

			void Texture2D::Unbind() const
			{

#ifdef BF_PLATFORM_WINDOWS
#endif
#if defined (BF_PLATFORM_WINDOWS) || defined (BF_PLATFORM_LINUX) || defined (BF_PLATFORM_WEB) || defined (BF_PLATFORM_ANDROID)
				if (Context::GetRenderAPI() == RenderAPI::OpenGL)
					glTexture2D.Unbind();
#endif
			}

			void Texture2D::Unbind(const string& samplerName, unsigned int index) const
			{
#ifdef BF_PLATFORM_WINDOWS
#endif
#if defined (BF_PLATFORM_WINDOWS) || defined (BF_PLATFORM_LINUX) || defined (BF_PLATFORM_WEB) || defined (BF_PLATFORM_ANDROID)
				if (Context::GetRenderAPI() == RenderAPI::OpenGL)
					glTexture2D.Unbind(samplerName, index);
#endif
			}
		}
	}
}